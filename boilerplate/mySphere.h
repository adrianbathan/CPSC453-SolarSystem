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

#include "geometry.h"
#include "texture.h"
#include <vector>

using namespace glm;
using namespace std;

class mySphere {
	public:
		mySphere(const char * name, float d, const float &r) : sphereName(name), offset(d), radius(r) {}
		mySphere(const char * name, mySphere *p, float d, const float r) : sphereName(name), parent(p), offset(d), radius(r) {}
		void setParent(mySphere *p) {parent = p;}
		const char * sphereName;
		float getOffset() {return offset;}
		vec3 getCenter() {return center;}
		vec3 getCenter0() {return center0;}
		vec3 getAxis() {return axis;}
		vec3 getIncline() {return incline;}
		float getRadius() {return radius;}
		vector<vec3> getPoints() {return points;}
		vector<vec2> getUVs() {return uvs;}
		vector<vec3> getNormals() {return normals;}
		vector<unsigned int> getIndices() {return indices;}
		void generateSphere(char * filename);
		void rotate0(float angle);
		void rotate();
		void orbit();
		mySphere *parent;
		MyTexture sphereTexture;
		Geometry sphereGeometry;
		void setParameters(float rot, float orb, float ang, float inc, int i) {
			rotation = (2.0f*M_PI/rot);//speed; 
			solorbit = (2.0f*M_PI/orb);//speed;
			axis = vec3(-sin(ang), 0.0f, cos(ang));
			angle = ang;
			incline = vec3(-sin(inc), 0.0f, cos(inc));
			sphereIndex = i;
			}
		void setSpeed(float fps)  {speed = fps;}
		void setAngles(float fps) {rotAngle += rotation/speed; orbAngle += solorbit/speed;}
		float getRotAngle() {return rotAngle;}
		float getOrbAngle() {return orbAngle;}
		void resetCenter() {center = center0;}//vec3(0.0f);}
		void setCenter(vec3 c) {center0 = c;}//vec3(0.0f);}
		float getSpeed()  {return speed;}
		float getRotation()  {return rotation;}
		
	private:
		vector<vec3> points;
		vector<vec2> uvs;
		vector<vec3> normals;
		vector<unsigned int> indices;
		vec3 center = vec3(0.0f);
		vec3 center0;// = vec3(0.0f);
		vec3 axis;
		float angle;
		vec3 incline;
		float orbAngle=0;
		float rotAngle=0;
		float offset;
		float radius;
		float rotation;
		float solorbit;
		float speed = 60.0f;
		int sphereIndex;
};
