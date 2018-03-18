#include "controls.h"

#include <maths/maths.h>
#include <math.h>

#include <utilities/console.h>

Controls::Controls()
	: bEnableCamera(false)
{
}

Controls::~Controls()
{
}

void Controls::MatricesFromInputs(Window &window, Camera &camera, double deltaTime)
{
	// TO-DO: Get the value from a console variable
	float sensitivity = 0.002f;

	// Toggle camera mode
	if (window.OnKeyPress(GLFW_KEY_Z))
	{
		bEnableCamera = !bEnableCamera;

		if (bEnableCamera == true)
		{
			glfwSetInputMode(window.GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			window.ResetCursorPosition();
		}
		else
		{
			glfwSetInputMode(window.GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	// Process camera inputs only if camera input is enabled
	if (bEnableCamera)
	{
		//// Mouse controls ////
		// Set look angle
		lookAngle.x += sensitivity * -window.GetCursorPosition().x;
		lookAngle.y += sensitivity * -window.GetCursorPosition().y;
		lookAngle.x = fmod(lookAngle.x, Maths::DoublePi);
		lookAngle.y = Maths::Clamp(lookAngle.y, -(Maths::HalfPi), Maths::HalfPi);

		camera.forwardVector.x = cos(lookAngle.y) * sin(lookAngle.x);
		camera.forwardVector.y = sin(lookAngle.y);
		camera.forwardVector.z = cos(lookAngle.y) * cos(lookAngle.x);

		Vec3<float> rightVector(
			sin(lookAngle.x - (Maths::HalfPi)),
			0,
			cos(lookAngle.x - (Maths::HalfPi)));

		Vec3<float> upVector(Maths::Cross(rightVector, camera.forwardVector));

		camera.upwardVector.x = upVector.x;
		camera.upwardVector.y = upVector.y;
		camera.upwardVector.z = upVector.z;

		if (Console::GetVarB("bPrintMouse"))
			Console::Log() << "Mouse position: X " << lookAngle.x << " | Y " << lookAngle.y << "\n";

		//// Keyboard controls ////		
		// Increases camera movement speed if shift is held
		movementSpeed = Console::GetVarF("CamSpeed");

		(window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) ? movementSpeed *= 2.0f : movementSpeed;

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

		window.ResetCursorPosition();
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
