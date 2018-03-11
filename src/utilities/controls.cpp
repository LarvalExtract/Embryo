#include "controls.h"
#include <maths/_maths.h>
#include <math.h>

double xpos, ypos;

vec2 lookAngle;
vec2 screenCentre;

float sensitivity = 0.002f;
float movementSpeed;

bool bEnableCamera = false;

void MatricesFromInputs(Window &window, Camera &camera, double deltaTime)
{	
	screenCentre.x = window.GetWidth() / 2;
	screenCentre.y = window.GetHeight() / 2;

	if (window.OnKeyPress(GLFW_KEY_Z))
	{
		bEnableCamera = !bEnableCamera;
		window.SetCursorPosition(screenCentre);
	}

	// Process camera inputs only if camera input is enabled
	if (bEnableCamera == true)
	{
		glfwSetInputMode(window.GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);	// Hides the mouse cursor (Should I call this every frame?)

		//// Mouse controls ////
		xpos = window.GetCursorPosition().x;
		ypos = window.GetCursorPosition().y;

		window.SetCursorPosition(screenCentre);

		lookAngle.x += sensitivity * float(screenCentre.x - xpos);
		lookAngle.y += sensitivity * float(screenCentre.y - ypos);

		clamp(lookAngle.y, -1.5f, 1.5f);		// Prevents camera from inverting

		camera.SetForward(
			cos(lookAngle.y) * sin(lookAngle.x),
			sin(lookAngle.y),
			cos(lookAngle.y) * cos(lookAngle.x));

		vec3 rightVector(
			sin(lookAngle.x - (3.14159f / 2.0f)),
			0,
			cos(lookAngle.x - (3.14159f / 2.0f)));

		vec3 upVector(cross(rightVector, camera.GetForward()));

		camera.SetUpward(upVector.x, upVector.y, upVector.z);


		//// Keyboard controls ////
		
		
		// Increases camera movement speed if shift is held
		(window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) ? movementSpeed = 10.0f : movementSpeed = 4.0f;

		// Movement controls
		if (window.IsKeyPressed(GLFW_KEY_W))
			camera.position += camera.forward * movementSpeed * deltaTime;

		if (window.IsKeyPressed(GLFW_KEY_A))
			camera.position -= rightVector * movementSpeed * deltaTime;

		if (window.IsKeyPressed(GLFW_KEY_S))
			camera.position -= camera.forward * movementSpeed * deltaTime;

		if (window.IsKeyPressed(GLFW_KEY_D))
			camera.position += rightVector * movementSpeed * deltaTime;

		if (window.IsKeyPressed(GLFW_KEY_SPACE))
			camera.position += upVector * movementSpeed * deltaTime;

		if (window.IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
			camera.position.y -= movementSpeed * deltaTime;
	}

	// 
	else
	{
		glfwSetInputMode(window.GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);		// Reveals the mouse cursor (Should I call this every frame?)
	}

	// Returns camera to home position
	if (window.IsKeyPressed(GLFW_KEY_H))
	{
		camera.SetPosition(0.0f, 1.0f, -3.0f);
		camera.SetUpward(0.0f, 1.0f, 0.0f);
		camera.SetForward(0.0f, 0.0f, 0.1f);
	}

	camera.viewMatrix = mat4::LookAt(camera.position, camera.position + camera.forward, camera.upward);
}