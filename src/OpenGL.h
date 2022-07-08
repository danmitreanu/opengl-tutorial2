#pragma once


#ifdef _WIN32

/*
	Static libs:
	glew32.lib
	glew32s.lib
	glfw3.lib
	glfw3_mt.lib
	glfw3dll.lib
	opengl32.lib

	Dynamic link libs:
	glew32.dll
*/
#include <GL/glew.h>

#else // Linux, Mac OS

#include <OpenGL/gl3.h>

#endif


#include <GLFW/glfw3.h>