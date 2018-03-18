#pragma once

#include "utilities/console.h"

#include "graphics/window.h"
#include <graphics/renderable/box3d.h>
#include <graphics/renderable/gizmo3d.h>
#include <graphics/renderable/gyro3d.h>
#include <graphics/renderable/model3d.h>
#include <graphics/renderable/skybox3d.h>
#include <graphics/renderable/sprite2d.h>
#include <graphics/renderable/sprite3d.h>
#include "graphics/scene.h"

#include "utilities/controls.h"

#include <utilities/timer.h>

// Encapsulates the entire game
class Game
{
public:
	Game();
	~Game();

	bool Initialise();
	void Start();

private:
	// Process user input
	void ProcessInput();

	// Update the game's states and conditions
	void Update();

	// Draw frame to the window
	void Draw();

	Window window;
	Scene scene;
	Scene hud;

	Controls controls;

	// Delta time
	double lastTime;
	double currentTime;
	double deltaTime;

	float counter;

	bool bInitialised;

};