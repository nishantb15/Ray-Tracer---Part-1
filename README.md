# Ray-Tracer---Part-1
Ray Tracer that can project a ray orthographically or perspectively and test for the rays intersection with spheres, triangles and planes. Colors for the pixel are generated using Phong shading model. Anti-aliasing (Multi-Sampled jittering) helps prevent the staircase like appeareance of curved edges. It smooths the surfaces by casting many rays, adding up the colors at each point then dividing by the number of rays. Shadows are generated by assuming light as point sources. Part 2 of this project implements a Bounding Volume Hierarchy (BVH) and adds support for reading triangular meshes from a .obj file. You can find it here: https://github.com/nishantb15/MP2---Part-2---BVH-and-Mesh