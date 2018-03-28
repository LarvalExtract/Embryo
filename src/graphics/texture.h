#pragma once
#include <string>
#include <GL/glew.h>
#include <fstream>

class Texture
{
public:
	virtual ~Texture();

	void Bind(unsigned int unit);

	unsigned short GetWidth() { return width; }
	unsigned short GetHeight() { return height; }
	unsigned char GetBitDepth() { return bpp; }

protected:
	Texture();

	unsigned int hTexture;
	unsigned int target;

	unsigned short width;
	unsigned short height;
	unsigned char  bpp;

	void GetTGAInfo(
		std::ifstream &tgaFile,
		unsigned short &width,
		unsigned short &height,
		unsigned char &bpp);

	bool ImportTGAFile(
		const std::string& filePath,
		char*& imageData,
		unsigned int& imageDataLength);
};