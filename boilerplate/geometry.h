// ==========================================================================
// Barebones OpenGL Core Profile Boilerplate
//    using the GLFW windowing system (http://www.glfw.org)
//
// Loosely based on
//  - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
//  - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)
//
// Author:  Sonny Chan, University of Calgary
// Co-Authors:
//			Jeremy Hart, University of Calgary
//			John Hall, University of Calgary
// Modified by:
//			Adrian Bathan, University of Calgary (30011953)
// Date:    December 2015
// Modified on: February 28, 2018
// Citation:
//		https://medium.com/@evanwallace/easy-scalable-text-rendering-on-the-gpu-c3f4d782c5ac
//		https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch25.html
//		https://www.fontsquirrel.com/fonts/list/find_fonts?filter%5Btags%5D%5B0%5D=decorative&filter%5Btags%5D%5B1%5D=elegant&sort=hot
//		https://www.martinstoeckli.ch/fontmap/fontmap.html
//		http://www.shaderific.com/glsl-functions/
//		https://en.wikibooks.org/wiki/GLSL_Programming/GLUT/Transparency
// ==========================================================================


#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace glm;
using namespace std;

struct Geometry
{
	// OpenGL names for array buffer objects, vertex array object
	GLuint  vertexBuffer;
	GLuint  textureBuffer;
	GLuint  normalBuffer;
	GLuint indicesBuffer;
//	GLuint  colourBuffer;
	GLuint  vertexArray;
	GLsizei elementCount;
	

	// initialize object names to zero (OpenGL reserved value)
//	Geometry() : vertexBuffer(0), colourBuffer(0), vertexArray(0), elementCount(0)
	Geometry() : vertexBuffer(0), textureBuffer(0), normalBuffer(0), vertexArray(0), elementCount(0)
	{}
};

bool InitializeVAO(Geometry *geometry);
bool LoadGeometry(Geometry *geometry, vec3 *vertices, vec2 *textures, vec3 *normals, int elementCount);
bool LoadVBO(Geometry *geometry, const vector<vec3> &vertices, const vector<vec2> &textures, const vector<vec3> &normals, const vector<unsigned int> &indices);//, int vn, int un, int nn);
bool LoadIndices(Geometry *geometry, unsigned int *indices, int nIndices);
//bool LoadGeometry(Geometry *geometry, vector<vec3> vertices, vector<vec2> textures, vector<vec3> normals, vector<unsigned int> indices);
void DestroyGeometry(Geometry *geometry);
