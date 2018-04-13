// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 430

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
//layout(location = 1) in vec3 VertexColour;
layout(location = 1) in vec2 UVPos;
layout(location = 2) in vec3 VertexNormal;

// output to be interpolated between vertices and passed to the fragment stage
//out vec3 Colour;
out vec3 Points;
out vec2 UV;
out vec3 Normal;
//out vec3 fNormal;

uniform mat4 MVP;
uniform mat4 ROT;
uniform bool isModel;
uniform float windOffset;

void main()
{
    // assign vertex position without modification
//    gl_Position = vec4(VertexPosition, 0.0, 1.0);
//	Normal = normalize(MVP*mat4(1.0f)*vec4(VertexNormal,0.0f)).xyz;
//	if(isModel) { 
		Normal = normalize(vec3(1.0f)*VertexNormal);
		Points = VertexPosition;
//	}
//	else {
//z	Normal = normalize((MVP*vec4(vec3(1.0f)*VertexNormal,0.0f)).xyz);
//	Points = (MVP*vec4(VertexPosition,0.0f)).xyz;
//	}
    gl_Position = MVP * vec4(VertexPosition, 1.0f);

    // assign output colour to be interpolated
//    Colour = VertexColour;
    UV = UVPos;
}
