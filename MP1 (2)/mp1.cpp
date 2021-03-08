/*
*	@author: Nishant Bhushan
*		bhushan3
*/
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "util/hittable.h"
#include "util/ray.cpp"
#include "util/sphere.cpp"
#include "util/triangle.cpp"
#include "util/plane.cpp"
#include "util/hittable_list.cpp"
#include "util/util.h"
#include "util/camera.h"

using namespace std;
using std::string;

/* Whats happening with ray-sphere intersection in perspective view?

/* t values greater than 1 if you only want objects
	beyond the viewplane and dont care if there is
	any object between the eyepoint and viewplane.
	Ignore t values between [0,1]
*/

/* Perspective Distortion: Occurs due to the viewplane being flat.
	angle of the rays and the relative scale of nearby to distant
	objects causes objects closer to the viewplane to
	look bigger and objects further away to look smaller.
	Can be fixed by moving eyepoint back away from objects.
	(+z direction)
*/

/* Note: Our retinas are spherical so our eyes don't experience this
	phenomena.
*/

/* Zoom (Perspective): Shift the eyepoint closer to the viewplane
	to zoom in. Shift the eyepoint away from the 
	viewplane to zoom out. (-z direction) 
*/

/* Zoom (Orthographic): Decrease s (pixel extent) while keeping
	the aspect_ratio (resolution) fixed or vice versa to zoom in.
	Increase s (pixel extent) while keeping the aspect_ratio
	(resolution) fixed or vice versa. 
*/

/* Shadows (lighting): To determine whether a hitpoint on an object is
	a shadow, We send a ray from the hitpoint to all light sources. 
	If the ray intersects an object for t > 0 (meaning it is toward the 
	light source), then that object is casting a shadow on the
	hitpoint. So the color returned is black? 
*/

/* Up vector and view direction should not be parallel else the cross product
	will give a 0 vector and we wont be able to construct the local camera frame.
	We can check if the dot product is 1, then they are parallel.
*/

/* Ray Sphere intersection: When trying to solve the quadratic equation
	ax^2 + b^x + c = 0:
	if sign(b) = 1, then do -b - sqrt(b^2 - 4ac) / 2a
	if sign(b) = -1 then do -b + sqrt(b^2 - 4ac) / 2a
	The other root can be found using:
	c/(a*x). 
	Note: If the origin of the sphere is at a distance more than
	(2^12)r = 4096r from the ray origin then the radius will have no effect
	on the ray-sphere intersection solution. We will start seeing artifacts way
	before this distance is reached. This is diminished significance.

	We only care for points where t > 1
*/

/* If there is a barycentric coordinate < 0, then
* the point lies outside the triangle.
*/

struct interval {
	double min;
	double max;
	bool closed;
};

// interval vector
vector<interval> intervals; 

// Phong reflection
vec3 ka = vec3(0.5,0.3,0.7); 	// ambient material
vec3 la = vec3(0.4,0.23,0.1);	// ambient colo
//vec3 ka = vec3(0,0,0); 	// ambient material
//vec3 la = vec3(0,0,0);	// ambient color
vec3 lightPos = vec3(-500,-200,1200);
//vec3 lightPos = vec3(1,1,1);
//float alpha = 1;	// shininess coefficient;

/* Writes the color to the output stream
*	@out: The output stream to write the color to
*	@pixel_color: The color to write
*/
void write_color(std::ostream &out, vec3 pixel_color, int samples_per_pixel) {
    // Write the translated [0,255] value of each color component.
	auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    out << static_cast<int>(255.999 * r) << ' '
        << static_cast<int>(255.999 * g) << ' '
        << static_cast<int>(255.999 * b) << '\n';
}

/* Clamps the value of components of color.
*	to the interval [0,1]
*	@color: The vec3 to clamp
*	a vec3 with components clamped to [0,1].
*/
vec3 clamp(vec3 color) {
	color[0] = (color[0] > 1) ? 1 : color[0];
	color[1] = (color[1] > 1) ? 1 : color[1];
	color[2] = (color[2] > 1) ? 1 : color[2];
	color[0] = (color[0] < 0) ? 0 : color[0];
	color[1] = (color[1] < 0) ? 0 : color[1];
	color[2] = (color[2] < 0) ? 0 : color[2];
	return color;
}

