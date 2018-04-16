#include <iostream>

#include "window.h"

#include <utilities/console.h>

void Resize(GLFWwindow*, int width, int height);

Window::Window() :
	lastKey(0),
	m_alcDevice(nullptr),
	m_alcContext(nullptr)
{
}

Window::~Window()
{
	glfwTerminate();

	alcMakeContextCurrent(nullptr);

	if (m_alcContext != nullptr)
		alcDestroyContext(m_alcContext);

	if (m_alcDevice != nullptr)
		alcCloseDevice(m_alcDevice);
}

bool Window::Init(std::string title, int width, int height)
{
	this->title = title;
	this->width = width;
	this->height = height;

	// Initialise GLFW
	if (glfwInit() != GLFW_TRUE)
	{
		Console::Log(LogType::Error) << "Failed to initialise GLFW.\n";
		return false;
	}

	// Create GLFW window
	m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (!m_Window)
	{
		Console::Log(LogType::Error) << "Failed to create GLFW window.\n";
		return false;
	}

	// Set key array to false
	for (int i = 0; i < GLFW_KEY_LAST; i++)
		m_Keys[i] = false;

	// Set mouse button array to false
	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
		m_Buttons[i] = false;

	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetWindowSizeCallback(m_Window, Resize);

	glfwSetKeyCallback(m_Window, key_callback);
	glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
	glfwSetCursorPosCallback(m_Window, mouse_cursor_position_callback);

	glfwSwapInterval(0);

	// Initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		Console::Log(LogType::Error) << "Failed to initialise GLEW.\n";
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	m_alcDevice = alcOpenDevice(NULL);
	m_alcContext = alcCreateContext(m_alcDevice, NULL);
	alcMakeContextCurrent(m_alcContext);

	return true;
}

void Window::SetColour(float r, float g, float b, float a)
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

Vec2<double> Window::GetCursorPosition()
{ 
	glfwGetCursorPos(m_Window, &xpos, &ypos); 
	return Vec2<double>(xpos, ypos);
}