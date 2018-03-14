#include "game.h"

#include "graphics/sprite2d.h"
#include "graphics/sphere.h"
#include "graphics/pivot.h"
#include "graphics/model3d.h"
#include "graphics/box.h"

#include "utilities/controls.h"

#include <string>
#include <iostream>

Game::Game() : 
	bInitialised(false),
	counter(0.0f)
{
}

Game::~Game()
{
}

void Game::Start()
{
	if (!bInitialised)
	{
		//std::cerr << "The game is not initialised! Attempting to initialise...\n\n";
		
		// Attempt to initialise the game
		if (!Initialise())
		{
			std::cerr << "Could not initialise!" << std::endl;
			return;
		}
	}

	// GAME LOOP
	while (glfwWindowShouldClose(window.m_Window) == GLFW_FALSE)
	{
		ProcessInput();
		Update();
		Draw();
	}

	// Exit game
	glfwTerminate();
	bInitialised = false;
}

bool Game::Initialise()
{
	// Return if already initialised
	if (bInitialised)
		return false;

	Timer initTimer;

	// Abort game initialisation if the window failed to initialise
	if (!window.Init("Embryo", 1280, 720))
		return false;

	//window->SetColour(0.1f, 0.3f, 0.6f, 1.0f);

	// Print OpenGL and OpenAL version info
	std::cout << "OpenGL " << glGetString(GL_VERSION) << ", " << glGetString(GL_VENDOR) << ", " << glGetString(GL_RENDERER) << "\n"
			  << "OpenAL " << alGetString(AL_VERSION) << ", " << alGetString(AL_VENDOR) << ", " << alGetString(AL_RENDERER) << "\n"
			  << std::endl;

	std::cout << "Press Z to enable camera controls" << "\n" << std::endl;

	scene.SetSkybox("skybox_ocean.tga");

	Camera *pCamera = new Camera(Vec3<float>(0.0f, 1.0f, -3.0f), 60.0f, (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 1000.0f);
	scene.AddCamera(pCamera);

	Shader *pShader = new Shader("basicPhong");
	scene.AddShader(pShader);

	Model3D *pFloor = new Model3D("floor.mmf");
	pFloor->SetPos(0.0f, -1.0f, 0.0f);
	pFloor->SetTexture("models/floor.tga");
	pFloor->SetShader(pShader);
	scene.AddRenderable(pFloor);

	Model3D *pCube = new Model3D("cube.mmf");
	pCube->SetScale(0.2f, 0.2f, 0.2f);
	pCube->SetTexture("models/cube.tga");
	pCube->SetSpecularity(0.2f);
	pCube->SetGlossiness(20.0f);
	pCube->SetShader(pShader);
	scene.AddRenderable(pCube);

	Model3D *pTeapot = new Model3D("teapot.mmf");
	pTeapot->SetPos(1.2f, 0.0f, 0.0f);
	pTeapot->SetScale(0.02f, 0.02f, 0.02f);
	pTeapot->SetTexture("models/test.tga");
	pTeapot->SetSpecularity(2.0f);
	pTeapot->SetGlossiness(100.0f);
	pTeapot->SetShader(pShader);
	scene.AddRenderable(pTeapot);

	Model3D *pClone = new Model3D("poo.mmf");
	pClone->SetPos(-1.0f, 0.0f, 0.0f);
	pClone->SetRot(0.0f, 180.0f, 0.0f);
	pClone->SetScale(0.02f, 0.02f, 0.02f);
	pClone->SetTexture("models/clone.tga");
	pClone->SetSpecularity(0.5f);
	pClone->SetGlossiness(30.0f);
	pClone->SetShader(pShader);
	scene.AddRenderable(pClone);

	Sprite2D *pSprite = new Sprite2D("sprites/sprite_test3.tga");
	pSprite->SetPos(1.7f, 0.0f, -1.0f);
	pSprite->SetShader(pShader);
	scene.AddRenderable(pSprite);

	Sphere *pSphere = new Sphere(24, 2.0f, Vec4<float>(1.0f, 1.0f, 0.0f, 1.0f));
	pSphere->SetPos(-5, 2, 3);
	scene.AddRenderable(pSphere);

	Box *pBox = new Box(2, 3, 3, Vec4<float>(1.0f, 0.0f, 1.0f, 1.0f));
	pBox->SetPos(13.3f, 3.0f, 5.0f);
	pBox->SetRot(0.0f, 23.0f, 0.0f);
	scene.AddRenderable(pBox);

	Pivot *pPivot = new Pivot();
	pPivot->SetPos(0.0f, 2.0f, 0.0f);
	scene.AddRenderable(pPivot);

	AudioSourceGlobal *pWind = new AudioSourceGlobal("sandtrap_wind_loop.wav");
	pWind->SetGain(0.3f);
	pWind->SetLoop(true);
	scene.AddSound(pWind);

	AudioSourceLocal *pComputer = new AudioSourceLocal("alien_generator.wav");
	pComputer->SetGain(0.55f);
	pComputer->SetLoop(true);
	scene.AddSound(pComputer);

	LightOmni *pLight = new LightOmni(Vec3<float>(0.0f, 2.0f, 0.0f), Vec3<float>(1.0f, 1.0f, 1.0f), 1.0f);
	scene.AddLight(pLight);

	//LightOmni *pRedLight = new LightOmni(Vec3<float>(0.0f, 2.0f, 0.0f), Vec3<float>(1.0f, 1.0f, 1.0f), 1.0f);
	//scene->AddLight(redLight);

	testQuad = new Shape2D::Quad(250.0f, 250.0f);
	pCrosshair = new Sprite("sprites/crosshair.tga");
	pCrosshair->CenterOrigin();
	pCrosshair->SetPosition(window.GetWidth() / 2, window.GetHeight() / 2);

	//pCrosshair->SetPosition(0.0f, 0.0f);

	proj.Orthographic(0.0f, window.GetWidth(), 0.0f, window.GetHeight(), -1.0f, 1.0f);
	
	// Initialise the scene
	if (scene.InitialiseScene("Scene1") == false)
		return false;

	//scene->PrintRenderableList();
	//scene->PrintLightList();
	//scene->PrintSoundList();
	//scene->PrintCameraList();

	std::cout << "Initialisation time: " << initTimer.Elapsed() << " seconds" << "\n" << std::endl;

	lastTime = glfwGetTime();

	return bInitialised = true;
}


void Game::ProcessInput()
{
	MatricesFromInputs(window, scene.GetActiveCamera(), deltaTime);
}

void Game::Update()
{
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	// Display frame time/frame rate
	std::cerr << "Frame time: " << 1000 * deltaTime << "ms,\t" << static_cast<int>(1 / deltaTime + 0.5) << "fps" << "                   \r";
	
	counter += 1.0f * deltaTime;

	scene.GetRenderable("teapot")->SetRot(sin(counter * 10), 0.0f, cos(counter * 20));
	scene.GetLight("light_omni")->SetPosition(sin(counter) * 2.0f, 2.0f, cos(counter) * 2.0f);
	//scene->GetLight("light_omni")->SetPower(sin(counter * 2) * 3);

	pCrosshair->SetRotation(counter * 15.0f);
	//sprite->SetScale(cos(counter), cos(counter));

	scene.UpdateScene();
}

void Game::Draw()
{
	// Clear the current buffer
	window.Clear();

	// Draw everything in the scene
	scene.DrawScene();

	//testQuad->GetShader().Bind();
	//testQuad->GetShader().SetUniformMat4("projectionMatrix", proj);
	//testQuad->Draw();

	pCrosshair->GetShader().Bind();
	pCrosshair->GetShader().SetUniformMat4("projectionMatrix", proj);
	pCrosshair->GetShader().SetUniformMat4("mpMatrix", pCrosshair->GetTransformMatrix() * proj);
	pCrosshair->Draw();

	// Update the window with the next frame
	window.Update();
}