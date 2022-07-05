#include <iostream>

#include "VertexLayout.h"

int main()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "test", NULL, NULL);

	if (!window)
		return -1;

	glfwMakeContextCurrent(window);

	VertexLayout vlayout;
	VertexBuffer vbuffer;

	vlayout.AddVertexAttribute("attrib1", 2);
	vlayout.AddVertexAttribute("attrib2", 3);

	float data[] = { 0.2f, 0.2f, 1, 1, 1, 0.5f, 0.5f, 1, 1, 1 };

	vbuffer.create(data, vlayout, sizeof(data) / vlayout.float_count());

	glfwTerminate();

	return 0;
}
