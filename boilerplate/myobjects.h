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
    std::vector < std::vector < glm::vec3 > >& out_vertices,
    std::vector < std::vector < glm::vec2 > >& out_uvs,
    std::vector < std::vector < glm::vec3 > >& out_normals,
    std::vector < unsigned int > & texture_names
);
