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
// Date:    December 2015
// ==========================================================================

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

#include "texture.h"
#include "geometry.h"
#include "myobjects.h"
#include "mySphere.h"
//#include "GlyphExtractor.h"
//#include "imagebuffer.h"
#define GL_SHADER_STORAGE_BUFFER 0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING 0x90D3
#define GL_SHADER_STORAGE_BUFFER_START 0x90D4
#define GL_SHADER_STORAGE_BUFFER_SIZE 0x90D5
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS 0x90D7
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS 0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS 0x90D9
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE 0x90DE
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT 0x90DF
using namespace std;
using namespace glm;
// --------------------------------------------------------------------------
// OpenGL utility and support function prototypes

void QueryGLVersion();
bool CheckGLErrors();

string LoadSource(const string &filename);
GLuint CompileShader(GLenum shaderType, const string &source);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

// --------------------------------------------------------------------------
// Functions to set up OpenGL shader programs for rendering

// load, compile, and link shaders, returning true if successful
GLuint InitializeShaders()
{
	// load shader source from files
	string vertexSource = LoadSource("shaders/vertex.glsl");
	string fragmentSource = LoadSource("shaders/fragment.glsl");
	if (vertexSource.empty() || fragmentSource.empty()) return false;

	// compile shader source into shader objects
	GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	// link shader program
	GLuint program = LinkProgram(vertex, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// check for OpenGL errors and return false if error occurred
	return program;
}

bool goforward = false, backward = false, strafeLeft = false, strafeRight = false;
bool flyUp = false, flyDown = false;

double dx=0, dy=0, dz=-2, thx=0, thy=0;
float fov = 90.0f;
double xclick = 0, yclick = 0, oldx=0, oldy=0;
bool lclickdown = false;
vec2 mousePos = vec2(0, 0);
//vec2 newMousePos;
vec3 position = vec3(0,100,0);
vec3 up = vec3(0,0,-1);
bool pause = false;
bool resetPos = false;
vector<mySphere*> stars;
vector<mySphere*> spheres;
vector<mySphere*> spheresUnordered;
vector<Geometry> sphgeos;

MyTexture earthnightTex;
MyTexture earthspecTex;
MyTexture earthcloudTex;

vec3 camPos = vec3(0.0f, 0.0f, 0.0f);
vec3 camLookAt = vec3(0.0f);
vec3 camDir = vec3(0.0f, 0.0f, -1.0f);
vec3 camUp = vec3(0.0f, 1.0f, 0.0f);
vec3 camRight = vec3(1.0f, 0.0f, 0.0f);
vec3 camSpherePos = vec3(M_PI/2, M_PI/2, 100.0f);
vec3 camTarget = -vec3(0.0f);
float camDistance = 8.8f;

int mode = 0, submode = 0;
float fps = 60.0f;
//vec3 direction;
//vec3 right;
/*vec3 direction(
			cos(thy) * sin(thx), 
			sin(thy),			
			cos(thy) * cos(thx)
		);
vec3 right = vec3(sign(thx-M_PI/2.0f),0,cos(thx-M_PI/2.0f));*/
// --------------------------------------------------------------------------
// Rendering function that draws our scene to the frame buffer

void RenderScene(Geometry *geometry, GLuint program, MyTexture *tex, bool clr, int elem)
{
	// clear screen to a dark grey colour
	if (clr) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	glBindTexture(GL_TEXTURE_2D, tex->textureID);
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(program);
	glBindVertexArray(geometry->vertexArray);
	if (elem != -1) {
	glDrawElements(
			GL_TRIANGLES,		//What shape we're drawing	- GL_TRIANGLES, GL_LINES, GL_POINTS, GL_QUADS, GL_TRIANGLE_STRIP
			elem,		//How many indices
			GL_UNSIGNED_INT,	//Type
			(void*)0			//Offset
			);
		}
	else
		glDrawArrays(GL_TRIANGLES, 0, geometry->elementCount);

	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}
void cameraController(float dp, float dt, float dr) {
	camSpherePos += vec3(dp, dt, dr);
	float theta = camSpherePos.y;
	float phi = camSpherePos.x;
/*	if (camSpherePos.z < ceil(camDistance)){
		camSpherePos.z = ceil(camDistance);
	}
	else if (camSpherePos.z > 75){
		camSpherePos.z = 75;
	}*/
//	cout << theta << " , " << phi << endl;
	if (phi >M_PI-0.001) phi = M_PI-0.001;
	if (phi <0.001) phi = 0.001f;
	camPos = camSpherePos.z * vec3(cos(theta) * sin(phi),
						sin(theta) * sin(phi),
						cos(phi)) + camTarget;
	camDir = normalize(camPos - camTarget);
	camRight = normalize(cross(camDir, vec3(0, 0, 1)));
	camUp =  normalize(cross(camRight, camDir));
}
// --------------------------------------------------------------------------
// GLFW callback functions

// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
	cout << "GLFW ERROR " << error << ":" << endl;
	cout << description << endl;
}

