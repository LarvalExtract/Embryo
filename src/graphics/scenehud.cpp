#include "scenehud.h"

#include <graphics/renderable/Box.h>
#include <graphics/renderable/Gizmo.h>
#include <graphics/renderable/Gyro.h>
#include <graphics/renderable/Model.h>
#include <graphics/renderable/skyBox.h>
#include <graphics/renderable/Sprite.h>
#include <graphics/text.h>

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
	Shader *pShader = new Shader("basicUnlit");
	AddShader(pShader);

	Camera *pOrtho = new CamOrtho(0.0f, window.GetWidth(), 0.0f, window.GetHeight(), -1.0f, 1.0f);
	AddCamera(pOrtho);

	SetActiveCamera(0);

	Sprite *pCrosshair = new Sprite("sprites/crosshair.tga", false);
	pCrosshair->CentreLocalOrigin();
	pCrosshair->SetPosition(window.GetWidth() / 2, window.GetHeight() / 2, 0.0f);
	pCrosshair->SetShader(pShader);
	AddRenderable(pCrosshair);

	//Text *pText = new Text("T");
	//AddRenderable(pText);

	return Scene::InitialiseScene(window);
}

bool SceneHud::InitialiseScene(Window &window, std::string name)
{
	return Scene::InitialiseScene(window, name);
}

void SceneHud::ProcessInput(float deltaTime)
{
	Scene::ProcessInput(deltaTime);

	GetRenderable("crosshair.tga")->SetRotation2D(counter * 30);
}

void SceneHud::UpdateScene(float deltaTime)
{
	counter += 1.0f * deltaTime;

	Scene::UpdateScene(deltaTime);
}

void SceneHud::DrawScene()
{
	Scene::DrawScene();
}