/* Does Phong Shading using Blinn-Phong model
*	@hitpoint: The solution to the ray equation. 
*			(The point on the ray intersecting the sphere.)
*	@n: normal to the surface
*	@kd: diffuse material component
*	@ld: diffuse light color
*	returns a vec3 color of the pixel shaded using phong shading
*/
vec3 phong(vec3 hitpoint,vec3 n, vec3 kd, vec3 ld) {
	vec3 L = normalize(lightPos - hitpoint);	// clamp L and N maybe?
	vec3 N = normalize(n);

	double d = dot(L,N);
	if (d < 0) d = 0;
	vec3 color = (ka*la) + (kd*d*ld);
	return clamp(color);
}

/* Generates a list of intervals based on n and s
*	@n: A perfect square, the number of intervals
*	@s: pixel extent
*	returns a list of intervals from [si/n,s(i+1)/n]
*/
void generateIntervals(int n, int s) {
	// n must be a perfect square
	// coarse grid is sqrt(n)*sqrt(n)
	// fine grid is n*n
	int course_grid_size = sqrt(n) * sqrt(n);
	int fine_grid_size = n * n;
	for (int i = 0; i < n; i++) {
		interval in;
		double g = double(i)/double(n);
		double h = double((i+1))/double(n);
		in.min = s*g + 1e-6;
		in.max = s*h;
		in.closed = true;
		intervals.push_back(in);
	}
}

/* Determines if an int is in a vector of ints
*	@n: element to search for
*	@list: vector of ints
*	returns true if element is in list, false otherwise.
*/
bool contains(int n, vector<int>& list) {
	if (list.size() == 0) return false;
	for (int i = 0; i < list.size(); i++) {
		if (n == list[i]) return true;
	}
	return false;
}


/*	Gets the dx and dy used for jittering
*	@intervals: the list of intervals
*	@n: samples per pixel
*	returns random dx and dy values based on multi-jittered sampling
*/
vector<vec3> getdxdy(int n) {
	double dx = 0;
	double dy = 0;
	int ysize = n;
	vector<vec3> vecs;
	vector<int> deleted_fine;
	int j = 0;
	int d = 1;

	for (int k = 0; k < int(sqrt(n)); k++) {
		ysize = n;
		vector<int> deleted_coarse;
		for (int i = j; i < (d)*int(sqrt(n)); i++) {
			interval inx = intervals[i];
			dx = random_double(inx.min, inx.max);
			int yi = rand() % ysize;
			while (contains(yi,deleted_coarse) && contains(yi, deleted_fine)) {
				yi = rand() % ysize;
			}

			interval iny = intervals[yi];
			dy = random_double(iny.min, iny.max);
			deleted_fine.push_back(yi);
		
			int m = yi % int(sqrt(n));
			int r = int(sqrt(n)) - m;
			// delete m numbers before and then
			// sqrt(n) - m intervals after including
			// current one
			for (int l = yi-m; l < yi + r; l++) {
				deleted_coarse.push_back(l);
			}
		
			vecs.push_back(vec3(dx,dy,0));
		}
		j += int(sqrt(n));
		d += 1;
	}

	return vecs;
}

