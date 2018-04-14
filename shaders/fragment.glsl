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

// interpolated colour received from vertex stage
in vec3 Normal;
in vec2 UV;
in vec3 Points;
// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

uniform sampler2D ourTexture;
uniform sampler2D earthNight;
uniform sampler2D earthSpec;
uniform sampler2D earthCloud;
uniform bool isDiffuse;
uniform bool isEarth;
uniform bool isMoon;
uniform float camx;
uniform float camy;
uniform float camz;
uniform float windOffset;

struct Sphere{
	vec3 p;
	float r;
};
layout(std430, binding = 2) buffer SphereName {Sphere sphereFaces[];};
float sphereGetT(vec3 o, vec3 d, Sphere s) {	//d should be normalized
	float a = dot(d, d);
	float b = 2.0 * (dot(o, d) - dot(s.p, d));
	float c = (-2.0*dot(o, s.p)) + dot(o, o) + dot(s.p, s.p) - (s.r * s.r);
	float underRoot = b*b - 4.0*a*c;
	if (underRoot < 0){
		return -1.0;
	}
	float t1 = (-b + sqrt(underRoot)) / 2.0*a;
	float t2 = (-b - sqrt(underRoot)) / 2.0*a;
	float val = min(t1,t2);
	return val;
}
void main(void)
{
    // write colour output without modification
	if(isDiffuse) {
		if (isEarth) {
			float p = 16.0f;
			vec3 lightCol = texture(earthNight,UV).xyz;
			float avgLight = (lightCol.x+lightCol.y+lightCol.z)/3.0f;
			if (avgLight > 0.2f) 
				lightCol *=3;
			vec3 specCol = texture(earthSpec,UV).xyz;
			vec3 cloudCol = texture(earthCloud,UV).xyz;
			float avgFrag;
			vec3 lightRay = normalize(vec3(0) - Points);
			vec3 viewRay = normalize(vec3(camx,camy,camz)- Points);
			vec3 hRay = normalize(viewRay + lightRay);
			vec4 shadows;
			FragmentColour=vec4(specCol,1.0f) * vec4(vec3(1.0f),1.0f) * pow(max(0.01f, dot(hRay, Normal)),p);
			FragmentColour+= 0.5f*texture(earthCloud, UV+vec2(windOffset, 0.0f));
			FragmentColour += texture(ourTexture,UV) * vec4(lightCol,1.0f) * max(0.1f, dot(-Normal, lightRay))
								+texture(ourTexture,UV)*  max(0.1f, dot(Normal, lightRay));
			shadows = vec4(1.0f) * vec4(1.0f) * max(0.1f, dot(-Normal, lightRay));
			vec3 amb = vec3(1.0f);
			float t;
			float maxt = sqrt(dot(vec3(0.0f)-Points, vec3(0.0f)-Points));
			float t0 = maxt;
			t = sphereGetT(Points, lightRay, sphereFaces[0]);
			if (t > 0.001f && t<=maxt && t < t0) 
				t0 = t;
			avgFrag = (shadows.xyz.x+shadows.xyz.y+shadows.xyz.z)/3.0f;
			if (t0 < maxt && avgFrag <= 0.10001f) 
				FragmentColour = vec4(FragmentColour.xyz*0.3f, 1.0f);		
		}
		else if (isMoon) {
			vec3 lightCol = vec3(1.0f);
			vec3 lightRay = normalize(vec3(0) - Points);
			vec4 shadows;
			float avgFrag;
			FragmentColour = texture(ourTexture,UV) * vec4(lightCol,1.0f) * max(0.1f, dot(Normal, lightRay));
			shadows = vec4(1.0f) * vec4(1.0f) * max(0.1f, dot(Normal, lightRay));
			vec3 amb = vec3(1.0f);
			float t;
			float maxt = sqrt(dot(vec3(0.0f)-Points, vec3(0.0f)-Points));
			float t0 = maxt;
			t = sphereGetT(Points, lightRay, sphereFaces[0]);
			if (t > 0.001f && t<=maxt && t < t0) 
				t0 = t;
			avgFrag = (shadows.xyz.x+shadows.xyz.y+shadows.xyz.z)/3.0f;
			if (t0 < maxt && avgFrag >= 0.100001f) 
				FragmentColour = vec4(FragmentColour.xyz*0.3f, 1.0f);		
		}
		else {
			vec3 lightCol = vec3(1.0f);
			vec3 lightRay = normalize(vec3(0) - Points);
			FragmentColour = texture(ourTexture,UV) * vec4(lightCol,1.0f) * max(0.1f, dot(Normal, lightRay));
		}
	}
	else
		FragmentColour = texture(ourTexture,UV);
}
