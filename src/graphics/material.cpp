#include "material.h"

Texture2D* const Material::diffuseMap_default = new Texture2D("missingtexture2.tga");

void Material::SetDiffuseMap(const std::string &textureName)
{
	diffuseMap = new Texture2D(textureName);
}