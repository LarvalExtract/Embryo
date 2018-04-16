#pragma once

#include "texture.h"

class Cubemap : public Texture
{
public:
	Cubemap(const std::string &imageFileName);

private:
	static void ImportCubemapFace(
		char*& cubemapSrc,
		char*& faceDst,
		unsigned int offset,
		unsigned int faceDataLength,
		unsigned int faceLineLength,
		const bool& flipVertical);
};