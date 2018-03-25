#pragma once

#include "renderable.h"
#include <graphics/texture.h>
#include <vector>

class Model3D : public Renderable
{
public:
	Model3D();
	~Model3D();

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;

	void SetDiffuseTexture(const std::string &fileName);
	void SetNormalMap(const std::string &fileName);
	void SetSpecularMap(const std::string &fileName);

	float specularity;
	float glossiness;

protected:
	Texture2D *diffuseMap;
	Texture2D *normalMap;		// Currently unused
	Texture2D *specularMap;		// Currently unused

	static Texture2D* const defaultTexture;
};