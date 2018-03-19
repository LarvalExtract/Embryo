#include "scenehud.h"

#include <graphics/renderable/box3d.h>
#include <graphics/renderable/gizmo3d.h>
#include <graphics/renderable/gyro3d.h>
#include <graphics/renderable/model3d.h>
#include <graphics/renderable/skybox3d.h>
#include <graphics/renderable/sprite2d.h>
#include <graphics/renderable/sprite3d.h>

#include <utilities/console.h>

SceneHud::SceneHud() :
	Scene(),
	counter(0)
{
}

SceneHud::~SceneHud()
{
}

bool SceneHud::InitialiseScene(Window &window)
{
	Shader *pShader = new Shader("basicPhong");
	AddShader(pShader);

	Camera *pOrtho = new CamOrtho(0.0f, window.GetWidth(), 0.0f, window.GetHeight(), -1.0f, 1.0f);
	AddCamera(pOrtho);

	SetActiveCamera(0);

	Sprite2D *pCrosshair = new Sprite2D("sprites/crosshair.tga");
	pCrosshair->CentreLocalOrigin();
	pCrosshair->SetPosition(window.GetWidth() / 2, window.GetHeight() / 2, 0.0f);
	AddRenderable(pCrosshair);

	return Scene::InitialiseScene(window);
}

bool SceneHud::InitialiseScene(std::string name, Window &window)
{
	return Scene::InitialiseScene(name, window);
}

void SceneHud::ProcessInput(Window &window, float deltaTime)
{
	Scene::ProcessInput(window, deltaTime);

	GetRenderable("crosshair.tga")->SetRotation2D(counter * 30);
}

void SceneHud::UpdateScene(float deltaTime)
{
	Scene::UpdateScene(deltaTime);

	counter += 1.0f * deltaTime;
}

void SceneHud::DrawScene()
{
	Scene::DrawScene();
}