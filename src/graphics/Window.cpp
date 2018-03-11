#include "window.h"

#include <iostream>

void Resize(GLFWwindow*, int width, int height);

Window::Window(const char* title, int width, int height) :
	lastKey(0)
{
	Init(title, width, height);
}

Window::~Window()
{
	glfwTerminate();
	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_alcContext);
	alcCloseDevice(m_alcDevice);
}

bool Window::Init(const char* title, int width, int height)
{
	m_Title = title;
	m_Width = width;
	m_Height = height;

	// Initialise GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialise GLFW!" << std::endl;
		return false;
	}

	// Create GLFW window
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
	if (!m_Window)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		return false;
	}

	// Set key array to false
	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		m_Keys[i] = false;
	}

	// Set mouse button array to false
	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{
		m_Buttons[i] = false;
	}

	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetWindowSizeCallback(m_Window, Resize);

	glfwSetKeyCallback(m_Window, key_callback);
	glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
	glfwSetCursorPosCallback(m_Window, mouse_cursor_position_callback);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialise GLEW!" << std::endl;
		return false;
	}

	glfwSwapInterval(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	m_alcDevice = alcOpenDevice(NULL);
	m_alcContext = alcCreateContext(m_alcDevice, NULL);
	alcMakeContextCurrent(m_alcContext);

	return true;
}

void Window::SetColour(const float& r, const float& g, const float& b, const float& a)
{
	glClearColor(r, g, b, a);
}

void Window::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Update()
{
	glfwSwapBuffers(m_Window);

	glfwPollEvents();
}

bool Window::Close()
{
	if (glfwWindowShouldClose(m_Window))
	{
		glfwTerminate();
		return true;
	}
}

void Resize(GLFWwindow*, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool Window::IsKeyPressed(unsigned int key)
{
	if (key > GLFW_KEY_LAST)
	{
		return false;
	}

	return m_Keys[key];
}

bool Window::OnKeyPress(unsigned int key)
{	
	if (m_Keys[key] == false)
	{
		if (key == lastKey)
			wasKeyPressed = false;

		return false;
	}

	else
	{
		if (wasKeyPressed == true)
			return false;

		wasKeyPressed = true;
		lastKey = key;
		return true;
	}
}

bool Window::IsButtonPressed(unsigned int button)
{
	if (button > GLFW_MOUSE_BUTTON_LAST)
	{
		return false;
	}

	return m_Buttons[button];
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_Keys[key] = action != GLFW_RELEASE;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_Buttons[button] = action != GLFW_RELEASE;
}

void Window::mouse_cursor_position_callback(GLFWwindow* window, double x, double y)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->xpos = x;
	win->ypos = y;
}

vec2 Window::GetCursorPosition()
{ 
	glfwGetCursorPos(m_Window, &xpos, &ypos); 
	return vec2(xpos, ypos); 
}