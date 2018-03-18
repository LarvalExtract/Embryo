#include "game.h"

#include "utilities/controls.h"

#include <string>
#include <iostream>
#include <thread>

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
			Console::Log(LogType::Error) << "Could not initialise!\n";
			return;
		}
	}

	// Start console thread
	std::thread tConsole(Console::ConsoleLoop);

	// GAME LOOP
	while (glfwWindowShouldClose(window.m_Window) == GLFW_FALSE)
	{
		ProcessInput();
		Update();
		Draw();
	}

	// Clean up threads
	Console::bIsRunning = false;
	//tConsole.join();
	// TO-DO: Find out if this could cause corruption
	tConsole.detach();

	// Exit game
	glfwTerminate();
	bInitialised = false;
}

bool Game::Initialise()
{
	// Return if already initialised
	if (bInitialised)
		return false;

	// TO-DO: DELETE ME
	Console::AddCommand("cmdtest", Console::CmdTest);
	Console::AddVar("timescale", "1.0");

	Timer initTimer;

	// Abort game initialisation if the window failed to initialise
	if (!window.Init("Embryo", 1280, 720))
		return false;

	//window->SetColour(0.1f, 0.3f, 0.6f, 1.0f);

	// Print OpenGL and OpenAL version info
	Console::Log(LogType::Log) << "OpenGL " << ColourCode::BrightYellow << glGetString(GL_VERSION) << ColourCode::White << ", " << glGetString(GL_VENDOR) << ", " << glGetString(GL_RENDERER) << "\n";
	Console::Log(LogType::Log) << "OpenAL " << ColourCode::BrightYellow << alGetString(AL_VERSION) << ColourCode::White << ", " << alGetString(AL_VENDOR) << ", " << alGetString(AL_RENDERER) << "\n\n";

	Console::Log(LogType::Log) << "Press Z to enable camera controls" << "\n\n";

	scene.SetSkybox("skybox_ocean.tga");

	Console::AddVar("CamSpeed", "4.0");
	Camera *pCamera = new CamPersp(60.0f, static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight()), 0.01f, 1000.0f);
	pCamera->SetPosition(0.0f, 1.0f, -3.0f);
	scene.AddCamera(pCamera);

	Camera *pOrtho = new CamOrtho(0.0f, window.GetWidth(), 0.0f, window.GetHeight(), -1.0f, 1.0f);
	hud.AddCamera(pOrtho);

	scene.SetActiveCamera(0);
	hud.SetActiveCamera(0);

	Shader *pShader = new Shader("basicPhong");
	scene.AddShader(pShader);

	Model3D *pFloor = new Model3D("floor.mmf");
	pFloor->SetPosition(0.0f, -1.0f, 0.0f);
	pFloor->SetDiffuseTexture("models/floor.tga");
	pFloor->SetShader(pShader);
	scene.AddRenderable(pFloor);

	Model3D *pCube = new Model3D("cube.mmf");
	pCube->SetScale(0.2f, 0.2f, 0.2f);
	pCube->SetDiffuseTexture("models/cube.tga");
	pCube->specularity = 0.2f;
	pCube->glossiness = 20.0f;
	pCube->SetShader(pShader);
	scene.AddRenderable(pCube);

	Model3D *pTeapot = new Model3D("teapot.mmf");
	pTeapot->SetPosition(1.2f, 0.0f, 0.0f);
	pTeapot->SetScale(0.02f, 0.02f, 0.02f);
	pTeapot->SetDiffuseTexture("models/test.tga");
	pTeapot->specularity = 2.0f;
	pTeapot->glossiness = 100.0f;
	pTeapot->SetShader(pShader);
	scene.AddRenderable(pTeapot);

	Model3D *pClone = new Model3D("poo.mmf");
	pClone->SetPosition(-1.0f, 0.0f, 0.0f);
	pClone->SetRotation3D(0.0f, 180.0f, 0.0f);
	pClone->SetScale(0.02f, 0.02f, 0.02f);
	pClone->SetDiffuseTexture("models/clone.tga");
	pClone->specularity = 0.5f;
	pClone->glossiness = 30.0f;
	pClone->SetShader(pShader);
	scene.AddRenderable(pClone);

	Sprite3D *pSprite = new Sprite3D("sprites/sprite_test3.tga");
	pSprite->SetPosition(1.7f, 0.0f, -1.0f);
	pSprite->SetShader(pShader);
	scene.AddRenderable(pSprite);

	Gyro3D *pSphere = new Gyro3D(2.0f, 4);
	pSphere->SetPosition(-5, 2, 3);
	pSphere->SetColour(1.0f, 1.0f, 0.0f, 1.0f);
	scene.AddRenderable(pSphere);

	Box3D *pBox = new Box3D(2, 3, 3);
	pBox->SetPosition(13.3f, 3.0f, 5.0f);
	pBox->SetRotation3D(0.0f, 23.0f, 0.0f);
	pBox->SetColour(1.0f, 0.0f, 1.0f, 1.0f);
	scene.AddRenderable(pBox);

	Gizmo3D *pPivot = new Gizmo3D();
	pPivot->SetPosition(0.0f, 2.0f, 0.0f);
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

	Sprite2D *pCrosshair = new Sprite2D("sprites/crosshair.tga");
	pCrosshair->CentreLocalOrigin();
	pCrosshair->SetPosition(window.GetWidth() / 2, window.GetHeight() / 2, 0.0f);
	hud.AddRenderable(pCrosshair);
	
	// Initialise the scene
	if (scene.InitialiseScene("Scene1") == false)
		return false;

	//hud.PrintRenderableList();
	//scene->PrintLightList();
	//scene->PrintSoundList();
	//scene->PrintCameraList();

	Console::Log() << "Initialisation time: " << initTimer.Elapsed() << " seconds" << "\n";

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
	deltaTime = (currentTime - lastTime) * Console::GetVarF("timescale");
	lastTime = currentTime;

	// Display frame time/frame rate
	//Console::Log() << "Frame time: " << ColourCode::BrightGreen << 1000 * deltaTime << "ms,\t" << static_cast<int>(1 / deltaTime + 0.5) << "fps" << "                   \r";
	
	counter += 1.0f * deltaTime;

	// BUG: Crashes if the scene cannot find a renderable or light
	scene.GetRenderable("teapot")->SetRotation3D(sin(counter * 10), 0.0f, cos(counter * 20));
	scene.GetLight("light_omni")->SetPosition(sin(counter) * 2.0f, 2.0f, cos(counter) * 2.0f);
	//scene->GetLight("light_omni")->SetPower(sin(counter * 2) * 3);

	hud.GetRenderable("crosshair.tga")->SetRotation2D(counter * 30);

	scene.UpdateScene();
}

void Game::Draw()
{
	// Clear the current buffer
	window.Clear();

	// Draw everything in the scene
	scene.DrawScene();
	hud.DrawScene();

	//pCrosshair->Draw(;

	// Update the window with the next frame
	window.Update();
}