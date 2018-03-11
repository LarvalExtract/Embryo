#pragma once

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

	void Start();

private:
	bool Initialise();
	void ProcessInput();
	void Update();
	void Draw();

	Window *window;

	Embryo::Scene *scene;
	Shape2D::Quad *testQuad;
	Sprite *sprite;

	mat4 proj;

	// Delta time
	double lastTime;
	double currentTime;
	double deltaTime;

	float counter;

	bool bInitialised;
};