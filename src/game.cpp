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
		Update();

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
	Console::AddCommand("listc", Console::CmdListC, "List all commands");
	Console::AddCommand("listv", Console::CmdListV, "List all variables");
	Console::AddVar("timescale", "1.0");
	Console::AddVar("bprintmouse", false);
	Console::AddVar("camspeed", "4.0");

	Timer initTimer;

	// Abort game initialisation if the window failed to initialise
	if (!window.Init("Embryo", 1280, 720))
		return false;

	// Print OpenGL and OpenAL version info
	Console::Log(LogType::Log) << "OpenGL " << ColourCode::BrightYellow << glGetString(GL_VERSION) << ColourCode::White << ", " << glGetString(GL_VENDOR) << ", " << glGetString(GL_RENDERER) << "\n";
	Console::Log(LogType::Log) << "OpenAL " << ColourCode::BrightYellow << alGetString(AL_VERSION) << ColourCode::White << ", " << alGetString(AL_VENDOR) << ", " << alGetString(AL_RENDERER) << "\n\n";

	Console::Log(LogType::Log) << "Press Z to enable camera controls" << "\n\n";

	SceneTest *pSceneTest = new SceneTest();
	SceneHud *pSceneHud = new SceneHud();
	scenes.push_back(pSceneTest);
	scenes.push_back(pSceneHud);

	// Initialise the scenes
	if (!pSceneTest->InitialiseScene(window, "Scene1"))

		return false;

	if (!pSceneHud->InitialiseScene(window, "Scene2"))
		return false;

	Console::Log() << "Initialisation time: " << initTimer.Elapsed() << " seconds" << "\n";

	lastTime = glfwGetTime();

	return bInitialised = true;
}

void Game::Update()
{
	currentTime = glfwGetTime();
	deltaTime = (currentTime - lastTime) * Console::GetVarF("timescale");
	lastTime = currentTime;

	// Display frame time/frame rate
	//Console::Log() << "Frame time: " << ColourCode::BrightGreen << 1000 * deltaTime << "ms,\t" << static_cast<int>(1 / deltaTime + 0.5) << "fps" << "                   \r";

	// Clear the current buffer
	window.Clear();

	// Update scenes
	for (std::vector<Scene*>::iterator scene = scenes.begin(); scene != scenes.end(); scene++)
	{
		if (*scene != nullptr)
		{
			(*scene)->UpdateScene(deltaTime);
			(*scene)->DrawScene();
		}
		else
		{
			Console::Log(LogType::Warning) << "Deleted a stray nullptr scene in scene vector\n";
			scenes.erase(scene);
		}	
	}

	// Update the window with the next frame
	window.Update();
}

bool Game::AddScene(Scene *pScene)
{
	if (pScene == nullptr)
		return false;

	std::vector<Scene*>::iterator scene;

	for (scene = scenes.begin(); scene != scenes.end(); scene++)
	{
		if (*scene == pScene)
		{
			Console::Log(LogType::Error) << "Scene already exists in scene vector!\n";
			return false;
		}
			
	}

	Console::Log(LogType::Success) << "Added scene\n";

	scenes.push_back(pScene);
	return true;
}

bool Game::RemoveScene(Scene *pScene)
{
	if (pScene == nullptr)
		return false;

	std::vector<Scene*>::iterator scene;

	for (scene = scenes.begin(); scene != scenes.end(); scene++)
	{
		if (*scene == pScene)
		{
			scenes.erase(scene);
			delete pScene;

			Console::Log(LogType::Success) << "Scene removed from scene vector and deleted!\n";

			return true;
		}
			
	}

	Console::Log(LogType::Error) << "Couldn't remove unknown scene from scene vector!\n";
	return false;
}

bool Game::AddRemoveScene(Scene *pScene, Scene *pNewScene)
{
	// Return false if either scene is nullptr
	if (pScene == nullptr || pNewScene == nullptr)
		return false;

	// Return false if both pointers are the same
	if (pScene == pNewScene)
		return false;

	std::vector<Scene*>::iterator scene;

	// Find scene to be replaced in vector
	// TO-DO: Prevent new scene being added if it's already in vector
	for (scene = scenes.begin(); scene != scenes.end(); scene++)
	{
		if (*scene == pScene)
		{
			*scene = pNewScene;
			delete pScene;

			Console::Log(LogType::Success) << "Scene removed/deleted from scene vector and replaced!\n";

			return true;
		}
	}

	Console::Log(LogType::Error) << "Couldn't find scene to replace in scene vector!\n";

	return false;
}
