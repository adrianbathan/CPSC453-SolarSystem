// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// Modified by:
//			Adrian Bathan, University of Calgary (30011953)
// Modified on: April 13, 2018
// Citation (for all files):
//		SSBO (shader storage buffer object)
//			https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
//			http://www.geeks3d.com/20140704/tutorial-introduction-to-opengl-4-3-shader-storage-buffers-objects-ssbo-demo/
//		THIS PROGRAM USES OPENGL 4.3 #defines taken from
//			https://github.com/cloudwu/ejoy3d/blob/master/3rd/glad/include/glad/glad.h#L1826
//		Object Loader
//			http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
//		Textures From
//			https://www.solarsystemscope.com/textures/
//		Planet Stats
//			various sites from wikipedia
// ==========================================================================
// ==========================================================================
#version 430

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 UVPos;
layout(location = 2) in vec3 VertexNormal;

// output to be interpolated between vertices and passed to the fragment stage
out vec3 Points;
out vec2 UV;
out vec3 Normal;

uniform mat4 MVP;
uniform mat4 ROT;
uniform bool isModel;
uniform float windOffset;

void main()
{
	Normal = normalize(vec3(1.0f)*VertexNormal);
	Points = VertexPosition;
    gl_Position = MVP * vec4(VertexPosition, 1.0f);

    UV = UVPos;
}