// handles keyboard input events
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
	
//	else {
		if (key == GLFW_KEY_1 && action == GLFW_PRESS)  {
			mode = 0;
			submode = 0;
			camSpherePos = vec3(M_PI/2, M_PI/2, spheresUnordered[mode]->getRadius()*5);
			cameraController(0.0f, 0.0f, spheresUnordered[mode]->getRadius()*5);
		}
		if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
			mode = 1;	
			submode = 0;
			camSpherePos = vec3(M_PI/2, M_PI/2, spheresUnordered[mode]->getRadius()*2);
			cameraController(0.0f, 0.0f, spheresUnordered[mode]->getRadius()*2);
		}
		if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
			mode = 2;	
			submode = 0;
			camSpherePos = vec3(M_PI/2, M_PI/2, spheresUnordered[mode]->getRadius()*2);
			cameraController(0.0f, 0.0f, spheresUnordered[mode]->getRadius()*2);
		}
		if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
			mode = 3;	
			submode = 0;
			camSpherePos = vec3(M_PI/2, M_PI/2, spheresUnordered[mode]->getRadius()*2);
			cameraController(0.0f, 0.0f, spheresUnordered[mode]->getRadius()*2);
		}
		if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
			mode = 4;	
			submode = 0;
			camSpherePos = vec3(M_PI/2, M_PI/2, spheresUnordered[mode]->getRadius()*2);
			cameraController(0.0f, 0.0f, spheresUnordered[mode]->getRadius()*2);
		}
		if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
			mode = 5;	
			submode = 0;
			camSpherePos = vec3(M_PI/2, M_PI/2, spheresUnordered[mode]->getRadius()*2);
			cameraController(0.0f, 0.0f, spheresUnordered[mode]->getRadius()*2);
		}
		if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
			mode = 6;	
			submode = 0;
			camSpherePos = vec3(M_PI/2, M_PI/2, spheresUnordered[mode]->getRadius()*2);
			cameraController(0.0f, 0.0f, spheresUnordered[mode]->getRadius()*2);
		}
		if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
			mode = 7;	
			submode = 0;
			camSpherePos = vec3(M_PI/2, M_PI/2, spheresUnordered[mode]->getRadius()*2);
			cameraController(0.0f, 0.0f, spheresUnordered[mode]->getRadius()*2);
		}
		if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
			mode = 8;	
			submode = 0;
			camSpherePos = vec3(M_PI/2, M_PI/2, spheresUnordered[mode]->getRadius()*3);
			cameraController(0.0f, 0.0f, spheresUnordered[mode]->getRadius()*3);
		}
		if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
			mode = 9;	
			submode = 0;
			camSpherePos = vec3(M_PI/2, M_PI/2, spheresUnordered[mode]->getRadius()*3);
			cameraController(0.0f, 0.0f, spheresUnordered[mode]->getRadius()*3);
		}
		if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
			mode = 10;	
			submode = 0;
			camSpherePos = vec3(M_PI/2, M_PI/2, spheresUnordered[mode]->getRadius()*5);
			cameraController(0.0f, 0.0f, spheresUnordered[mode]->getRadius()*5);
		}
	if (key == GLFW_KEY_SPACE)  {
		if (action == GLFW_PRESS) {
			if (pause)
				pause = false;
			else
				pause = true;
		}	
	}
	if (key == GLFW_KEY_LEFT) {// && action == GLFW_PRESS)
		if (action == GLFW_PRESS) {
			if (submode == 0)
				submode = 11;
			else
				submode--;
		}
	}
	if (key == GLFW_KEY_RIGHT) {// && action == GLFW_PRESS)
		if (action == GLFW_PRESS) {
			if (submode == 11)
				submode = 0;
			else
				submode++;
		}
	}
	if (key == GLFW_KEY_UP) {// && action == GLFW_PRESS)
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
//			fps = fps / 1.5f;
//			if (fps <= 0.1f)
//				fps = 0.1f;
			if (fps <= 5.0f)
				fps =  5.0f;//fps - 0.1f;
			else if (fps <= 10.0f)
				fps = fps - 1.0f;
			else
				fps = fps - 10.0f;
		}
	}
	if (key == GLFW_KEY_DOWN) {// && action == GLFW_PRESS) 
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
//			fps = fps * 1.5f;
//			if (fps <= 1.0f)
//				fps = fps + 0.1f;
			if (fps < 10.0f)
				fps = fps + 1.0f;
			else
				fps = fps + 10.0f;
		}
	}
	if (key == GLFW_KEY_R || key == GLFW_KEY_N)  {
		if (action == GLFW_PRESS) {
			camPos = vec3(0.0f, 0.0f, 0.0f);
			camLookAt = vec3(0.0f);
			camDir = vec3(0.0f, 0.0f, -1.0f);
			camUp = vec3(0.0f, 1.0f, 0.0f);
			camRight = vec3(1.0f, 0.0f, 0.0f);
			camSpherePos = vec3(M_PI/2, M_PI/2, 100.0f);
			camTarget = -vec3(0.0f);
			camDistance = 8.8f;

			mode = 0;
			fps = 60.0f;
			if(key == GLFW_KEY_N) 
				resetPos = true;

		}	
	}
