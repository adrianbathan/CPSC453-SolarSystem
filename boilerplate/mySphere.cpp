#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "mySphere.h"
#include "texture.h"

using namespace glm;
using namespace std;

void mySphere::generateSphere(char * filename) {
	if (!InitializeVAO(&this->sphereGeometry))
			cout << "Program failed to intialize geometry!" << endl;
	if (!InitializeTexture(&this->sphereTexture, filename, GL_TEXTURE_2D))
		cout << "Program failed to initialize texture" << endl;
	this->center = vec3(this->parent->getOffset(), 0.0f, 0.0f)+vec3(this->offset, 0.0f, 0.0f);
	int nTris = 120;
	float interval = 1.0f/119.0f;
	vec3 point;
	float phi = 0.0f;
	for(int i=0; i<nTris; i++) {
		float theta = 0.0f;
		for(int j=0; j<nTris; j++) {
			point = this->radius * vec3(cos(2.0f*M_PI*theta) * sin(M_PI*phi),
								sin(2.0f*M_PI*theta) * sin(M_PI*phi),
								cos(M_PI*phi)) + this->center;
			vec3 normal = normalize(point - this->center);
			this->points.push_back(point);
			this->normals.push_back(normal);
			this->uvs.push_back(vec2(1-theta, phi));
			theta = theta + interval;
		}
		phi = phi + interval;
	}
	for(int i=0; i<119; i++) {
		for(int j=0; j<119; j++) {
			unsigned int pBL = i*120+j;
			unsigned int pBR = i*120+j+1;
			unsigned int pTL = (i+1)*120 + j;
			unsigned int pTR = (i+1)*120 + j + 1;
			this->indices.push_back(pBL);
			this->indices.push_back(pBR);
			this->indices.push_back(pTL);
			this->indices.push_back(pTL);
			this->indices.push_back(pBR);
			this->indices.push_back(pTR);
		}
	}
}

//void mySphere::rotate(float angle) {
void mySphere::rotate() {
	float angle = this->rotation/this->speed;
	vec3 axis = normalize(this->axis);
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    mat3 rotationMatrix = mat3(	c + x*x*oc,	x*y*oc - z*s,	x*z*oc + y*s,
    							y*x*oc + z*s,	c + y*y*oc,	y*z*oc - x*s,
    							z*x*oc - y*s,	z*y*oc + x*s,	c + z*z*oc);
	for (int i=0; i<points.size(); i++){
    	this->points[i] = ((this->points[i] - this->center) * rotationMatrix) + this->center;
    	this->normals[i] = normalize(this->points[i] - this->center);
    }
}
void mySphere::rotate0(float angle) {
	vec3 axis = normalize(this->parent->getAxis());
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    mat3 rotationMatrix = mat3(	c + x*x*oc,	x*y*oc - z*s,	x*z*oc + y*s,
    							y*x*oc + z*s,	c + y*y*oc,	y*z*oc - x*s,
    							z*x*oc - y*s,	z*y*oc + x*s,	c + z*z*oc);
	for (int i=0; i<points.size(); i++){
    	this->points[i] = ((this->points[i] - this->center) * rotationMatrix) + this->center;
    	this->normals[i] = normalize(this->points[i] - this->center);
    }
}
void mySphere::orbit() {
	float angle = this->solorbit/this->speed;
	vec3 axis = normalize(this->incline);
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	float s = sin(angle);
	float c = cos(angle);
	float oc = 1.0f - c;
	this->rotate0(-angle);
	mat3 rotationMatrix = mat3(	c + x*x*oc,	x*y*oc - z*s,	x*z*oc + y*s,
								y*x*oc + z*s,	c + y*y*oc,	y*z*oc - x*s,
								z*x*oc - y*s,	z*y*oc + x*s,	c + z*z*oc);
//	if (strcmp(this->sphereName,"moon")==0)
//		this->center = ((this->center - this->parent->getCenter()) * rotationMatrix) + this->parent->getCenter();
//	else
		this->center = ((this->center - this->parent->getCenter()) * rotationMatrix) + this->parent->getCenter();
	for (int i=0; i<this->points.size(); i++){
		this->points[i] = ((this->points[i] - this->parent->getCenter()) * rotationMatrix) + this->parent->getCenter();
		this->normals[i] = normalize(this->points[i] - this->center);
	}
	this->center0 = this->center;
}
