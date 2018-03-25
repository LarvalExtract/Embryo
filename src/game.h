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

	// Update and draw scenes
	void Update();

	// Scenes
	// Add a new scene
	bool AddScene(Scene *pScene);

	// Remove an existing screen
	bool RemoveScene(Scene *pScene);

	// Swap an existing screen with a new scene (e.g., game level to "game over" screen)
	bool AddRemoveScene(Scene *pScene, Scene *pNewScene);

	Window window;

	std::vector<Scene*> scenes;

	// Delta time
	double lastTime;
	double currentTime;
	double deltaTime;

	float counter;

	bool bInitialised;

};