//	cout << fps << endl;
//cout << mode << endl;
	//	camTarget = spheresUnordered[mode]->getCenter();
//		camSpherePos = vec3(M_PI/2, M_PI/2, spheresUnordered[mode]->getRadius()*5);

//		cameraController(0.0f, 0.0f, spheresUnordered[mode]->getRadius()*5);
//	}

}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	cout.precision(8);
	
//	double x_raw, y_raw, x_ogl, y_ogl;//, dx0,dy0;
	vec2 newPos = vec2(xpos/1024, -ypos/1024)*2.f - vec2(1.f);
		vec2 diff = newPos - mousePos;
		float dt = diff.x;
		float dp = diff.y;
		
	if (lclickdown) {	
		vec3 oldCamDir = camDir;
		cameraController(-dp, dt, 0.0f);
//		if (camDir.x*oldCamDir.x < 0) {
//			cameraController(-dp, -dt, -2*camSpherePos.z);
//
//			camPos = -camPos;//vec3(-camPos.x, -camPos.y, camPos.z);
//			cameraController(0, 0, 0.0f);
//		}
//		cout << camDir.x << "," << oldCamDir.x << "," << camPos.z << "," << endl;
		
	}	
	mousePos = newPos;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
//	double x, y;
//	glfwGetCursorPos(window, &x, &y);
//	xclick = (float)x;
//	yclick = (float)y;
//	xclick = (-1024+x)/1024;
//	yclick = (1024-y)/1024;
//	glfwGetCursorPos(window, &xclick, &yclick);
	
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{	
		lclickdown = true;
//		mousePos=vec2(x,y);
//		dx = 0;
//		dy = 0;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		lclickdown = false;
//		oldx=dx;//thx;
//		oldy=dy;//thy;

	}
	
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameraController(0.0f, 0.0f, -yoffset);
}

char* pics[36] = {	"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/new_body.png",
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/mayu01.tga",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_ribbon.png",
					
					"shimakaze/on_SHIMAKAZE_v090/eye_LA.png",
					"shimakaze/on_SHIMAKAZE_v090/eye_LB.png",
					"shimakaze/on_SHIMAKAZE_v090/eye_LC.png",
					"shimakaze/on_SHIMAKAZE_v090/eye_RA.png",
					"shimakaze/on_SHIMAKAZE_v090/eye_RB.png",
					"shimakaze/on_SHIMAKAZE_v090/eye_RC.png",
					
					"shimakaze/on_SHIMAKAZE_v090/hair_B.tga",
					"shimakaze/on_SHIMAKAZE_v090/hair_F.tga",
					"shimakaze/on_SHIMAKAZE_v090/hair_F.tga",
					"shimakaze/on_SHIMAKAZE_v090/hair_F.tga",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_top_skt.png",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_eri.png",
					
					"shimakaze/on_SHIMAKAZE_v090/simakaze_top_skt.png",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_top_skt.png",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_glove.png",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_sox.png",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_ribbon.png",
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/eyeparts.png",
					"shimakaze/on_SHIMAKAZE_v090/0siki_5ren.png",
					
					"shimakaze/on_SHIMAKAZE_v090/0siki_5ren_cover.png",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_parts.png",
					"shimakaze/on_SHIMAKAZE_v090/hair_S.tga",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_boots.png",
					"shimakaze/on_SHIMAKAZE_v090/hair_B.tga",
					"shimakaze/on_SHIMAKAZE_v090/hair_S.tga" };
					

