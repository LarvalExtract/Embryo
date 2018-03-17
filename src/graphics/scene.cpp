#include "Scene.h"

#include <utilities/console.h>

Scene::Scene() :
	activeCameraID(-1),
	pSceneSky(nullptr),
	sceneName("Unnamed")
{
}

Scene::~Scene()
{
	if (pSceneSky != nullptr)
		delete pSceneSky;

	for (unsigned char i = 0; i < sceneCameras.size(); i++)
		delete sceneCameras[i];

	for (renderable_it = sceneRenderables.begin(); renderable_it != sceneRenderables.end(); renderable_it++)
		if (renderable_it->second != nullptr)
			delete renderable_it->second;

	for (light_it = sceneLights.begin(); light_it != sceneLights.end(); light_it++)
		if (light_it->second != nullptr)
			delete light_it->second;

	for (sound_it = sceneSounds.begin(); sound_it != sceneSounds.end(); sound_it++)
		if (sound_it->second != nullptr)
			delete sound_it->second;

	for (shader_it = sceneShaders.begin(); shader_it != sceneShaders.end(); shader_it++)
		if (shader_it->second != nullptr)
			delete shader_it->second;
}

bool Scene::InitialiseScene()
{
	if (sceneCameras.size() == 0)
	{
		Console::Log(LogType::Error) << "No cameras found in " << sceneName
			<< ". Your scene must have at least 1 camera.\n";

		return false;
	}

	//activeCameraID = 0;

	// Print a warning if the skybox does not exist
	if (pSceneSky == nullptr)
		Console::Log(LogType::Warning) << "No skybox initialised in " << sceneName << "\n";

	// Play all sounds in the scene
	for (sound_it = sceneSounds.begin(); sound_it != sceneSounds.end(); sound_it++)
		sound_it->second->Play();

	return true;
}

bool Scene::InitialiseScene(std::string name)
{
	// Set screen name if it doesn't have one already
	if (sceneName.empty())
		sceneName = "\'" + name + "\'";

	return InitialiseScene();
}

void Scene::UpdateScene()
{
	alListener3f(AL_POSITION, sceneCameras[activeCameraID]->position.x, sceneCameras[activeCameraID]->position.y, sceneCameras[activeCameraID]->position.z);

	// Attenuate sounds in scene
	for (sound_it = sceneSounds.begin(); sound_it != sceneSounds.end(); sound_it++)
		sound_it->second->Attenuate();

	sceneShaders["basicPhong"]->Bind();
	sceneShaders["basicPhong"]->SetUniformFloat("ambience", 0.4f);

	// Update lights in shader
	for (light_it = sceneLights.begin(); light_it != sceneLights.end(); light_it++)
	{
		sceneShaders["basicPhong"]->Bind();
		sceneShaders["basicPhong"]->SetUniformVec3f("lightPosition", light_it->second->GetPosition());
		sceneShaders["basicPhong"]->SetUniformVec3f("lightColour", light_it->second->GetColour());
		sceneShaders["basicPhong"]->SetUniformFloat("lightPower", light_it->second->GetPower());
	}

}

void Scene::DrawScene()
{
	// Pre-calculate the view * projection matrix so it doesn't need to be recalculated for every renderable
	vpMatrix = sceneCameras[activeCameraID]->viewMatrix * sceneCameras[activeCameraID]->projectionMatrix;

	// Draw renderable objects in scene
	for (renderable_it = sceneRenderables.begin(); renderable_it != sceneRenderables.end(); renderable_it++)
		renderable_it->second->Draw(*sceneCameras[activeCameraID], vpMatrix);

	// Draw scene's skybox
	if (pSceneSky != nullptr)
		pSceneSky->Draw(*sceneCameras[activeCameraID], vpMatrix);
}

void Scene::AddRenderable(Renderable3D *pRenderable)
{
	sceneRenderables.insert(std::pair<std::string, Renderable3D*>(pRenderable->GetName(), pRenderable));
}

void Scene::AddLight(LightSource *pLight)
{
	sceneLights.insert(std::pair<std::string, LightSource*>(pLight->GetName(), pLight));
}

void Scene::AddSound(AudioSource *pSound)
{
	sceneSounds.insert(std::pair<std::string, AudioSource*>(pSound->GetName(), pSound));
}

