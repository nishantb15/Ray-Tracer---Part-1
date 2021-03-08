#include "vec3.h"

#include <cmath>
#include <iostream>

using std::sqrt;

/* Empty Constructor for vec3
*/
vec3::vec3() {
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
}

/* Constructor for vec3
	@v1: x component
	@v2: y component
	@v3: z component
*/
vec3::vec3(double v1, double v2, double v3) {
	v[0] = v1;
	v[1] = v2;
	v[2] = v3;
}

/*vec3(vec3& v2) {
	v[0] = v2[0];
	v[1] = v2[1];
	v[2] = v2[2];
}*/

/* Returns the x-component of the vector.
*/
double vec3::x() {
	return v[0];
}

/* Returns the y-component of the vector.
*/
double vec3::y() {
	return v[1];
}

/* Returns the z-component of the vector.
*/
double vec3::z() {
	return v[2];
}

/* Returns the length of the vector (equlidian 2-norm).
*/
double vec3::length() {
	return sqrt((v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2]));
}

/* Returns the square of the length of the vector (euclidian 2-norm).
*/
double vec3::length_squared() {
	return length()*length();
}

/* Returns a vector with all its components negated.
*/
vec3 vec3::operator-() const {
	return vec3(-v[0],-v[1],-v[2]);
}

/* Used to return the ith element of v. 
*/
double vec3::operator[](int i) const {
	return v[i];
}

/* Used to return the ith element of v. 
*/
double& vec3::operator[](int i) {
	return v[i];
}

/* This operator does a component wise addition
*	of v and v1.
*	@v1: the vector whose components will be added to v
*	returns: A reference to itself. 
*/
vec3& vec3::operator+=(const vec3 &v1) {
	v[0] += v1.v[0];
	v[1] += v1.v[1];
	v[2] += v1.v[2];
	return *this;
}

/* This operator does a scalar multiplication
*	of v and t.
*	@t: the scalar value.
*	returns: A reference to itself. 
*/
vec3& vec3::operator*=(const double t) {
	v[0] *= t;
	v[1] *= t;
	v[2] *= t;
	return *this;
}

/* This operator does a scalar multiplication withv and
*	the reciprocal of t.
*	@v1: the scalar value.
*	returns: A reference to itself. 
*/
vec3& vec3::operator/=(const double t) {
	v[0] /= t;
	v[1] /= t;
	v[2] /= t;
	return *this;
}

/* Allows us to easily print out the contents of a vec3.
*	@out: The ouput stream to write to.
*	@v1: the vector whose components will be added to v
*	returns: A reference to itself. 
*/
std::ostream& operator<<(std::ostream &out, const vec3 &v1) {
    return out << v1[0] << ' ' << v1[1] << ' ' << v1[2];
}

/* Performs a component-wise addition ofS two vectors.
*	@u1: the first vector
*	@u2: the second vector
*	returns: A new vec3 whose components are the sum of 
*	the components of u1 and u2.
*/
vec3 operator+(const vec3 &u1, const vec3 &u2) {
    return vec3(u1[0] + u2[0], u1[1] + u2[1], u1[2] + u2[2]);
}

/* Performs a component-wise subtraction of two vectors.
*	@u1: the first vector
*	@u2: the second vector
*	returns: A new vec3 whose components are the difference of 
*	the components of u1 and u2.
*/
vec3 operator-(const vec3 &u1, const vec3 &u2) {
    return vec3(u1[0] - u2[0], u1[1] - u2[1], u1[2] - u2[2]);
}

/* Performs a component-wise multiplication of two vec3 objects.
*	@u1: the first vector
*	@u2: the second vector
*	returns: A new vec3 whose components are the product of 
*	the components of u1 and u2.
*/
vec3 operator*(const vec3 &u1, const vec3 &u2) {
    return vec3(u1[0] * u2[0], u1[1] * u2[1], u1[2] * u2[2]);
}

/* Performs a scalar multplication.
*	@t: scalar value
*	@v1: the vec3 to multiply with
*	returns: A new vec3 whose components are multiplied
*	by t
*/
vec3 operator*(double t, const vec3 &v1) {
    return vec3(t*v1[0], t*v1[1], t*v1[2]);
}

/* Performs a scalar multplication.
*	@v: the vec3 to multiply with
*	@t: scalar value
*	returns: A new vec3 whose components are multiplied
*	by t.
*/
vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

/* Performs a scalar multplication.
*	@v: the vec3 to multiply with
*	@t: reciprocal of scalar value
*	returns: A new vec3 whose components are multiplied
*	by 1/t.
*/
vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

/* Performs a dot product
*	@u1: First vec3
*	@u2: Second vec3
*	returns the dot product of u1 and u2.
*/
double dot(const vec3 &u1, const vec3 &u2) {
    return u1[0] * u2[0]
         + u1[1] * u2[1]
         + u1[2] * u2[2];
}

/* Performs a cross product
*	@u1: First vec3
*	@u2: Second vec3
*	returns the cross product of u1 and u2.
*/
vec3 cross(const vec3 &u1, const vec3 &u2) {
    return vec3(u1[1] * u2[2] - u1[2] * u2[1],
                u1[2] * u2[0] - u1[0] * u2[2],
                u1[0] * u2[1] - u1[1] * u2[0]);
}

/* normalizes the vec3 v
*	@v: Vec3 to normalize.
*	returns the normalized vec3.
*/
vec3 normalize(vec3 v) {
    return v / v.length();
}