// ==========================================================================
// PROGRAM ENTRY POINT

bool sortObjects(mySphere* s0, mySphere* s1) {return distance(s0->getCenter(),position)>distance(s1->getCenter(),position);}
bool parseTXT(const char * path) {
	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return false;
	}

//	mySphere *galaxy = new mySphere("galaxy", vec3(0.0f),5000.0f);
	mySphere *galaxy = new mySphere("galaxy", 0.0f,5000.0f);
	galaxy->setParameters(0.0f, 0.0f, radians(0.0f), radians(0.0f), -1);
	galaxy->generateSphere("2k_stars_milky_way.jpg");
	stars.push_back(galaxy);
//	mySphere *sun = new mySphere("sun", galaxy, vec3(0.0f),8.8f);
	mySphere *sun = new mySphere("sun", galaxy, 0.0f,8.8f);
	sun->setParameters(26.24f, 26.24f, radians(7.25f), radians(0.0f), 0);
	sun->generateSphere("2k_sun.jpg");
	spheres.push_back(sun);
	spheresUnordered.push_back(sun);	
int count = 0;
//	while (getline( myfile, line )) {
	while( 1 ){

		char objectName[64];
		float parseX, parseR, parseRot, parseOrb, parseAng, parseInc;
		char textureFile[64];
		int parentIndex, planetIndex;
		// read the first word of the line
		int res = fscanf(file, "%s", objectName);
		
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		int res0 = fscanf(file, "%f %f %f %f %f %f %d %d %s\n",
//		int res = fscanf(line, "%s %f %f %f %f %f %f %s %d\n", objectName,
						&parseX, &parseR, &parseRot, &parseOrb, &parseAng, &parseInc,
						&parentIndex, &planetIndex, textureFile);
		cout << objectName<< "  ,  " << textureFile<<endl;
		char * planetName;
		if(planetIndex == 1)
			planetName = "mercury";
		if(planetIndex == 2)
			planetName = "venus";
		if(planetIndex == 3)
			planetName = "earth";
		if(planetIndex == 4)
			planetName = "moon";
		if(planetIndex == 5)
			planetName = "mars";
		if(planetIndex == 6)
			planetName = "jupiter";
		if(planetIndex == 7)
			planetName = "saturn";
		if(planetIndex == 8)
			planetName = "uranus";
		if(planetIndex == 9)
			planetName = "neptune";
		if(planetIndex == 10)
			planetName = "pluto";
		
//		mySphere *s = new mySphere(objectName, spheres[parentIndex], spheres[parentIndex]->getCenter() + vec3(parseX, 0.0f, 0.0f), parseR);
		mySphere *s = new mySphere(planetName, spheres[parentIndex], parseX, parseR);
//		mySphere *s = new mySphere(objectName, spheres[parentIndex], vec3(parseX, 0.0f, 0.0f), parseR);
		s->setParameters(parseRot, parseOrb, radians(parseAng), radians(parseInc), planetIndex);
		s->generateSphere(textureFile);
//		cout << s->sphereName << endl;
		spheres.push_back(s);
		spheresUnordered.push_back(s);	
		count++;
//		if(!LoadVBO(&s->sphereGeometry, s->getPoints(), s->getUVs(), s->getNormals(), s->getIndices()))
//			cout << "Failed to load vbo" << endl;
			
	}
	
	fclose(file);
	return true;
}
struct Sphere{
	vec3 p;
	float r;
};
int main(int argc, char *argv[])
{
	// initialize the GLFW windowing system
	if (!glfwInit()) {
		cout << "ERROR: GLFW failed to initialize, TERMINATING" << endl;
		return -1;
	}
	glfwSetErrorCallback(ErrorCallback);

	// attempt to create a window with an OpenGL 4.1 core profile context
	GLFWwindow *window = 0;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 1024, height = 1024;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}

	// set keyboard callback function and make our context current (active)
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwMakeContextCurrent(window);

	//Intialize GLAD
	if (!gladLoadGL())
	{
		cout << "GLAD init failed" << endl;
		return -1;
	}

	// query and print out information about our OpenGL environment
	QueryGLVersion();

	// call function to load and compile shader programs
	GLuint program = InitializeShaders();
	if (program == 0) {
		cout << "Program could not initialize shaders, TERMINATING" << endl;
		return -1;
	}

	// three vertex positions and assocated colours of a triangle
