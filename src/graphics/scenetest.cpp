#include "scenetest.h"

#include <utilities/console.h>

#include <graphics/renderable/Box.h>
#include <graphics/renderable/Gizmo.h>
#include <graphics/renderable/Gyro.h>
#include <graphics/renderable/modelmmf.h>
#include <graphics/renderable/skyBox.h>
#include <graphics/renderable/Sprite.h>
#include <graphics/text.h>

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

	Shader *pUnlitShader = new Shader("basicUnlit");
	AddShader(pUnlitShader);

	ModelMMF *pFloor = new ModelMMF("floor.mmf");
	pFloor->SetPosition(0.0f, -1.0f, 0.0f);
	pFloor->material.diffuseMap = new Texture2D("models/floor.tga");
	pFloor->SetShader(pShader);
	AddRenderable(pFloor);

	ModelMMF *pCube = new ModelMMF("cube.mmf");
	pCube->SetScale(0.2f, 0.2f, 0.2f);
	//pCube->material.diffuseMap = new Texture2D("models/cube.tga");
	pCube->material.specularity = 0.2f;
	pCube->material.glossiness = 20.0f;
	pCube->SetShader(pShader);
	AddRenderable(pCube);

	ModelMMF *pTeapot = new ModelMMF("teapot.mmf");
	pTeapot->SetPosition(1.2f, 0.0f, 0.0f);
	pTeapot->SetScale(0.02f, 0.02f, 0.02f);
	pTeapot->material.diffuseMap = new Texture2D("models/test.tga");
	pTeapot->material.specularity = 2.0f;
	pTeapot->material.glossiness = 100.0f;
	pTeapot->SetShader(pShader);
	AddRenderable(pTeapot);

	ModelMMF *pClone = new ModelMMF("poo.mmf");
	pClone->SetPosition(-1.0f, 0.0f, 0.0f);
	pClone->SetRotation3D(0.0f, 180.0f, 0.0f);
	pClone->SetScale(0.02f, 0.02f, 0.02f);
	pClone->material.diffuseMap = new Texture2D("models/clone.tga");
	pClone->material.specularity = 0.5f;
	pClone->material.glossiness = 30.0f;
	pClone->SetShader(pShader);
	AddRenderable(pClone);

	Sprite *pSprite = new Sprite("sprites/hammer_light.tga", true);
	pSprite->SetPosition(1.7f, 0.0f, -1.0f);
	pSprite->SetShader(pUnlitShader);
	AddRenderable(pSprite);

	Gyro *pSphere = new Gyro(2.0f, 16);
	pSphere->SetPosition(-5, 2, 3);
	pSphere->SetColour(1.0f, 1.0f, 0.0f, 1.0f);
	AddRenderable(pSphere);

	Box *pBox = new Box(2, 3, 3);
	pBox->SetPosition(13.3f, 3.0f, 5.0f);
	pBox->SetRotation3D(0.0f, 23.0f, 0.0f);
	pBox->SetColour(1.0f, 0.0f, 1.0f, 1.0f);
	AddRenderable(pBox);

	Gizmo *pPivot = new Gizmo();
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

	LightOmni *pLight = new LightOmni();
	pLight->SetPosition(0.0f, 2.0f, 0.0f);
	AddLight(pLight);

	LightOmni *pRedLight = new LightOmni();
	pRedLight->name = "light_red";
	pRedLight->SetPosition(4.0f, 2.0f, 0.0f);
	pRedLight->SetColour(1.0f, 0.0f, 0.0f);
	AddLight(pRedLight);

	sceneShaders["basicPhong"]->Bind();
	sceneShaders["basicPhong"]->SetUniformInt("u_numLights", sceneLights.size());
	sceneShaders["basicPhong"]->SetUniformFloat("ambience", 0.4f);

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

	sceneShaders["basicPhong"]->Bind();

	Scene::UpdateScene(deltaTime);
}

void SceneTest::DrawScene()
{
	Scene::DrawScene();
}