/* Casts a ray to determine if it hits any objects in the scene.
*	@r: The ray to cast.
*	returns the color for a pixel at a point on the viewplane.
*/
vec3 raycast(ray r, const hittable& world) {
	hit_record rec;
	if (world.hit(r,0,infinity,rec)) {
		// Shadows
		// create a ray from hitpoint to all light sources
		vec3 hitpoint = rec.p;
		vec3 norm_dir = normalize(lightPos - hitpoint);
		double eps = 1e-5;
		ray shadow_ray = ray(hitpoint + vec3(eps,eps,eps)*norm_dir, norm_dir);
		hit_record new_rec;
		if (world.hit(shadow_ray,0,infinity,new_rec)) {
			// color at that point is black
			return vec3(0,0,0);
		}
		return phong(rec.p, rec.n, rec.kd, rec.ld);
	}

	vec3 unit_direction = normalize(r.direction());
    double t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

/* The main method to run everything.
*	compile using: g++ mp1.cpp -std=c++11 -o mp1
*	./mp1 0 400 1.7 > output.ppm
*	@argc: The size of args array
*	@args: The arguments provided by the command line
*	@args[0] - './mp1'
*	@args[1] - 1 for orthographic, otherwise perspective projection
*	is used.
*	@args[2] - image width (400 by default)
*	@args[3] - aspect ratio (resolution of image). (16/9 by default)
*	@args[4] - image height (optional)(if used, aspect ratio is discarded).
*	returns 0 on successful completion.
*/
int main(int argc, char** args) {
	int ortho = (argc == 1 || args[1][0] == '1') ? 1 : 0;

	// World stuff
	hittable_list world;
	world.add(make_shared<plane>(vec3(0,0,-400), vec3(0,0,1), vec3(0.8,0.5,0.8), vec3(1,0.4,0.6)));
    //world.add(make_shared<sphere>(vec3(0,0,-2), 1.95, vec3(1,0,0), vec3(1,1,1)));
	//world.add(make_shared<triangle>(vec3(50-100,50-100,-1.5), vec3(0-100,-50-100,-1.5), vec3(100-100,-50-100,-1.5), vec3(0.5,0.4,0.8), vec3(0.5,0.5,0.5)));
    //world.add(make_shared<sphere>(vec3(0,-100.5,-1), 100, vec3(1,1,1), vec3(1,1,1)));

    //world.add(make_shared<sphere>(vec3(-50,0,-52),49.99,vec3(0,0,1),vec3(1,1,1)));
    //world.add(make_shared<sphere>(vec3(0,-100.5,-120),100,vec3(66.0/255.0, 221.0/255.0, 245.0/255.0),vec3(1,1,1)));

	// perspective
	world.add(make_shared<triangle>(vec3(50-100,50-100,100), vec3(0-100,-50-100,100), vec3(100-100,-50-100,100), vec3(0.5,0.4,0.8), vec3(0.5,0.5,0.5)));
	world.add(make_shared<sphere>(vec3(-50,0,0),49.99,vec3(0,0,1),vec3(1,1,1)));
	world.add(make_shared<sphere>(vec3(0,-100.5,0),100,vec3(66.0/255.0, 221.0/255.0, 245.0/255.0),vec3(1,1,1)));

	// Image
	int image_width = 400;
	float aspect_ratio = 16/9;
	int image_height = static_cast<int>(image_width/aspect_ratio);
	if (argc == 4) {
		image_width = strtol(args[2],&args[2],10);
		aspect_ratio = atof(args[3]);
		image_height = static_cast<int>(image_width / aspect_ratio);

	} else if (argc == 5) {
		image_width = strtol(args[2],&args[2],10);
		image_height = strtol(args[4],&args[4],10);
		aspect_ratio = image_width/image_height;
	}

	//const vec3 eyepoint = vec3(1,1,400); // perspective projection
	const vec3 eyepoint = vec3(-250,250,400); // perspective projection
	const int s = 1; // pixel extent
	const int d = 1; // focal length, viewport/plane is at (0,0,-1)
	vec3 up = vec3(0,1,0); // Up direction (world space)

	// orthonormal basis vectors for camera space:
	const vec3 camera_origin = eyepoint; // camera space
	vec3 viewdir = vec3(0,0,-1); // viewdir
	camera cam = camera(aspect_ratio, camera_origin, viewdir, up, d, ortho);
	//camera cam;

	const int samples_per_pixels = 100;	// 100
	generateIntervals(samples_per_pixels,s);
	vector<vec3> vecs = getdxdy(samples_per_pixels);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
			// Apply anti-aliasing method-
			// Multi-jittered sampling
			double dx,dy = 0;
			vec3 color = vec3(0,0,0);
			for (int k = 0; k < samples_per_pixels; k++) {
				vec3 dxdy = vecs[k];
				dx = dxdy.x();
				dy = dxdy.y();
				double x = s*(double(i) - (image_width/2) + dx);
				double y = s*(double(j) - (image_height/2) + dy);
				ray r = cam.get_ray(x,y);
				color += raycast(r, world);
			}
			write_color(cout,color,samples_per_pixels);
        }
    }

	
	return 0;
}
