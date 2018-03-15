#include "controls.h"
#include <maths/maths.h>
#include <math.h>

double xpos, ypos;

Vec2<float> lookAngle;
Vec2<float> screenCentre;

float sensitivity = 0.002f;
float movementSpeed;

bool bEnableCamera = false;

void MatricesFromInputs(Window &window, Camera &camera, double deltaTime)
{	
	screenCentre.x = window.GetWidth() / 2;
	screenCentre.y = window.GetHeight() / 2;

	// Toggle camera mode
	if (window.OnKeyPress(GLFW_KEY_Z))
	{
		bEnableCamera = !bEnableCamera;
		window.SetCursorPosition(screenCentre);
	}

	// Process camera inputs only if camera input is enabled
	if (bEnableCamera)
	{
		glfwSetInputMode(window.GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);	// Hides the mouse cursor (Should I call this every frame?)

		//// Mouse controls ////
		xpos = window.GetCursorPosition().x;
		ypos = window.GetCursorPosition().y;

		window.SetCursorPosition(screenCentre);

		lookAngle.x += sensitivity * float(screenCentre.x - xpos);
		lookAngle.y += sensitivity * float(screenCentre.y - ypos);

		lookAngle.y = Maths::Clamp(lookAngle.y, -1.5f, 1.5f);		// Prevents camera from inverting

		camera.forwardVector.x = cos(lookAngle.y) * sin(lookAngle.x);
		camera.forwardVector.y = sin(lookAngle.y);
		camera.forwardVector.z = cos(lookAngle.y) * cos(lookAngle.x);

		Vec3<float> rightVector(
			sin(lookAngle.x - (3.14159f / 2.0f)),
			0,
			cos(lookAngle.x - (3.14159f / 2.0f)));

		Vec3<float> upVector(Maths::Cross(rightVector, camera.forwardVector));

		camera.upwardVector.x = upVector.x;
		camera.upwardVector.y = upVector.y;
		camera.upwardVector.z = upVector.z;


		//// Keyboard controls ////		
		// Increases camera movement speed if shift is held
		(window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) ? movementSpeed = 10.0f : movementSpeed = 4.0f;

		// Movement controls
		if (window.IsKeyPressed(GLFW_KEY_W))
			camera.position += camera.forwardVector * movementSpeed * static_cast<float>(deltaTime);

		if (window.IsKeyPressed(GLFW_KEY_A))
			camera.position -= rightVector * movementSpeed * static_cast<float>(deltaTime);

		if (window.IsKeyPressed(GLFW_KEY_S))
			camera.position -= camera.forwardVector * movementSpeed * static_cast<float>(deltaTime);

		if (window.IsKeyPressed(GLFW_KEY_D))
			camera.position += rightVector * movementSpeed * static_cast<float>(deltaTime);

		if (window.IsKeyPressed(GLFW_KEY_SPACE))
			camera.position += upVector * movementSpeed * static_cast<float>(deltaTime);

		if (window.IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
			camera.position.y -= movementSpeed * static_cast<float>(deltaTime);
	}

	// Give control of the mouse cursor back to the user once camera mode is disabled
	else
	{
		glfwSetInputMode(window.GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);		// Reveals the mouse cursor (Should I call this every frame?)
	}

	// Returns camera to home position
	if (window.IsKeyPressed(GLFW_KEY_H))
	{
		camera.position = Vec3<float>(0.0f, 1.0f, -3.0f);
		camera.upwardVector = Vec3<float>(0.0f, 1.0f, 0.0f);
		camera.forwardVector = Vec3<float>(0.0f, 0.0f, 0.1f);
	}

	camera.viewMatrix.LookAt(camera.position, camera.position + camera.forwardVector, camera.upwardVector);
}