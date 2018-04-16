#pragma once
#include <string>
#include <GL/glew.h>

class Texture
{
public:
	virtual ~Texture();

	void Bind(unsigned int unit);

	unsigned short GetWidth() const { return width; }
	unsigned short GetHeight() const { return height; }
	unsigned char GetBitDepth() const { return bitsPerPixel; }

protected:
	Texture(unsigned int textureTarget);

	unsigned int hTexture;
	unsigned int target;

	unsigned short width;
	unsigned short height;
	unsigned char  bitsPerPixel;
};