/*	vec3 vertices[] = {
		vec3( -1.0f, -1.0f, .0f ),
		vec3( 1.0f,  1.0f, .0f ),
		vec3( 1.0f, -1.0f, .0f )
	};

	vec3 colours[] = {
		vec3( 1.0f, 0.0f, 0.0f ),
		vec3( 0.0f, 1.0f, 0.0f ),
		vec3( 0.0f, 0.0f, 1.0f )
	};
	vec2 textures[] = {
		vec2( .0f, .0f ),
		vec2( 1.0f,  1.0f ),
		vec2( 1.0f, .0f )
	};
	
	*/
			
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
//	glEnable(GL_CULL_FACE);
	
	// Read our .obj file
	vector< vector< vec3 > > vertices0;
	vector< vector< vec2 > > uvs;
	vector< vector< vec3 > > normals; // Won't be used at the moment.
	vector< unsigned int > tID;
		
	bool res = loadOBJ("shimakaze/shimakaze.obj", vertices0, uvs, normals, tID);
	
	Geometry shima[vertices0.size()];
	MyTexture kaze[vertices0.size()];
	
	for(int i=0; i<vertices0.size(); i++) {
		vec3 v[vertices0[i].size()];
		vec3 col[vertices0[i].size()];
		vec2 uv[uvs[i].size()];
		vec3 ns[normals[i].size()];
		
		for(int j=0; j<vertices0[i].size(); j++) {
			v[j] = 0.5f*vertices0[i][j]+vec3(0.0f,6.378f,0.0f);;
			col[j]=vec3(1.0f, 0.0f, 0.0f);
		}
		for (int j=0; j<uvs[i].size(); j++)
			uv[j]=uvs[i][j];
		for (int j=0; j<normals[i].size(); j++) {
			ns[j]=normals[i][j];
		}
		if (!InitializeVAO(&shima[i]))
			cout << "Program failed to intialize geometry!" << endl;
		if(!LoadGeometry(&shima[i], v, uv, ns, vertices0[i].size()))
			cout << "Failed to load geometry" << endl;
		if (!InitializeTexture(&kaze[i], pics[tID[i]], GL_TEXTURE_2D))
			cout << "Program failed to initialize texture" << endl;
//		cout << vertices0[i].size() << endl;
	}

	if (!parseTXT("solarsystem.txt"))
		cout << "failed to parse txt file" << endl;

	for(int i=0; i<spheres.size(); i++) {
		if(!LoadVBO(&spheres[i]->sphereGeometry, spheres[i]->getPoints(), spheres[i]->getUVs(), spheres[i]->getNormals(), spheres[i]->getIndices()))//, 14400, 14400, 14400))
			cout << "Failed to load vao" << endl;
	}
	cameraController(0.0f, 0.0f, 0.0f);
		// run an event-triggered main loop
		
	if (!InitializeTexture(&earthnightTex, "2k_earth_nightmap.jpg", GL_TEXTURE_2D))
		cout << "Program failed to initialize texture" << endl;
	if (!InitializeTexture(&earthspecTex, "2k_earth_specular_map.jpg", GL_TEXTURE_2D))
		cout << "Program failed to initialize texture" << endl;
	if (!InitializeTexture(&earthcloudTex, "2k_earth_clouds.jpg", GL_TEXTURE_2D))
		cout << "Program failed to initialize texture" << endl;
glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(program, "earthNight"), 1);
	glUniform1i(glGetUniformLocation(program, "earthSpec"), 2);
	glUniform1i(glGetUniformLocation(program, "earthCloud"), 3);
	glActiveTexture(GL_TEXTURE0 + 0);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, earthnightTex.textureID);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, earthspecTex.textureID);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, earthcloudTex.textureID);
	glActiveTexture(GL_TEXTURE0 + 0);
	
