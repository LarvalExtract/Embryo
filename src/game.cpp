#include "game.h"

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
	delete window;
	delete scene;
	
	std::cout << std::endl;
	std::cin.get();
}

void Game::Start()
{
	// Return if initialisation failed (don't start game loop)
	if (!Initialise())
		return;

	// GAME LOOP
	while (!window->Close())
	{
		ProcessInput();
		Update();
		Draw();
	}
}

bool Game::Initialise()
{
	Timer initTimer;
	// Return if already initialised
	if (bInitialised)
		return false;

	window = new Window("Embryo", 1280, 720);
	//window->SetColour(0.1f, 0.3f, 0.6f, 1.0f);

	std::cout << "OpenGL " << glGetString(GL_VERSION) << ", " << glGetString(GL_VENDOR) << ", " << glGetString(GL_RENDERER) << "\n"
			  << "OpenAL " << alGetString(AL_VERSION) << ", " << alGetString(AL_VENDOR) << ", " << alGetString(AL_RENDERER) << "\n"
			  << std::endl;

	std::cout << "Press Z to enable camera controls" << "\n" << std::endl;

	scene = new Embryo::Scene("Scene1");

	scene->SetSkybox("skybox_ocean.tga");

	Camera* camera = new Camera(Vec3<float>(0.0f, 1.0f, -3.0f), 60.0f, (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 1000.0f);
	scene->AddCamera(camera);

	Shader* shader = new Shader("basicPhong");
	scene->AddShader(shader);

	Model3D *pFloor = new Model3D("floor.mmf");
	pFloor->SetPos(0.0f, -1.0f, 0.0f);
	pFloor->SetTexture("models/floor.tga");
	pFloor->SetShader(shader);
	scene->AddRenderable(pFloor);

	Model3D *pCube = new Model3D("cube.mmf");
	pCube->SetScale(0.2f, 0.2f, 0.2f);
	pCube->SetTexture("models/cube.tga");
	pCube->SetSpecularity(0.2f);
	pCube->SetGlossiness(20.0f);
	pCube->SetShader(shader);
	scene->AddRenderable(pCube);

	Model3D *pTeapot = new Model3D("teapot.mmf");
	pTeapot->SetPos(1.2f, 0.0f, 0.0f);
	pTeapot->SetScale(0.02f, 0.02f, 0.02f);
	pTeapot->SetTexture("models/test.tga");
	pTeapot->SetSpecularity(2.0f);
	pTeapot->SetGlossiness(100.0f);
	pTeapot->SetShader(shader);
	scene->AddRenderable(pTeapot);

	Model3D *pClone = new Model3D("poo.mmf");
	pClone->SetPos(-1.0f, 0.0f, 0.0f);
	pClone->SetRot(0.0f, 180.0f, 0.0f);
	pClone->SetScale(0.02f, 0.02f, 0.02f);
	pClone->SetTexture("models/clone.tga");
	pClone->SetSpecularity(0.5f);
	pClone->SetGlossiness(30.0f);
	pClone->SetShader(shader);
	scene->AddRenderable(pClone);

	Sprite2D *pSprite = new Sprite2D("sprites/sprite_test3.tga");
	pSprite->SetPos(1.7f, 0.0f, -1.0f);
	pSprite->SetShader(shader);
	scene->AddRenderable(pSprite);

	Primitive::Sphere *pSphere = new Primitive::Sphere(24, 2.0f, Vec4<float>(1.0f, 1.0f, 0.0f, 1.0f));
	pSphere->SetPos(-5, 2, 3);
	scene->AddRenderable(pSphere);

	Primitive::Box *pBox = new Primitive::Box(2, 3, 3, Vec4<float>(1.0f, 0.0f, 1.0f, 1.0f));
	pBox->SetPos(13.3f, 3.0f, 5.0f);
	pBox->SetRot(0.0f, 23.0f, 0.0f);
	scene->AddRenderable(pBox);

	Primitive::Pivot *pPivot = new Primitive::Pivot();
	pPivot->SetPos(0.0f, 2.0f, 0.0f);
	scene->AddRenderable(pPivot);

	AudioSourceGlobal *wind = new AudioSourceGlobal("sandtrap_wind_loop.wav");
	wind->SetGain(0.3f);
	wind->SetLoop(true);
	scene->AddSound(wind);

	AudioSourceLocal *computer = new AudioSourceLocal("alien_generator.wav");
	computer->SetGain(0.55f);
	computer->SetLoop(true);
	scene->AddSound(computer);

	LightOmni *light = new LightOmni(Vec3<float>(0.0f, 2.0f, 0.0f), Vec3<float>(1.0f, 1.0f, 1.0f), 1.0f);
	scene->AddLight(light);

	LightOmni *redLight = new LightOmni(Vec3<float>(0.0f, 2.0f, 0.0f), Vec3<float>(1.0f, 1.0f, 1.0f), 1.0f);
	//scene->AddLight(redLight);

	testQuad = new Shape2D::Quad(250.0f, 250.0f);
	sprite = new Sprite("sprites/crosshair.tga");
	sprite->CenterOrigin();
	sprite->SetPosition(window->GetWidth() / 2, window->GetHeight() / 2);
	//sprite->SetPosition(0.0f, 0.0f);

	proj = mat4::Orthographic(0.0f, window->GetWidth(), 0.0f, window->GetHeight(), -1.0f, 1.0f);
	
	// Initialise the scene
	if (scene->InitialiseScene() == false)
		return false;

	//scene->PrintRenderableList();
	//scene->PrintLightList();
	//scene->PrintSoundList();
	//scene->PrintCameraList();

	std::cout << "Initialisation time: " << initTimer.Elapsed() << " seconds" << "\n" << std::endl;

	lastTime = glfwGetTime();

	return bInitialised = true;
}

// Process game inputs
void Game::ProcessInput()
{


	MatricesFromInputs(*window, scene->GetActiveCamera(), deltaTime);
}

// Update the game's states and conditions
void Game::Update()
{
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	// Display frame time/frame rate
	std::cerr << "Frame time: " << 1000 * deltaTime << "ms,\t" << static_cast<int>(1 / deltaTime) << "fps" << "                   \r";
	
	counter += 1.0f * deltaTime;

	scene->GetRenderable("teapot")->SetRot(sin(counter * 10), 0.0f, cos(counter * 20));
	scene->GetLight("light_omni")->SetPosition(sin(counter) * 2.0f, 2.0f, cos(counter) * 2.0f);
	//scene->GetLight("light_omni")->SetPower(sin(counter * 2) * 3);

	sprite->SetRotation(counter * 15.0f);
	//sprite->SetScale(cos(counter), cos(counter));

	scene->UpdateScene();
	
}

// Output frame
void Game::Draw()
{
	// Clear the current buffer
	window->Clear();

	// Draw everything in the scene
	scene->DrawScene();

	testQuad->GetShader().Bind();
	testQuad->GetShader().SetUniformMat4("projectionMatrix", proj);
	//testQuad->Draw();
	sprite->GetShader().Bind();
	sprite->GetShader().SetUniformMat4("projectionMatrix", proj);
	sprite->GetShader().SetUniformMat4("mpMatrix", sprite->GetTransformMatrix() * proj);
	sprite->Draw();


	// Update the window with the next frame
	window->Update();
}