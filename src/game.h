#pragma once

#include "utilities/console.h"

#include "graphics/window.h"

#include "graphics/scene.h"

#include <utilities/timer.h>

// Encapsulates the entire game
class Game
{
public:
	Game();
	~Game();

	void Start();

private:
	bool Initialise();

	// Process user input
	void ProcessInput();

	// Update the game's states and conditions
	void Update();

	// Draw frame to the window
	void Draw();

	Window window;

	std::vector<Scene*> scenes;

	// Delta time
	double lastTime;
	double currentTime;
	double deltaTime;

	float counter;

	bool bInitialised;

};