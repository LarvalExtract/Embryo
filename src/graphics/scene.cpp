#include "Scene.h"

#include <utilities/console.h>

Scene::Scene() :
	activeCameraID(-1),
	pSceneSky(nullptr),
	sceneName(""),
	maxLightSources(8)
{
}

Scene::~Scene()
{
	if (pSceneSky != nullptr)
		delete pSceneSky;

	for (unsigned char i = 0; i < sceneCameras.size(); i++)
		delete sceneCameras[i];

	for (auto renderable : sceneRenderables)
		if (renderable.second != nullptr)
			delete renderable.second;

	for (auto light : sceneLights)
		if (light.second != nullptr)
			delete light.second;

	for (auto sound : sceneSounds)
		if (sound.second != nullptr)
			delete sound.second;

	for (auto shader : sceneShaders)
		if (shader.second != nullptr)
			delete shader.second;
}

bool Scene::InitialiseScene(Window &window)
{
	if (sceneCameras.size() == 0)
	{
		Console::Log(LogType::Error) << "No cameras found in " << sceneName
			<< ". Your scene must have at least 1 camera.\n";

		return false;
	}

	//activeCameraID = 0;
	sceneShaders["basicPhong"]->Bind();
	sceneShaders["basicPhong"]->SetUniformInt("u_numLights", sceneLights.size());
	sceneShaders["basicPhong"]->SetUniformFloat("ambience", 0.4f);

	// Print a warning if the skybox does not exist
	if (pSceneSky == nullptr)
		Console::Log(LogType::Warning) << "No skybox initialised in " << sceneName << "\n";

	// Play all sounds in the scene
	for (auto sound : sceneSounds)
		sound.second->Play();

	return true;
}

bool Scene::InitialiseScene(Window &window, std::string name)
{
	// Set screen name if it doesn't have one already
	if (sceneName.empty())
		sceneName = "\'" + name + "\'";

	pWindow = &window;

	return InitialiseScene(window);
}

void Scene::ProcessInput(float deltaTime)
{
	controls.MatricesFromInputs(*pWindow, GetActiveCamera(), deltaTime);
}

void Scene::UpdateScene(float deltaTime)
{
	ProcessInput(deltaTime);

	// Update lights in shader
	int i = 0;
	for (auto light : sceneLights)
	{
		sceneShaders["basicPhong"]->Bind();
		sceneShaders["basicPhong"]->SetUniformVec3f("u_lightPosition[" + std::to_string(i) + ']', light.second->position);
		sceneShaders["basicPhong"]->SetUniformVec3f("u_lightColour[" + std::to_string(i) + ']', light.second->colour);
		sceneShaders["basicPhong"]->SetUniformFloat("u_lightPower[" + std::to_string(i) + ']', light.second->power);
		i++;
	}

	// Attenuate sounds in scene
	for (auto sound : sceneSounds)
		sound.second->Attenuate(sceneCameras[activeCameraID]->position);
}

void Scene::DrawScene()
{
	// Pre-calculate the view * projection matrix so it doesn't need to be recalculated for every renderable
	vpMatrix = sceneCameras[activeCameraID]->viewMatrix * sceneCameras[activeCameraID]->projectionMatrix;

	// Draw renderable objects in scene
	for (auto renderable : sceneRenderables)
		renderable.second->Draw(*sceneCameras[activeCameraID], vpMatrix);

	// Draw scene's skybox
	if (pSceneSky != nullptr)
		pSceneSky->Draw(*sceneCameras[activeCameraID], vpMatrix);
}

void Scene::AddRenderable(Renderable *pRenderable)
{
	sceneRenderables.insert(std::pair<std::string, Renderable*>(pRenderable->name, pRenderable));
}

void Scene::AddLight(LightSource *pLight)
{
	if (sceneLights.size() + 1 > maxLightSources)
		Console::Log(LogType::Warning) << "Cannot add more than " << maxLightSources << " lights to " << sceneName << "\n";

	else
		sceneLights.insert(std::pair<std::string, LightSource*>(pLight->name, pLight));
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

Renderable* Scene::GetRenderable(const std::string &renderableName)
{
	return sceneRenderables.at(renderableName);
}

LightSource* Scene::GetLight(const std::string &lightName)
{
	return sceneLights.at(lightName);
}

AudioSource* Scene::GetSound(const std::string &soundName)
{
	if (sceneSounds.find(soundName) == sceneSounds.end())
		Console::Log(LogType::Error) << "Couldn't find " << soundName << " in " << sceneName << "\n";

	else
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

	for (auto renderable : sceneRenderables)
		Console::Log() << "\t" << renderable.first << "\n";

	Console::Log() << "\n";
}

void Scene::PrintLightList()
{
	Console::Log() << sceneLights.size() << " lights in " << sceneName << "\n";

	for (auto light : sceneLights)
		Console::Log() << "\t" << light.first << "\n";

	Console::Log() << "\n";
}

void Scene::PrintSoundList()
{
	Console::Log() << sceneSounds.size() << " sounds in " << sceneName << "\n";

	for (auto sound : sceneSounds)
		Console::Log() << "\t" << sound.first << "\n";

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
	pSceneSky = new Skybox(skyName, 100.0f);
}

void Scene::SetActiveCamera(const char& cameraID)
{
	activeCameraID = cameraID;
}

Camera& Scene::GetActiveCamera()
{
	return *sceneCameras[activeCameraID];
}