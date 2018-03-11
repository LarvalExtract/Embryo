#include "texture.h"
#include <iostream>
#include <fstream>
#include <GL/glew.h>

// Base class methods
void Texture::Bind(unsigned int unit)
{
	if (unit < 0 || unit > 31)
		unit = 0;

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(target, hTexture);
}

void Texture::SetSpecularity(const float& value)
{
	specularity = value;
}

void Texture::SetGlossiness(const float& value)
{
	glossiness = value;
}

float Texture::GetSpecularity()
{
	return specularity;
}

float Texture::GetGlossiness()
{
	return glossiness;
}

// Texture2D sub-class methods
Texture2D::Texture2D() :
	Texture()
{
}

Texture2D::Texture2D(const std::string& filePath) :
	Texture()
{
	Init(filePath);
}

Texture2D::~Texture2D()
{
	glBindTexture(target, 0);
	glDeleteTextures(1, &hTexture);
}

void Texture2D::Init(const std::string& filePath)
{
	char* imageData = nullptr;
	unsigned int imageDataLength;

	target = GL_TEXTURE_2D;

	glGenTextures(1, &hTexture);
	glBindTexture(target, hTexture);

	ImportBitmap(filePath, imageData, imageDataLength, width, height, bpp);

	if (bpp == 24)
		glTexImage2D(target, 0, GL_RGB,  width, height, 0, GL_BGR,  GL_UNSIGNED_BYTE, imageData);

	if (bpp == 32)
		glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, imageData);

	delete[] imageData;

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

// Cubemap sub-class methods
Cubemap::Cubemap() :
	Texture()
{
}

Cubemap::Cubemap(const std::string& filePath) :
	Texture()
{
	Init(filePath);
}

Cubemap::~Cubemap()
{
	glBindTexture(target, 0);
	glDeleteTextures(1, &hTexture);
}

void Cubemap::Init(const std::string& filePath)
{
	std::ifstream file("res/bitmaps/" + filePath, std::ios::binary);

	if (!file.is_open())
	{
		std::cout << "Error: Couldn't open " << filePath << std::endl;
		return;
	}

	char header[5];
	file.seekg(12, std::ios::beg);
	file.read(header, 5);

	memcpy(&width,  &header[0], 2);
	memcpy(&height, &header[2], 2);
	memcpy(&bpp,	&header[4], 1);

	width  /= 4;
	height /= 3;
	bpp	   /= 8;

	if (width != height)
	{
		std::cout
			<< "Error: Cubemap texture dimensions are invalid." << "\n"
			<< "Please ensure the width is a power of 2 and the hight is 3/4 the width." << "\n"
			<< "e.g. 2048 x 1536 or 4096 x 3072" << std::endl;
		return;
	}

	unsigned int faceDataLength = width * height * bpp;
	char* faceData = new char[faceDataLength];

	target = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &hTexture);
	glBindTexture(target, hTexture);

	GetCubemapFace(file, faceData, (faceDataLength * 0) + ((width * bpp) * 1), width, height, bpp, true);			
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);		// Bottom

	GetCubemapFace(file, faceData, (faceDataLength * 4) + ((width * bpp) * 0), width, height, bpp, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);		// Left

	GetCubemapFace(file, faceData, (faceDataLength * 4) + ((width * bpp) * 1), width, height, bpp, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);		// Front

	GetCubemapFace(file, faceData, (faceDataLength * 4) + ((width * bpp) * 2), width, height, bpp, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);		// Right
		
	GetCubemapFace(file, faceData, (faceDataLength * 4) + ((width * bpp) * 3), width, height, bpp, true);	
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);		// Back

	GetCubemapFace(file, faceData, (faceDataLength * 8) + ((width * bpp) * 1), width, height, bpp, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);		// Top

	delete[] faceData;
	file.close();

	bpp *= 8;

	// Set cubemap parameters for skybox
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Cubemap::SetCubemapFace(const unsigned int& face, const char*& imageData)
{
	glTexImage2D(face, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);
}