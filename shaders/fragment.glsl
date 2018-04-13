// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 430

// interpolated colour received from vertex stage
//in vec3 Colour;
in vec3 Normal;
in vec2 UV;
//in vec4 galaxy;
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
//	vec3 p1;
//	float pad1;
//	vec3 p2;
//	float pad2;
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
/*	if (s.kd > 0) {
		if (abs(t1) < abs(t2))
			return t1;
		else
			return t2;
		}*/
//	if (val < 0.0){
//		val = max(t1,t2);
//	}
	return val;
}
/*
float triangleGetT(vec3 o, vec3 d, Triangle s) {
	vec3 d0 = o - s.p0;
	vec3 e1 = s.p1 - s.p0;
	vec3 e2 = s.p2 - s.p0;

	float tNumer = determinant(mat3(d0,e1,e2));
	float uNumer = determinant(mat3(-d,d0,e2));
	float vNumer = determinant(mat3(-d,e1,d0));
	float denom = 1.0/determinant(mat3(-d,e1,e2));

	float t = tNumer * denom;
	float u = uNumer * denom;
	float v = vNumer * denom;

	if ((u+v) < 1.0 && (u+v) > 0.0 && u > 0.0 && u < 1.0 && v > 0.0 && v < 1.0){
		return t;
	}
	else {
		return -1.0;
	}
}*/
void main(void)
{
    // write colour output without modification
//    FragmentColour = vec4(Colour, 0);
	if(isDiffuse) {
		if (isEarth) {
			float p = 16.0f;
			vec3 lightCol = texture(earthNight,UV).xyz;
			float avgLight = (lightCol.x+lightCol.y+lightCol.z)/3.0f;
			if (avgLight > 0.2f) 
				lightCol *=3;
			vec3 specCol = texture(earthSpec,UV).xyz;//vec3(1.0f);
			vec3 cloudCol = texture(earthCloud,UV).xyz;
			float avgFrag;// = (specCol.x+specCol.y+specCol.z)/3.0f;
			vec3 lightRay = normalize(vec3(0) - Points);
			vec3 viewRay = normalize(vec3(camx,camy,camz)- Points);
			vec3 hRay = normalize(viewRay + lightRay);
			vec4 shadows;
			FragmentColour=vec4(specCol,1.0f) * vec4(vec3(1.0f),1.0f) * pow(max(0.01f, dot(hRay, Normal)),p);
			FragmentColour+= 0.5f*texture(earthCloud, UV+vec2(windOffset, 0.0f));//* vec4(lightCol,1.0f) * max(0.1f, dot(-Normal, lightRay));	
			FragmentColour += texture(ourTexture,UV) * vec4(lightCol,1.0f) * max(0.1f, dot(-Normal, lightRay))
								+texture(ourTexture,UV)*  max(0.1f, dot(Normal, lightRay));
			shadows = vec4(1.0f) * vec4(1.0f) * max(0.1f, dot(-Normal, lightRay));
//			FragmentColour = shadows;
			vec3 amb = vec3(1.0f);
			float t;
			float maxt = sqrt(dot(vec3(0.0f)-Points, vec3(0.0f)-Points));
			float t0 = maxt;//100000.0f;
//			for (int i=0; i<sphereFaces.length(); i++) {
//				if (i != 1)
//			if (abs(distance(Points, vec3(0.0f))) >= abs(distance(sphereFaces[1].p, vec3(0.0f))))
//				t0 = 1000000000.0f;
//			else {
				t = sphereGetT(Points, lightRay, sphereFaces[0]);
				if (t > 0.001f && t<=maxt && t < t0) 
					t0 = t;
//			}
//			if (distance(Points, vec3(0.0f)) < distance(sphereFaces[0].p, vec3(0.0f)))
//				t0 = 1000000000.0f;
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
			float t0 = maxt;//100000.0f;
//			for (int i=0; i<sphereFaces.length(); i++) {
//				if (i != 0)
//			if (abs(distance(Points, vec3(0.0f))) >= abs(distance(sphereFaces[1].p, vec3(0.0f))))
//				t0 = 1000000000.0f;
//			else {
				t = sphereGetT(Points, lightRay, sphereFaces[0]);
				if (t > 0.001f && t<=maxt && t < t0) 
					t0 = t;
//			}
//			if (distance(Points, vec3(0.0f)) > distance(sphereFaces[1].p, vec3(0.0f)))
//				t0 = 1000000000.0f;
			avgFrag = (shadows.xyz.x+shadows.xyz.y+shadows.xyz.z)/3.0f;
			if (t0 < maxt && avgFrag >= 0.100001f) 
				FragmentColour = vec4(FragmentColour.xyz*0.3f, 1.0f);		
		}
		else {
			vec3 lightCol = vec3(1.0f);
			vec3 lightRay = normalize(vec3(0) - Points);
			FragmentColour = texture(ourTexture,UV) * vec4(lightCol,1.0f) * max(0.1f, dot(Normal, lightRay));
//			FragmentColour = vec4(FragmentColour.xyz,0.4f);
		}
	}
	else
		FragmentColour = texture(ourTexture,UV);//+vec4(1.0f,1.0f,1.0f,1.0f);
}
