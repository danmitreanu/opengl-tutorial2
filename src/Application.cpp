#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"

bool Application::initialize(const char* window_name, std::size_t width, std::size_t height)
{
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_Window = glfwCreateWindow(width, height, window_name, NULL, NULL);

	if (!m_Window)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_Window);
	glfwSetKeyCallback(m_Window, Application::key_callback);
	glfwSetWindowUserPointer(m_Window, this);

	return true;
}

void Application::run()
{
	float delta_time = glfwGetTime();

	while (!glfwWindowShouldClose(m_Window))
	{
		float now_time = glfwGetTime();

		this->update(now_time - delta_time);
		this->render();
		delta_time = now_time;

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

void Application::update(const float delta_seconds)
{
	std::cout << delta_seconds << std::endl;
}

void Application::render()
{
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* handler = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(handler->m_Window, GLFW_TRUE);
}