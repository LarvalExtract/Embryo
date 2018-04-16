#include "Skybox.h"

Skybox::Skybox(const std::string &skyboxName, float size) :
	Renderable(GL_QUADS)
{
	name = skyboxName;

	float positions[] = {
	//    X		 Y		Z
		-size,  size, -size,		// 0
		 size,  size, -size,		// 1
		 size,  size,  size,		// 2
		-size,  size,  size,		// 3	 													 	
		-size, -size, -size,		// 4
		 size, -size, -size,		// 5
		 size, -size,  size,		// 6
		-size, -size,  size			// 7
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
	vao.AddBuffer(positions, sizeof(positions), 0, 3);
	vao.AddIndices(indices, 24);

	// Use default sky shader
	shader = new Shader("sky");

	// Apply cubemap texture to skybox
	skyTexture = new Cubemap("sky/" + skyboxName);
}

void Skybox::Draw(Camera &camera, Mat4 &cameraMatrix)
{
	camera.viewMatrix.NullTranslate();	// Remove translations from camera view matrix to prevent skybox from moving against the camera
	
	vao.Bind();
	shader->Bind();
	shader->SetUniformMat4("mvpMatrix", camera.viewMatrix * camera.projectionMatrix);
	skyTexture->Bind(0);
	vao.DrawElements(renderMode);
}