void Scene::AddShader(Shader *pShader)
{
	sceneShaders.insert(std::pair<std::string, Shader*>(pShader->GetName(), pShader));
}

void Scene::AddCamera(Camera *pCamera)
{
	sceneCameras.push_back(pCamera);
}

Renderable3D* Scene::GetRenderable(const std::string &renderableName)
{
	return sceneRenderables.at(renderableName);
}

LightSource* Scene::GetLight(const std::string &lightName)
{
	return sceneLights.at(lightName);
}

AudioSource* Scene::GetSound(const std::string &soundName)
{
	return sceneSounds.at(soundName);
}

Shader* Scene::GetShader(const std::string &shaderName)
{
	return sceneShaders.at(shaderName);
}

Camera* Scene::GetCamera(const char& cameraID)
{
	return sceneCameras.at(cameraID);
}

void Scene::RemoveRenderable(const std::string &renderableName)
{
	if (sceneRenderables.find(renderableName) == sceneRenderables.end())
		Console::Log(LogType::Error) << "Couldn't find " << renderableName << " in " << sceneName << "\n";
	
	else
		sceneRenderables.erase(renderableName);
}

void Scene::RemoveLight(const std::string &lightName)
{
	if (sceneLights.find(lightName) == sceneLights.end())
		Console::Log(LogType::Error) << "Couldn't find " << lightName << " in " << sceneName << "\n";
	else
		sceneRenderables.erase(lightName);
}

void Scene::RemoveSound(const std::string &soundName)
{
	if (sceneSounds.find(soundName) == sceneSounds.end())
		Console::Log(LogType::Error) << "Couldn't find " << soundName << "\n";

	else
		sceneRenderables.erase(soundName);
}

void Scene::RemoveShader(const std::string &shaderName)
{
	if (sceneShaders.find(shaderName) == sceneShaders.end())
		Console::Log(LogType::Error) << "Couldn't find " << shaderName << " in " << sceneName << "\n";

	else
		sceneShaders.erase(shaderName);
}

void Scene::RemoveCamera(const char &cameraID)
{
	if (sceneCameras.size() < cameraID)
		Console::Log(LogType::Error) << "Couldn't find camera \'" << cameraID << "\' in " << sceneName << "\n";

	else
		sceneCameras.erase(sceneCameras.begin() + cameraID);
}

void Scene::PrintRenderableList()
{
	Console::Log() << sceneRenderables.size() << " renderables in " << sceneName << "\n";

	for (renderable_it = sceneRenderables.begin(); renderable_it != sceneRenderables.end(); renderable_it++)
		Console::Log() << "\t" << renderable_it->first << "\n";

	Console::Log() << "\n";
}

void Scene::PrintLightList()
{
	Console::Log() << sceneLights.size() << " lights in " << sceneName << "\n";

	for (light_it = sceneLights.begin(); light_it != sceneLights.end(); light_it++)
		Console::Log() << "\t" << light_it->first << "\n";

	Console::Log() << "\n";
}

void Scene::PrintSoundList()
{
	Console::Log() << sceneSounds.size() << " sounds in " << sceneName << "\n";

	for (sound_it = sceneSounds.begin(); sound_it != sceneSounds.end(); sound_it++)
		Console::Log() << "\t" << sound_it->first << "\n";

	Console::Log() << "\n";
}

void Scene::PrintCameraList()
{
	Console::Log() << sceneCameras.size() << " cameras in " << sceneName << "\n";

	for (unsigned char i = 0; i < sceneCameras.size(); i++)
	{
		if (i == 0)
			Console::Log() << "\tCamera_" << std::to_string(i) << " (Default)\n";

		else
			Console::Log() << "\tCamera_" << std::to_string(i) << "\n";
	}	

	Console::Log() << "\n";
}

// TO-DO: Fix potential memory leak due to not cleaning up previous skyboxes
void Scene::SetSkybox(const std::string &skyName)
{
	pSceneSky = new Skybox(skyName);
}

void Scene::SetActiveCamera(const char& cameraID)
{
	activeCameraID = cameraID;
}

Camera& Scene::GetActiveCamera()
{
	return *sceneCameras[activeCameraID];
}