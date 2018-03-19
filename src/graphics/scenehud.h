#pragma once

#include "scene.h"

// In-game HUD scene
class SceneHud : public Scene
{
public:
	SceneHud();
	virtual ~SceneHud();

	virtual bool InitialiseScene(Window &window) override;
	virtual bool InitialiseScene(std::string sceneName, Window &window) override;
	virtual void ProcessInput(Window &window, float deltaTime) override;
	virtual void UpdateScene(float deltaTime) override;
	virtual void DrawScene() override;
};
