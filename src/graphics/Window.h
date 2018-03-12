#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <AL/alc.h>
#include <AL/al.h>

#include <maths/Vec2.h>
#include <maths/Vec4.h>

//TODO: Implement key action functions, i.e. "OnKeyPress", "OnKeyRelease" etc.

class Window
{
	//// Window
private:
	const char* m_Title;
	int m_Width, m_Height;

	GLFWwindow* m_Window;
	ALCdevice* m_alcDevice;
	ALCcontext* m_alcContext;

public:
	Window(const char* title, int width, int height);
	~Window();

	bool Init(const char* title, int width, int height);		// Initialise GLFW window

	void SetColour(float r, float g, float b, float a);

	void Clear();		// Clears GLFW window
	void Update();		// Updates GLFW window
	bool Close();		// Closes GLFW window

	inline int GetWidth() { return m_Width; };
	inline int GetHeight() { return m_Height; };
	inline GLFWwindow* GetWindowPtr() { return m_Window; };
	ALCdevice* GetAudioDevicePtr() { return m_alcDevice; };

	//// Keyboard input
private:
	bool m_Keys[GLFW_KEY_LAST];
	bool wasKeyPressed;
	unsigned int lastKey;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	bool IsKeyPressed(unsigned int key);
	bool OnKeyPress(unsigned int key);
	inline void SetKeyState(unsigned int key, bool state) { m_Keys[key] = state; };
	inline bool GetKeyState(unsigned int key) { return m_Keys[key]; }

	//// Mouse input
private:
	bool m_Buttons[GLFW_MOUSE_BUTTON_LAST];
	double xpos, ypos;

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void mouse_cursor_position_callback(GLFWwindow* window, double x, double y);

public:
	bool IsButtonPressed(unsigned int button);
	inline void SetButtonState(unsigned int button, bool state) { m_Buttons[button] = state; };

	Vec2<float> GetCursorPosition();
	inline void SetCursorPosition(const Vec2<float> &pos) { glfwSetCursorPos(m_Window, pos.x, pos.y); }
};
