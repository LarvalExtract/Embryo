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
public:
	Window();
	~Window();

	// Initialise GLFW window
	bool Init(std::string title, int width, int height);

	// Keyboard input
	bool IsKeyPressed(unsigned int key);
	bool OnKeyPress(unsigned int key);
	inline void SetKeyState(unsigned int key, bool state) { m_Keys[key] = state; };
	inline bool GetKeyState(unsigned int key) { return m_Keys[key]; }

	// Mouse input
	bool IsButtonPressed(unsigned int button);
	inline void SetButtonState(unsigned int button, bool state) { m_Buttons[button] = state; };
	Vec2<double> GetCursorPosition();
	inline void SetCursorPosition(const Vec2<double> &pos) { glfwSetCursorPos(m_Window, pos.x, pos.y); }

	void SetColour(float r, float g, float b, float a);

	void Clear();		// Clears GLFW window
	void Update();		// Updates GLFW window

	inline int GetWidth() { return width; };
	inline int GetHeight() { return height; };
	inline GLFWwindow* GetWindowPtr() { return m_Window; };
	ALCdevice* GetAudioDevicePtr() { return m_alcDevice; };

	GLFWwindow *m_Window;

private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	bool m_Keys[GLFW_KEY_LAST];
	bool wasKeyPressed;
	unsigned int lastKey;

	// Window propreties
	std::string title;
	int width;
	int height;

	ALCdevice* m_alcDevice;
	ALCcontext* m_alcContext;

	bool m_Buttons[GLFW_MOUSE_BUTTON_LAST];
	double xpos, ypos;

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void mouse_cursor_position_callback(GLFWwindow* window, double x, double y);
};
