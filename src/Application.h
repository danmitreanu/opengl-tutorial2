#include <GLFW/glfw3.h>
#include <iostream>

//file: Application.h
class Application
{
public:
	Application() = default;
	Application& operator = (const Application&) = delete;

public:

	bool initialize(const char* window_name, std::size_t width, std::size_t height);

	void update(const float delta_seconds);
	//^ called from run() function. delta seconds is the ammount of time (in seconds) that passed since the last update() call. First call has 0 as argument.

	void render();
	//^ called from run() function.

public:
	void run();
	//^ this functions keeps the application alive until user presses escape

private:
	//add members here:
	GLFWwindow* m_Window;

	static void key_callback(GLFWwindow*, int, int, int, int);
};

