#include "myobjects.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//using namespace std;
//using namespace glm;

bool loadOBJ(
    const char * path,
    std::vector < std::vector < glm::vec3 > > & out_vertices,
    std::vector <std::vector < glm::vec2 > > & out_uvs,
    std::vector <std::vector < glm::vec3 > > & out_normals,
    std::vector < unsigned int > & texture_names
)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	
	std::vector< std::vector < unsigned int > > objectvIndices,objectuvIndices,objectnIndices;

	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return false;
	}
	int count = 0;
	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		else
			count++;
		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			temp_uvs.push_back(uv);
		
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		
		}
		else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
//			int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );			
			if (matches != 9){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
//				printf("%d/%d/%d %d/%d/%d %d/%d/%d\n", vertexIndex[0], uvIndex[0], normalIndex[0], vertexIndex[1], uvIndex[1], normalIndex[1], vertexIndex[2], uvIndex[2], normalIndex[2] );
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else if ( strcmp( lineHeader, "usemtl" ) == 0) {// && vertexIndices.size() > 0){
			if (vertexIndices.size()>0) {
				objectvIndices.push_back(vertexIndices);
				objectuvIndices.push_back(uvIndices);
				objectnIndices.push_back(normalIndices);
				vertexIndices.clear();
				uvIndices.clear();
				normalIndices.clear();
			}
			unsigned int texID;
			int matches = fscanf(file, "%d\n", &texID);
			texture_names.push_back(texID);
		}
	}
	if (vertexIndices.size()>0) {
		objectvIndices.push_back(vertexIndices);
		objectuvIndices.push_back(uvIndices);
		objectnIndices.push_back(normalIndices);
		vertexIndices.clear();
		uvIndices.clear();
		normalIndices.clear();
	}
	
	 // For each vertex of each triangle
/*	for( unsigned int i=0; i<vertexIndices.size(); i++ ){
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	}
	*/
	std::vector< glm::vec3 > tempv;
	std::vector< glm::vec2 > tempu;
	std::vector< glm::vec3 > tempn;
	for(unsigned int i=0; i<objectvIndices.size(); i++) {
		for(unsigned int j=0; j<objectvIndices[i].size(); j++) {
			unsigned int objvIndex = objectvIndices[i][j];
			unsigned int objuvIndex = objectuvIndices[i][j];
			unsigned int objnIndex = objectnIndices[i][j];
			glm::vec3 v = temp_vertices[ objvIndex-1 ];
			glm::vec2 u = temp_uvs[ objuvIndex-1 ];
			glm::vec3 n = temp_normals[ objnIndex-1 ];
			tempv.push_back(v);
			tempu.push_back(u);
			tempn.push_back(n);
//			std::cout << objvIndex << "/" << objuvIndex << "/" << objnIndex << std::endl;
		}
//		std::cout<<objectvIndices.size()<< ":" << objectuvIndices.size()<<std::endl;
		out_vertices.push_back(tempv);
		out_uvs.push_back(tempu);
		out_normals.push_back(tempn);
		tempv.clear();
		tempu.clear();
		tempn.clear();
	}
	fclose(file);
	return true;
}
