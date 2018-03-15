#include "skybox.h"
#include "shader.h"

// Create skybox using name of skybox texture
// Default directory: res/bitmaps/sky/
Skybox::Skybox(const std::string& skyboxName) :
	Renderable3D(GL_QUADS), scale(100)
{
	name = skyboxName;

	float positions[] = {
		//   X		 Y		 Z
		-scale,  scale, -scale,		// 0
		scale,  scale, -scale,		// 1
		scale,  scale,  scale,		// 2
		-scale,  scale,  scale,		// 3	 													 	
		-scale, -scale, -scale,		// 4
		scale, -scale, -scale,		// 5
		scale, -scale,  scale,		// 6
		-scale, -scale,  scale		// 7
	};

	unsigned short indices[] = {
		0, 1, 2, 3,		// Top,		Y+
		7, 4, 0, 3,		// Left,	X-
		6, 7, 3, 2,		// Front,	Z-
		5, 6, 2, 1,		// Right,	X+
		4, 5, 1, 0,		// Back,	Z+
		7, 6, 5, 4		// Bottom,	Y-
	};

	// Send skybox vertex data to GPU
	vao.AddBuffers(positions, sizeof(positions), 0, 3);
	vao.AddIndices(indices, 24);

	// Use default sky shader
	shader = new Shader("sky");

	// Apply cubemap texture to skybox
	skyTexture = new Cubemap("sky/" + skyboxName);
}

// Doesn't need to pass vpMatrix
void Skybox::Draw(Camera &camera, Mat4 &vpMatrix)
{
	vao.Bind();
	shader->Bind();

	// Remove translations from camera view matrix to prevent skybox from moving against camera
	camera.viewMatrix.NullTranslate();

	shader->SetUniformMat4("mvpMatrix", GetModelMatrix() * camera.GetViewMatrix() * camera.GetProjectionMatrix());

	skyTexture->Bind(0);
	vao.DrawElements(renderMode);
}
