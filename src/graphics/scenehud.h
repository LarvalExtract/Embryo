#pragma once

#include "scene.h"

// In-game HUD scene
class SceneHud : public Scene
{
public:
	SceneHud();
	virtual ~SceneHud();

	virtual bool InitialiseScene(Window &window) override;
	virtual bool InitialiseScene(Window &window, std::string sceneName) override;
	virtual void ProcessInput(float deltaTime) override;
	virtual void UpdateScene(float deltaTime) override;
	virtual void DrawScene() override;

private:
	float counter;
};
