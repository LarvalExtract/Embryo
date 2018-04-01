#pragma once

#include "texture2d.h"

class Material
{
public:
	Material() : specularity(0), glossiness(0), diffuseMap(nullptr), normalMap(nullptr), specularMap(nullptr) {}

	float specularity;
	float glossiness;

	Texture2D *diffuseMap;
	Texture2D *normalMap;
	Texture2D *specularMap;

	void SetDiffuseMap(const std::string &textureName);
	void SetNormalMap(const std::string &textureName);
	void SetSpecularMap(const std::string &textureName);

private:
	static Texture2D* const diffuseMap_default;
};