glUseProgram(0);
	
	float windspeed = 0.0f;
	float modelAngle = 0;
	glm::mat4 trans, trans0, trans1, trans2, trans3, trans4;
	mat4 identity = mat4(0.0f);
	identity[0][0] = 1.0f;
	identity[1][1] = 1.0f;
	identity[2][2] = 1.0f;
	identity[3][3] = 1.0f;
	LoadVBO(&stars[0]->sphereGeometry, stars[0]->getPoints(), stars[0]->getUVs(), stars[0]->getNormals(), stars[0]->getIndices());
	while (!glfwWindowShouldClose(window))
	{
/*		if (resetPos){
			for (mySphere *s : spheres) {
				s->resetSpherePos();
				resetPos =false;
				cameraController(0.0f, 0.0f, 0.0f);
			}
		}*/
//		vec3 moonearth = spheresUnordered[0]->getCenter()-spheresUnordered[2]->getCenter();
//		float dmoonearth = distance(spheresUnordered[0]->getCenter(),spheresUnordered[2]->getCenter());
//		cout << "----------" << endl;
//		cout << moonearth0.x << "," << moonearth0.y << ","<< moonearth0.z << "         "<< dmoonearth0 <<endl;
		if (!pause) {
			windspeed = windspeed + 1.0f/2048.0f;
			if (windspeed >= 2048.0f)
				windspeed = 0;

			modelAngle += (2.0f * M_PI / 1.0f)/fps;
			sort(spheres.begin(), spheres.end(), sortObjects);
			for(mySphere *s : spheres) {
				s->setSpeed(fps);
//				s->setAngles(fps);
				
//				s->setCenter(vec3(s->parent->getOffset(),0.0f,0.0f) + vec3(s->getOffset(), 0.0f, 0.0f));
				s->orbit();
				s->rotate();
				
			}

		}
	
		camTarget = spheresUnordered[mode]->getCenter0();
		camSpherePos = vec3(camSpherePos.x, camSpherePos.y, camSpherePos.z);
	//	camDistance = spheresUnordered[mode]->getRadius();
//		cameraController(camSpherePos.x, camSpherePos.y, camSpherePos.z);
		//cameraController(0.0f, 0.0f, spheresUnordered[mode]->getRadius()*5);
		cameraController(0.0f, 0.0f, 0.0f);
	
		
		mat4 ProjectionMatrix = perspective(radians(fov), 3.0f / 3.0f, 0.1f, 10000.0f);
		mat4 ViewMatrix       = lookAt(
									camPos, //position,           // Camera is here
									camTarget,//vec3(0.0f),//position+direction, // and looks here : at the same position, plus "direction"
//									cross(right,direction));
									-camUp);//up);//vec3(.0f,.0f,-1.0f));             // Head is up (set to 0,-1,0 to look upside-down)
		mat4 ModelMatrix = mat4(1.0);
		mat4 MVP = ProjectionMatrix * ViewMatrix;// * ModelMatrix;
		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUseProgram(0);
	
		
		RenderScene(&stars[0]->sphereGeometry, program, &stars[0]->sphereTexture, true, stars[0]->getIndices().size());
//		RenderScene(&stars[0]->sphereGeometry, program, &spheresUnordered[7]->sphereTexture, true, stars[0]->getIndices().size());


		
		for(int i=0; i<spheres.size(); i++) {
			LoadVBO(&spheres[i]->sphereGeometry, spheres[i]->getPoints(), spheres[i]->getUVs(), spheres[i]->getNormals(), spheres[i]->getIndices());
/*			trans0 = rotate(identity, spheresUnordered[i]->getRotAngle(), vec3(0.0f,0.0f,1.0f));
			trans1 = translate(identity, spheresUnordered[i]->getCenter0());
			trans2 = rotate(identity, spheresUnordered[i]->getOrbAngle(), vec3(0.0f,0.0f,1.0f));
			trans3 = translate(identity, spheresUnordered[i]->parent->getCenter0());
			trans4 = rotate(identity, -spheresUnordered[i]->getOrbAngle(), vec3(0.0f,0.0f,1.0f));
		switch(submode) {
			case 0: trans = MVP*trans0*trans1*trans2*trans3*trans4; break;
			case 1: trans = MVP*trans0*trans1*trans2*trans3*trans4; break;
			case 2: trans = MVP*trans2*trans1*trans4*trans3*trans0; break;
			case 3: trans = MVP*trans2*trans1*trans4*trans3*trans0; break;
			case 4: trans = MVP*trans4*trans1*trans0*trans3*trans2; break;
			case 5: trans = MVP*trans4*trans1*trans0*trans3*trans2; break;
			case 6: trans = MVP*trans0*trans3*trans2*trans1*trans4; break;
			case 7: trans = MVP*trans0*trans3*trans2*trans1*trans4; break;
			case 8: trans = MVP*trans2*trans3*trans4*trans1*trans0; break;
			case 9: trans = MVP*trans2*trans3*trans4*trans1*trans0; break;
			case 10: trans = MVP*trans4*trans3*trans0*trans1*trans2; break;
			case 11: trans = MVP*trans4*trans3*trans0*trans1*trans2; break;
		}*/
/*			case 12: trans = trans2*trans0*trans1*trans3; break;
			case 13: trans = trans2*trans0*trans3*trans1; break;
			case 14: trans = trans2*trans1*trans0*trans3; break;
			case 15: trans = trans2*trans1*trans3*trans0; break;
			case 16: trans = trans2*trans3*trans0*trans1; break;
			case 17: trans = trans2*trans3*trans1*trans0; break;
			case 18: trans = trans3*trans0*trans1*trans2; break;
			case 19: trans = trans3*trans0*trans2*trans1; break;
			case 20: trans = trans3*trans1*trans0*trans2; break;
			case 21: trans = trans3*trans1*trans2*trans0; break;
			case 22: trans = trans3*trans2*trans0*trans1; break;
			case 23: trans = trans3*trans2*trans1*trans0; break;
		}
		trans = MVP*trans0*trans1*trans2*trans3;
*/
//		trans = MVP*trans0*trans1*trans2*trans3*trans4;

//		spheres[i]->setCenter(vec3(trans[3][0],trans[3][1],trans[3][2]));
			glUseProgram(program);
//			glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, &trans[0][0]);
//			glUniform1i(glGetUniformLocation(program, "isModel"), 0);//spheres[i]->getCenter() != vec3(0.0f));
			glUniform1i(glGetUniformLocation(program, "isDiffuse"), strcmp(spheres[i]->sphereName,"sun"));//spheres[i]->getCenter() != vec3(0.0f));
			glUniform1i(glGetUniformLocation(program, "isEarth"), !strcmp(spheres[i]->sphereName,"earth"));// != vec3(0.0f));
			glUniform1i(glGetUniformLocation(program, "isMoon"), !strcmp(spheres[i]->sphereName,"moon"));// != vec3(0.0f));
			glUniform1f(glGetUniformLocation(program, "camx"), camPos.x);// != vec3(0.0f));
			glUniform1f(glGetUniformLocation(program, "camy"), camPos.y);// != vec3(0.0f));
			glUniform1f(glGetUniformLocation(program, "camz"), camPos.z);// != vec3(0.0f));
			glUniform1f(glGetUniformLocation(program, "windOffset"), windspeed);// != vec3(0.0f));
			if (!strcmp(spheres[i]->sphereName,"earth")) {
				Sphere sphFaces[2];
				sphFaces[0].p = spheresUnordered[4]->getCenter();
				sphFaces[0].r = spheresUnordered[4]->getRadius();
				sphFaces[1].p = spheresUnordered[3]->getCenter();
				sphFaces[1].r = spheresUnordered[3]->getRadius();
				GLuint ssbo;
				glGenBuffers(1, &ssbo);	
				glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssbo);
				glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Sphere), sphFaces, GL_DYNAMIC_COPY);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER,2,ssbo);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			}
			else if (!strcmp(spheres[i]->sphereName,"moon")) {
				Sphere sphFaces[2];
				sphFaces[0].p = spheresUnordered[3]->getCenter();
				sphFaces[0].r = spheresUnordered[3]->getRadius();
				sphFaces[1].p = spheresUnordered[4]->getCenter();
				sphFaces[1].r = spheresUnordered[4]->getRadius();
				GLuint ssbo;
				glGenBuffers(1, &ssbo);	
				glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssbo);
				glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Sphere), sphFaces, GL_DYNAMIC_COPY);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER,2,ssbo);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
				
			}
			glUseProgram(0);
