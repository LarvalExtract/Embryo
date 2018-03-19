#pragma once

#include "scene.h"

class SceneTest : public Scene
{
public:
	SceneTest();
	virtual ~SceneTest();

	virtual bool InitialiseScene(Window &window) override;
	virtual bool InitialiseScene(std::string sceneName, Window &window) override;
	virtual void ProcessInput(Window &window, float deltaTime) override;
	virtual void UpdateScene(float deltaTime) override;
	virtual void DrawScene() override;

private:
	float counter;
};
