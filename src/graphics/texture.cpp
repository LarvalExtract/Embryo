#include "texture.h"
#include <utilities/console.h>

Texture::Texture() :
	hTexture(0),
	target(0),
	width(0),
	height(0),
	bpp(0)
{

}

Texture::~Texture()
{
	glBindTexture(target, 0);
	glDeleteTextures(1, &hTexture);
}

void Texture::Bind(unsigned int unit)
{
	if (unit < 0 || unit > 31)
		unit = 0;

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(target, hTexture);
}

void Texture::GetTGAInfo(
	std::ifstream &tgaFile,
	unsigned short &width,
	unsigned short &height,
	unsigned char &bpp)
{
	// Get TGA file header
	char header[18];
	tgaFile.read(header, 18);

	memcpy(&width, &header[12], 2);
	memcpy(&height, &header[14], 2);
	memcpy(&bpp, &header[16], 1);
}

bool Texture::ImportTGAFile(
	const std::string& filePath,
	char*& imageData,
	unsigned int& imageDataLength)
{
	std::ifstream file("res/bitmaps/" + filePath, std::ios::binary);

	if (!file.is_open())
	{
		Console::Log(LogType::Error) << "Couldn't open " << filePath << "\n";
		return false;
	}

	GetTGAInfo(file, width, height, bpp);

	// Store TGA image data in to memory
	imageDataLength = width * height * (bpp / 8);
	imageData = new char[imageDataLength];

	file.read(imageData, imageDataLength);
	file.close();

	return true;
}