//			if (i==0)
//			RenderScene(&spheres[i]->sphereGeometry, program, &spheres[i]->sphereTexture, true, spheres[i]->getIndices().size());
//			else
			RenderScene(&spheres[i]->sphereGeometry, program, &spheres[i]->sphereTexture, false, spheres[i]->getIndices().size());
//			RenderScene(&spheresUnordered[i]->sphereGeometry, program, &spheresUnordered[i]->sphereTexture, false, spheresUnordered[i]->getIndices().size());
		}
		
		
//		LoadIndices(&sungeo, isun, sisize);
//		RenderScene(&shima[0], program, &kaze[0],true,-1);
		glUseProgram(program);
//			
//		MVP = glm::rotate(&MVP[0][0], 120.0f, vec3(0.0f,0.0f,1.0f)); //ProjectionMatrix * ViewMatrix;// * ModelMatrix;
		
		trans = glm::translate(MVP, spheresUnordered[3]->getCenter());
//		glUniformMatrix4fv(glGetUniformLocation(program, "ROT"), 1, GL_FALSE, &trans[0][0]);
		trans = glm::rotate(trans, (float)modelAngle, glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::rotate(trans, radians(-46.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::rotate(trans, radians(-35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		trans = glm::rotate(trans, (float)M_PI/-2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, &trans[0][0]);
		trans0 = glm::rotate(MVP, (float)modelAngle, glm::vec3(0.0f, 0.0f, 1.0f));
		trans0 = glm::rotate(trans0, radians(-46.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		trans0 = glm::rotate(trans0, radians(-35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		trans0 = glm::rotate(trans0, (float)M_PI/-2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
/*		trans0 = trans;
		trans0[3][0] = 0.0f;
		trans0[3][1] = 0.0f;
		trans0[3][2] = 0.0f;*/
		glUniformMatrix4fv(glGetUniformLocation(program, "ROT"), 1, GL_FALSE, &trans0[0][0]);
		glUseProgram(0);
//			
		for (int i=0; i<vertices0.size(); i++) {
			glUseProgram(program);
//			glUniform1i(glGetUniformLocation(program, "isModel"), -1);//spheres[i]->getCenter() != vec3(0.0f));
			glUniform1i(glGetUniformLocation(program, "isDiffuse"), 0);//spheres[i]->getCenter() != vec3(0.0f));
			glUniform1i(glGetUniformLocation(program, "isEarth"), 0);// != vec3(0.0f));
			glUniform1i(glGetUniformLocation(program, "isMoon"), 0);// != vec3(0.0f));
//			glUniform1f(glGetUniformLocation(program, "windOffset"), windspeed);// != vec3(0.0f));
			glUseProgram(0);
			RenderScene(&shima[i], program, &kaze[i],false,-1);
		}
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// clean up allocated resources before exit
//	DestroyGeometry(&geometry);
	glUseProgram(0);
	glDeleteProgram(program);
	glfwDestroyWindow(window);
	glfwTerminate();

	cout << "Goodbye!" << endl;
	return 0;
}

// ==========================================================================
// SUPPORT FUNCTION DEFINITIONS

// --------------------------------------------------------------------------
// OpenGL utility functions

void QueryGLVersion()
{
	// query opengl version and renderer information
	string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << endl;
}

bool CheckGLErrors()
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

// --------------------------------------------------------------------------
// OpenGL shader support functions

// reads a text file with the given name into a string
string LoadSource(const string &filename)
{
	string source;

	ifstream input(filename.c_str());
	if (input) {
		copy(istreambuf_iterator<char>(input),
			istreambuf_iterator<char>(),
			back_inserter(source));
		input.close();
	}
	else {
		cout << "ERROR: Could not load shader source from file "
			<< filename << endl;
	}

	return source;
}

// creates and returns a shader object compiled from the given source
GLuint CompileShader(GLenum shaderType, const string &source)
{
	// allocate shader object name
	GLuint shaderObject = glCreateShader(shaderType);

	// try compiling the source as a shader of the given type
	const GLchar *source_ptr = source.c_str();
	glShaderSource(shaderObject, 1, &source_ptr, 0);
	glCompileShader(shaderObject);

	// retrieve compile status
	GLint status;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
		cout << "ERROR compiling shader:" << endl << endl;
		cout << source << endl;
		cout << info << endl;
	}

	return shaderObject;
}

// creates and returns a program object linked from vertex and fragment shaders
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	// allocate program object name
	GLuint programObject = glCreateProgram();

	// attach provided shader objects to this program
	if (vertexShader)   glAttachShader(programObject, vertexShader);
	if (fragmentShader) glAttachShader(programObject, fragmentShader);

	// try linking the program with given attachments
	glLinkProgram(programObject);

	// retrieve link status
	GLint status;
	glGetProgramiv(programObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
		cout << "ERROR linking shader program:" << endl;
		cout << info << endl;
	}

	return programObject;
}
