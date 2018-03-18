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

	screenCentre.x = 0;
	screenCentre.y = 0;

	// Toggle camera mode
	if (window.OnKeyPress(GLFW_KEY_Z))
	{
		bEnableCamera = !bEnableCamera;

		if (bEnableCamera == true)
		{
			glfwSetInputMode(window.GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			window.SetCursorPosition(screenCentre + Vec2<double>(pos.x, pos.y));
		}
		else
		{
			glfwSetInputMode(window.GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			//window.SetCursorPosition(screenCentre);
		}
	}

	// Process camera inputs only if camera input is enabled
	if (bEnableCamera)
	{
		//// Mouse controls ////
		pos.x = window.GetCursorPosition().x;

		// Clamp the cursor's Y position to the window draw region
		if (window.GetCursorPosition().y > window.GetHeight())
			window.SetCursorPosition(Vec2<double>(window.GetCursorPosition().x, window.GetHeight()));
		else if (window.GetCursorPosition().y < -window.GetHeight())
			window.SetCursorPosition(Vec2<double>(window.GetCursorPosition().x, -window.GetHeight()));

		pos.y = Maths::Clamp(window.GetCursorPosition().y, -window.GetHeight(), window.GetHeight());

		// Set look angle
		lookAngle.x = sensitivity * -pos.x;
		lookAngle.y = sensitivity * -pos.y;

		camera.forwardVector.x = cos(lookAngle.y) * sin(lookAngle.x);
		camera.forwardVector.y = sin(lookAngle.y);
		camera.forwardVector.z = cos(lookAngle.y) * cos(lookAngle.x);

		Vec3<float> rightVector(
			sin(lookAngle.x - (Maths::Pi * 0.5f)),
			0,
			cos(lookAngle.x - (Maths::Pi * 0.5f)));

		Vec3<float> upVector(Maths::Cross(rightVector, camera.forwardVector));

		camera.upwardVector.x = upVector.x;
		camera.upwardVector.y = upVector.y;
		camera.upwardVector.z = upVector.z;

		if (Console::GetVarB("bPrintMouse"))
			Console::Log() << "Mouse position: X " << pos.x << " | Y " << pos.y << "\n";

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
