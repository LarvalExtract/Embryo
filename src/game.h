#pragma once

#include "utilities\logger.h"

#include "graphics/window.h"

#include "graphics/Scene.h"
#include "graphics/Renderable2D.h"

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
	Shape2D::Quad *testQuad;
	Sprite *pCrosshair;

	Mat4 proj;

	// Delta time
	double lastTime;
	double currentTime;
	double deltaTime;

	float counter;

	bool bInitialised;

};