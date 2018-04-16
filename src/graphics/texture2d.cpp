#include "texture2d.h"
#include "bitmap.h"

Texture2D::Texture2D(const std::string &imageFileName) :
	Texture(GL_TEXTURE_2D)
{
	Bitmap bitmap(imageFileName, true);
	this->width = bitmap.width;
	this->height = bitmap.height;
	this->bitsPerPixel = bitmap.bitsPerPixel;

	glGenTextures(1, &hTexture);
	glBindTexture(target, hTexture);

	if (bitmap.bitsPerPixel == 24)
		glTexImage2D(target, 0, GL_RGB, bitmap.width, bitmap.height, 0, GL_BGR, GL_UNSIGNED_BYTE, bitmap.pixelData);

	else if (bitmap.bitsPerPixel == 32)
		glTexImage2D(target, 0, GL_RGBA, bitmap.width, bitmap.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap.pixelData);

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture2D::Texture2D(unsigned short width, unsigned short height, unsigned char* &data) :
	Texture(GL_TEXTURE_2D)
{
	this->width = width;
	this->height = height;

	glGenTextures(1, &hTexture);
	glBindTexture(target, hTexture);

	glTexImage2D(target, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}