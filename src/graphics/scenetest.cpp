#include "scenetest.h"

#include <utilities/console.h>

#include <graphics/renderable/box3d.h>
#include <graphics/renderable/gizmo3d.h>
#include <graphics/renderable/gyro3d.h>
#include <graphics/renderable/modelmmf.h>
#include <graphics/renderable/skybox3d.h>
#include <graphics/renderable/sprite2d.h>
#include <graphics/renderable/sprite3d.h>

SceneTest::SceneTest() :
	Scene(),
	counter(0)
{
}

SceneTest::~SceneTest()
{
}

bool SceneTest::InitialiseScene(Window &window)
{
	SetSkybox("skybox_ocean.tga");

	Camera *pCamera = new CamPersp(60.0f, static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight()), 0.01f, 1000.0f);
	pCamera->SetPosition(0.0f, 1.0f, -3.0f);
	AddCamera(pCamera);

	SetActiveCamera(0);

	Shader *pShader = new Shader("basicPhong");
	AddShader(pShader);

	ModelMMF *pFloor = new ModelMMF("floor.mmf");
	pFloor->SetPosition(0.0f, -1.0f, 0.0f);
	pFloor->SetDiffuseTexture("models/floor.tga");
	pFloor->SetShader(pShader);
	AddRenderable(pFloor);

	ModelMMF *pCube = new ModelMMF("cube.mmf");
	pCube->SetScale(0.2f, 0.2f, 0.2f);
	pCube->SetDiffuseTexture("models/cube.tga");
	pCube->specularity = 0.2f;
	pCube->glossiness = 20.0f;
	pCube->SetShader(pShader);
	AddRenderable(pCube);

	ModelMMF *pTeapot = new ModelMMF("teapot.mmf");
	pTeapot->SetPosition(1.2f, 0.0f, 0.0f);
	pTeapot->SetScale(0.02f, 0.02f, 0.02f);
	pTeapot->SetDiffuseTexture("models/test.tga");
	pTeapot->specularity = 2.0f;
	pTeapot->glossiness = 100.0f;
	pTeapot->SetShader(pShader);
	AddRenderable(pTeapot);

	ModelMMF *pClone = new ModelMMF("poo.mmf");
	pClone->SetPosition(-1.0f, 0.0f, 0.0f);
	pClone->SetRotation3D(0.0f, 180.0f, 0.0f);
	pClone->SetScale(0.02f, 0.02f, 0.02f);
	pClone->SetDiffuseTexture("models/clone.tga");
	pClone->specularity = 0.5f;
	pClone->glossiness = 30.0f;
	pClone->SetShader(pShader);
	AddRenderable(pClone);

	Sprite3D *pSprite = new Sprite3D("sprites/sprite_test3.tga");
	pSprite->SetPosition(1.7f, 0.0f, -1.0f);
	pSprite->SetShader(pShader);
	AddRenderable(pSprite);

	Gyro3D *pSphere = new Gyro3D(2.0f, 4);
	pSphere->SetPosition(-5, 2, 3);
	pSphere->SetColour(1.0f, 1.0f, 0.0f, 1.0f);
	AddRenderable(pSphere);

	Box3D *pBox = new Box3D(2, 3, 3);
	pBox->SetPosition(13.3f, 3.0f, 5.0f);
	pBox->SetRotation3D(0.0f, 23.0f, 0.0f);
	pBox->SetColour(1.0f, 0.0f, 1.0f, 1.0f);
	AddRenderable(pBox);

	Gizmo3D *pPivot = new Gizmo3D();
	pPivot->SetPosition(0.0f, 2.0f, 0.0f);
	AddRenderable(pPivot);

	AudioSourceGlobal *pWind = new AudioSourceGlobal("sandtrap_wind_loop.wav");
	pWind->SetGain(0.3f);
	pWind->SetLoop(true);
	AddSound(pWind);

	AudioSourceLocal *pComputer = new AudioSourceLocal("alien_generator.wav");
	pComputer->SetGain(0.55f);
	pComputer->SetLoop(true);
	AddSound(pComputer);

	LightOmni *pLight = new LightOmni(Vec3<float>(0.0f, 2.0f, 0.0f), Vec3<float>(1.0f, 1.0f, 1.0f), 1.0f);
	AddLight(pLight);

	LightOmni *pRedLight = new LightOmni(Vec3<float>(4.0f, 2.0f, 0.0f), Vec3<float>(1.0f, 0.0f, 0.0f), 1.0f);
	pRedLight->name = "light_red";
	AddLight(pRedLight);

	return Scene::InitialiseScene(window);
}

bool SceneTest::InitialiseScene(Window &window, std::string name)
{
	return Scene::InitialiseScene(window, name);
}

void SceneTest::ProcessInput(float deltaTime)
{
	Scene::ProcessInput(deltaTime);
}

void SceneTest::UpdateScene(float deltaTime)
{
	counter += 1.0f * deltaTime;

	// BUG: Crashes if the scene cannot find a renderable or light
	GetRenderable("teapot")->SetRotation3D(sin(counter * 10), 0.0f, cos(counter * 20));
	GetLight("light_omni")->position = Vec3<float>(sin(counter) * 2.0f, 2.0f, cos(counter) * 2.0f);
	GetLight("light_red")->power = (sin(counter * 2) * 0.5f) + 0.5f;

	Scene::UpdateScene(deltaTime);
}

void SceneTest::DrawScene()
{
	Scene::DrawScene();
}