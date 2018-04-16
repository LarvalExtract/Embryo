#pragma once

#include "texture.h"

class Texture2D : public Texture
{
public:
	// Create a 2D texture from an image file
	Texture2D(const std::string &imageFileName);

	// Create a 2D texture from a specified width, height, and raw bitmap data
	Texture2D(unsigned short width, unsigned short height, unsigned char* &data);

private:

};