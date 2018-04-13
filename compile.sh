LIBS="-lglfw -lpthread -lm -lXi -lXrandr -lX11 -lXxf86vm -lXinerama -lXcursor -lGLU -ldl -lOpenGL "



g++ middleware/glad/src/glad.c $LIBS ./boilerplate/*.cpp -Wall -g -I"./middleware/glad/include" -I"./middleware"  -o "solar.o"
