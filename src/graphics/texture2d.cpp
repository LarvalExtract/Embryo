#include "texture2d.h"

Texture2D::Texture2D(const std::string &imageFileName) :
	Texture()
{
	char* imageData = nullptr;
	unsigned int imageDataLength;

	target = GL_TEXTURE_2D;

	glGenTextures(1, &hTexture);
	glBindTexture(target, hTexture);

	if (!ImportTGAFile(imageFileName, imageData, imageDataLength))
		return;

	if (bpp == 24)
		glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);

	else if (bpp == 32)
		glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, imageData);

	delete[] imageData;

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
}