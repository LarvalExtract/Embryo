#include "Scene.h"

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
		std::cout
			<< "Error: No cameras found in " << sceneName << ".\n"
			<< "Your scene must have at least 1 camera" << std::endl;

		return false;
	}

	activeCameraID = 0;

	// Print a warning if the skybox does not exist
	if (pSceneSky == nullptr)
		std::cout << "Warning: No skybox initialised in " << sceneName << "\n" << std::endl;

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
	alListener3f(AL_POSITION, sceneCameras[activeCameraID]->GetPos().x, sceneCameras[activeCameraID]->GetPos().y, sceneCameras[activeCameraID]->GetPos().z);

	// Attenuate sounds in scene
	for (sound_it = sceneSounds.begin(); sound_it != sceneSounds.end(); sound_it++)
		sound_it->second->Attenuate();

	sceneShaders["basicPhong"]->Bind();
	sceneShaders["basicPhong"]->SetUniformFloat("ambience", 0.5f);

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
	vpMatrix = sceneCameras[activeCameraID]->GetViewMatrix() * sceneCameras[activeCameraID]->GetProjectionMatrix();

	// Draw renderable objects in scene
	for (renderable_it = sceneRenderables.begin(); renderable_it != sceneRenderables.end(); renderable_it++)
	{
		// Update shader matrices
		renderable_it->second->GetShader().Bind();
		renderable_it->second->GetShader().SetUniformMat4("transformMatrix", renderable_it->second->GetModelMatrix());
		renderable_it->second->GetShader().SetUniformMat4("viewMatrix", sceneCameras[activeCameraID]->GetViewMatrix());
		renderable_it->second->GetShader().SetUniformMat4("mvpMatrix", renderable_it->second->GetModelMatrix() * vpMatrix);

		// Update shader lighting components
		renderable_it->second->GetShader().SetUniformFloat("specularity", renderable_it->second->GetSpecularity());
		renderable_it->second->GetShader().SetUniformFloat("glossiness", renderable_it->second->GetGlossiness());

		renderable_it->second->Draw();
	}

	// Draw scene's skybox
	if (pSceneSky != nullptr)
	{
		sceneCameras[activeCameraID]->viewMatrix.NullTranslate();	// Remove translations from camera view matrix to prevent skybox from moving against camera

		pSceneSky->GetShader().Bind();
		pSceneSky->GetShader().SetUniformMat4("mvpMatrix", pSceneSky->GetModelMatrix() * sceneCameras[activeCameraID]->GetViewMatrix() * sceneCameras[activeCameraID]->GetProjectionMatrix());
		pSceneSky->Draw();
	}
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
		std::cout << "Error: Couldn't find " << renderableName << " in " << sceneName << std::endl;
	
	else
		sceneRenderables.erase(renderableName);
}

void Scene::RemoveLight(const std::string &lightName)
{
	if (sceneLights.find(lightName) == sceneLights.end())
		std::cout << "Error: Couldn't find " << lightName << " in " << sceneName << std::endl;
	else
		sceneRenderables.erase(lightName);
}

void Scene::RemoveSound(const std::string &soundName)
{
	if (sceneSounds.find(soundName) == sceneSounds.end())
		std::cout << "Error: Couldn't find " << soundName << std::endl;

	else
		sceneRenderables.erase(soundName);
}

void Scene::RemoveShader(const std::string &shaderName)
{
	if (sceneShaders.find(shaderName) == sceneShaders.end())
		std::cout << "Error: Couldn't find " << shaderName << " in " << sceneName << std::endl;

	else
		sceneShaders.erase(shaderName);
}

void Scene::RemoveCamera(const char &cameraID)
{
	if (sceneCameras.size() < cameraID)
		std::cout << "Error: Couldn't find camera \'" << cameraID << "\' in " << sceneName << std::endl;

	else
		sceneCameras.erase(sceneCameras.begin() + cameraID);
}

void Scene::PrintRenderableList()
{
	std::cout << sceneRenderables.size() << " renderables in " << sceneName << std::endl;

	for (renderable_it = sceneRenderables.begin(); renderable_it != sceneRenderables.end(); renderable_it++)
		std::cout << "\t" << renderable_it->first << "\n";

	std::cout << std::endl;
}

void Scene::PrintLightList()
{
	std::cout << sceneLights.size() << " lights in " << sceneName << std::endl;

	for (light_it = sceneLights.begin(); light_it != sceneLights.end(); light_it++)
		std::cout << "\t" << light_it->first << "\n";

	std::cout << std::endl;
}

void Scene::PrintSoundList()
{
	std::cout << sceneSounds.size() << " sounds in " << sceneName << std::endl;

	for (sound_it = sceneSounds.begin(); sound_it != sceneSounds.end(); sound_it++)
		std::cout << "\t" << sound_it->first << "\n";

	std::cout << std::endl;
}

void Scene::PrintCameraList()
{
	std::cout << sceneCameras.size() << " cameras in " << sceneName << std::endl;

	for (unsigned char i = 0; i < sceneCameras.size(); i++)
	{
		if (i == 0)
			std::cout << "\tCamera_" << std::to_string(i) << " (Default)\n";

		else
			std::cout << "\tCamera_" << std::to_string(i) << "\n";
	}	

	std::cout << std::endl;
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