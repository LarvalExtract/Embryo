#pragma once

#include "texture2d.h"

class Material
{
public:
	Material();
	~Material();

	float specularity;
	float glossiness;

	Texture2D *diffuseMap;
	Texture2D *normalMap;
	Texture2D *specularMap;

	void SetDiffuseMap(Texture2D* texture);
	void SetNormalMap(const std::string &textureName);
	void SetSpecularMap(const std::string &textureName);

private:
	static Texture2D* const diffuseMap_default;
	static unsigned int materialCount;
};