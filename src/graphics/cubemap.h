#pragma once

#include "texture.h"

class Cubemap : public Texture
{
public:
	Cubemap(const std::string &imageFileName);

private:
	void ImportCubemapFace(
		std::ifstream& file,
		char*& face,
		unsigned int offset,
		unsigned short width,
		unsigned short height,
		unsigned short bpp,
		const bool& flipVertical);
};