#include "geometry.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace std;
using namespace glm;

bool CheckGeoGLErrors()
{
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			cout << "GL_INVALID_ENUM" << endl; break;
		case GL_INVALID_VALUE:
			cout << "GL_INVALID_VALUE" << endl; break;
		case GL_INVALID_OPERATION:
			cout << "GL_INVALID_OPERATION" << endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl; break;
		case GL_OUT_OF_MEMORY:
			cout << "GL_OUT_OF_MEMORY" << endl; break;
		default:
			cout << "[unknown error code]" << endl;
		}
		error = true;
	}
	return error;
}


bool InitializeVAO(Geometry *geometry){

	const GLuint VERTEX_INDEX = 0;
//	const GLuint COLOUR_INDEX = 1;
	const GLuint TEXTURE_INDEX = 1;
	const GLuint NORMAL_INDEX = 2;

	//Generate Vertex Buffer Objects
	// create an array buffer object for storing our vertices
	glGenBuffers(1, &geometry->vertexBuffer);

	// create another one for storing our colours
//	glGenBuffers(1, &geometry->colourBuffer);
	
	glGenBuffers(1, &geometry->textureBuffer);
	glGenBuffers(1, &geometry->normalBuffer);
	glGenBuffers(1, &geometry->indicesBuffer);

	//Set up Vertex Array Object
	// create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &geometry->vertexArray);
	glBindVertexArray(geometry->vertexArray);

	// associate the position array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glVertexAttribPointer(
		VERTEX_INDEX,		//Attribute index 
		3, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(vec3),		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(VERTEX_INDEX);
/*
	// associate the colour array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
	glVertexAttribPointer(
		COLOUR_INDEX,		//Attribute index 
		3, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(vec3), 		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(COLOUR_INDEX);
*/
	glBindBuffer(GL_ARRAY_BUFFER, geometry->textureBuffer);
	glVertexAttribPointer(
		TEXTURE_INDEX,		//Attribute index 
		2, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(vec2),		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(TEXTURE_INDEX);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
	glVertexAttribPointer(
		NORMAL_INDEX,		//Attribute index 
		3, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(vec3), 		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(NORMAL_INDEX);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->indicesBuffer);

	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return !CheckGeoGLErrors();
}

// create buffers and fill with geometry data, returning true if successful
bool LoadGeometry(Geometry *geometry, vec3 *vertices, vec2 *textures, vec3 *normals, int elementCount)
//bool LoadGeometry(Geometry *geometry, vector<vec3> *vertices, vector<vec2> &textures, vector<vec3> &normals, vector<unsigned int> &indices)
{
	geometry->elementCount = elementCount;

	// create an array buffer object for storing our vertices
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*geometry->elementCount, vertices, GL_STATIC_DRAW);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*vertices.size(), &vertices, GL_STATIC_DRAW);

	// create another one for storing our colours
//	glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*geometry->elementCount, colours, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*geometry->elementCount, textures, GL_STATIC_DRAW);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*textures.size(), &textures, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*geometry->elementCount, normals, GL_STATIC_DRAW);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*normals.size(), &normals, GL_STATIC_DRAW);
	
/*	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->indicesBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int)*indices.size(),
		&indices,
		GL_STATIC_DRAW
		);
	*/
	//Unbind buffer to reset to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGeoGLErrors();
}

bool LoadVBO(Geometry *geometry, const vector<vec3> &vertices, const vector<vec2> &textures, const vector<vec3> &normals, const vector<unsigned int> &indices)//, int vn, int un, int nn)
//bool LoadGeometry(Geometry *geometry, vector<vec3> *vertices, vector<vec2> &textures, vector<vec3> &normals, vector<unsigned int> &indices)
{
//	geometry->elementCount = elementCount;
	// create an array buffer object for storing our vertices
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*vn, vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
//	cout << "DFAD" << endl;
	// create another one for storing our colours
//	glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*geometry->elementCount, colours, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->textureBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*un, textures, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*textures.size(), &textures[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*nn, normals, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*normals.size(), &normals[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->indicesBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int)*indices.size(),
		&indices[0],
		GL_STATIC_DRAW
		);
	
	//Unbind buffer to reset to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGeoGLErrors();
}

bool LoadIndices(Geometry *geometry, unsigned int *indices, int nIndices) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->indicesBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int)*nIndices,
		indices,
		GL_STATIC_DRAW
		);
	return true;
}

// deallocate geometry-related objects
void DestroyGeometry(Geometry *geometry)
{
	// unbind and destroy our vertex array object and associated buffers
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &geometry->vertexArray);
	glDeleteBuffers(1, &geometry->vertexBuffer);
//	glDeleteBuffers(1, &geometry->colourBuffer);
	glDeleteBuffers(1, &geometry->normalBuffer);
	glDeleteBuffers(1, &geometry->textureBuffer);
}
