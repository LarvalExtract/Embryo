#pragma once

#include <unordered_map>

// Graphics
#include <graphics/renderable/renderable.h>
#include <graphics/renderable/skybox3d.h>

// Lights
#include "LightSource.h"
#include "lightomni.h"

#include "cameraperspective.h"
#include "cameraortho.h"
#include "Shader.h"
#include <audio/AudioSource.h>

class Scene
{
public:
	Scene();
	~Scene();

	bool InitialiseScene();
	bool InitialiseScene(std::string sceneName);
	void UpdateScene();
	void DrawScene();

	void AddRenderable(Renderable *pRenderable);
	void AddLight(LightSource *pLight);
	void AddSound(AudioSource *pSound);
	void AddShader(Shader *pShader);
	void AddCamera(Camera *pCamera);

	Renderable* GetRenderable(const std::string &renderableName);
	LightSource*  GetLight(const std::string &lightName);
	AudioSource*  GetSound(const std::string &soundName);
	Shader*	   GetShader(const std::string &shaderName);
	Camera*	   GetCamera(const char &cameraID);

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

private:
	std::unordered_map<std::string, Renderable*>   sceneRenderables;
	std::unordered_map<std::string, LightSource*>  sceneLights;
	std::unordered_map<std::string, AudioSource*>  sceneSounds;
	std::unordered_map<std::string, Shader*>	   sceneShaders;
	//std::unordered_map<unsigned char, Camera*>   sceneCameras;

	std::vector<Camera*> sceneCameras;

	std::unordered_map<std::string, Renderable*>::iterator    renderable_it;
	std::unordered_map<std::string, LightSource*>::iterator   light_it;
	std::unordered_map<std::string, AudioSource*>::iterator   sound_it;
	std::unordered_map<std::string, Shader*>::iterator		  shader_it;
	//std::unordered_map<std::string, Camera*>::iterator		camera_it;

	std::string sceneName;

	char activeCameraID;

	Skybox3D *pSceneSky;

	Mat4 vpMatrix;
};
