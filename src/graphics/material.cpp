#include "material.h"

Texture2D* const Material::diffuseMap_default = new Texture2D("missingtexture2.tga");
unsigned int Material::materialCount = 0;

Material::Material() :
	specularity(0), 
	glossiness(0), 
	diffuseMap(diffuseMap_default),
	normalMap(nullptr),
	specularMap(nullptr)
{
	//diffuseMap = diffuseMap_default;
	materialCount++;
}

Material::~Material()
{
	materialCount--;

	//if (materialCount == 0)
	//	delete diffuseMap_default;
}

void Material::SetDiffuseMap(Texture2D* texture)
{
	diffuseMap = texture;
}