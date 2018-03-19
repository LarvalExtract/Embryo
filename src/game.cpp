#include "game.h"

#include <string>
#include <iostream>
#include <thread>

#include "graphics/scenetest.h"
#include "graphics/scenehud.h"

Game::Game() : 
	bInitialised(false),
	counter(0.0f)
{
}

Game::~Game()
{
	// Clean up scenes
	for (auto scene : scenes)
	{
		if (scene != nullptr)
			delete scene;
	}
}

void Game::Start()
{
	if (!bInitialised)
	{
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
	// TO-DO: Find out if this could cause corruption
	Console::bIsRunning = false;
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
	Console::AddCommand("cmdtest", Console::CmdTest, "Test command");
	Console::AddCommand("listC", Console::CmdListC, "List all commands");
	Console::AddCommand("listV", Console::CmdListV, "List all variables");
	Console::AddVar("timescale", "1.0");
	Console::AddVar("bPrintMouse", false);
	Console::AddVar("CamSpeed", "4.0");

	Timer initTimer;

	// Abort game initialisation if the window failed to initialise
	if (!window.Init("Embryo", 1280, 720))
		return false;

	//window->SetColour(0.1f, 0.3f, 0.6f, 1.0f);

	// Print OpenGL and OpenAL version info
	Console::Log(LogType::Log) << "OpenGL " << ColourCode::BrightYellow << glGetString(GL_VERSION) << ColourCode::White << ", " << glGetString(GL_VENDOR) << ", " << glGetString(GL_RENDERER) << "\n";
	Console::Log(LogType::Log) << "OpenAL " << ColourCode::BrightYellow << alGetString(AL_VERSION) << ColourCode::White << ", " << alGetString(AL_VENDOR) << ", " << alGetString(AL_RENDERER) << "\n\n";

	Console::Log(LogType::Log) << "Press Z to enable camera controls" << "\n\n";

	SceneTest *pSceneTest = new SceneTest();
	SceneHud *pSceneHud = new SceneHud();
	scenes.push_back(pSceneTest);
	scenes.push_back(pSceneHud);

	// Initialise the scenes
	if (pSceneTest->InitialiseScene("Scene1", window) == false)
		return false;

	if (pSceneHud->InitialiseScene("Scene2", window) == false)
		return false;

	Console::Log() << "Initialisation time: " << initTimer.Elapsed() << " seconds" << "\n";

	lastTime = glfwGetTime();

	return bInitialised = true;
}

void Game::ProcessInput()
{
	// TO-DO: Update delta before this step
	for (auto scene : scenes)
		if (scene != nullptr)
			scene->ProcessInput(window, deltaTime);
}

void Game::Update()
{
	currentTime = glfwGetTime();
	deltaTime = (currentTime - lastTime) * Console::GetVarF("timescale");
	lastTime = currentTime;

	// Display frame time/frame rate
	//Console::Log() << "Frame time: " << ColourCode::BrightGreen << 1000 * deltaTime << "ms,\t" << static_cast<int>(1 / deltaTime + 0.5) << "fps" << "                   \r";
	
	// Update scenes
	for (auto scene : scenes)
		if (scene != nullptr)
			scene->UpdateScene(deltaTime);
}

void Game::Draw()
{
	// Clear the current buffer
	window.Clear();

	// Draw everything in the scene
	for (auto scene : scenes)
		if (scene != nullptr)
			scene->DrawScene();

	// Update the window with the next frame
	window.Update();
}