#pragma once

#include <unordered_map>

#include "window.h"
#include <utilities\controls.h>

// Graphics
#include "renderable/Skybox.h"
#include "renderable/renderable.h"
#include "camera.h"
#include "campersp.h"
#include "cameraortho.h"

// Lights
#include "LightSource.h"
#include "lightomni.h"

// Audio
#include <audio/AudioSource.h>

// Base scene
class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual bool InitialiseScene(Window &window) = 0;
	virtual bool InitialiseScene(Window &window, std::string sceneName);
	virtual void ProcessInput(float deltaTime);
	virtual void UpdateScene(float deltaTime);
	virtual void DrawScene();

	void AddRenderable(Renderable *pRenderable);
	void AddLight(LightSource *pLight);
	void AddSound(AudioSource *pSound);
	void AddShader(Shader *pShader);
	void AddCamera(Camera *pCamera);

	Renderable* GetRenderable(const std::string &renderableName);
	LightSource* GetLight(const std::string &lightName);
	AudioSource* GetSound(const std::string &soundName);
	Shader* GetShader(const std::string &shaderName);
	Camera* GetCamera(const char &cameraID);

	void RemoveRenderable(const std::string &renderableName);
	void RemoveLight(const std::string &lightName);
	void RemoveSound(const std::string &soundName);
	void RemoveShader(const std::string &shaderName);
	void RemoveCamera(const char &cameraID);

	void PrintRenderableList();
	void PrintLightList();
	void PrintSoundList();
	void PrintCameraList();

	void SetSkybox(const std::string &skyName);

	void SetActiveCamera(const char &cameraID);

	Camera& GetActiveCamera();

protected:
	Controls controls;

	std::unordered_map<std::string, Renderable*> sceneRenderables;
	std::unordered_map<std::string, LightSource*> sceneLights;
	std::unordered_map<std::string, AudioSource*> sceneSounds;
	std::unordered_map<std::string, Shader*> sceneShaders;
	//std::unordered_map<unsigned char, Camera*>   sceneCameras;

	std::vector<Camera*> sceneCameras;

	std::string sceneName;

	// Maximum number of lights allowed in the scene
	// Corresponds with MAX_LIGHT_SOURCES macro defined in basicPhong shader source
	const int maxLightSources;

	char activeCameraID;

	Skybox *pSceneSky;

	Mat4 vpMatrix;

	Window *pWindow;
};
