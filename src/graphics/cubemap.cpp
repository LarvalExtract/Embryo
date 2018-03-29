#include "cubemap.h"
#include <utilities/console.h>

Cubemap::Cubemap(const std::string &imageFileName) :
	Texture()
{
	std::ifstream file("res/bitmaps/" + imageFileName, std::ios::binary);

	if (!file.is_open())
	{
		Console::Log(LogType::Error) << "Couldn't open " << imageFileName << "\n";
		return;
	}

	GetTGAInfo(file, width, height, bpp);

	unsigned short faceWidth = width / 4;
	unsigned short faceHeight = height / 3;
	unsigned char Bpp = bpp / 8;			// Bpp = Bytes per pixel, bpp = bits per pixel

	if (faceWidth != faceHeight)
	{
		Console::Log(LogType::Error)
			<< "Cubemap texture dimensions are invalid.\n"
			<< "Please ensure the width is a power of 2 and the height is 3/4 the width.\n"
			<< "e.g. 2048 x 1536 or 4096 x 3072\n";
		return;
	}

	unsigned int faceDataLength = faceWidth * faceHeight * Bpp;
	char* faceData = new char[faceDataLength];

	target = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &hTexture);
	glBindTexture(target, hTexture);

	ImportCubemapFace(file, faceData, (faceDataLength * 0) + ((faceWidth * Bpp) * 1), faceWidth, faceHeight, Bpp, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, faceWidth, faceHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);		// Bottom

	ImportCubemapFace(file, faceData, (faceDataLength * 4) + ((faceWidth * Bpp) * 0), faceWidth, faceHeight, Bpp, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, faceWidth, faceHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);		// Left

	ImportCubemapFace(file, faceData, (faceDataLength * 4) + ((faceWidth * Bpp) * 1), faceWidth, faceHeight, Bpp, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, faceWidth, faceHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);		// Front

	ImportCubemapFace(file, faceData, (faceDataLength * 4) + ((faceWidth * Bpp) * 2), faceWidth, faceHeight, Bpp, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, faceWidth, faceHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);		// Right

	ImportCubemapFace(file, faceData, (faceDataLength * 4) + ((faceWidth * Bpp) * 3), faceWidth, faceHeight, Bpp, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, faceWidth, faceHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);		// Back

	ImportCubemapFace(file, faceData, (faceDataLength * 8) + ((faceWidth * Bpp) * 1), faceWidth, faceHeight, Bpp, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, faceWidth, faceHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);		// Top

	delete[] faceData;
	file.close();

	// Set cubemap parameters
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Cubemap::ImportCubemapFace(
	std::ifstream& file,
	char*& face,
	unsigned int offset,
	unsigned short width,
	unsigned short height,
	unsigned short bpp,
	const bool& flipVertical)
{
	// Reads the bytes from cubemap in vertical reverse order. Necessary for OpenGL cubemaps
	if (flipVertical == true)
	{
		unsigned int j = 0;
		for (int i = (width * height * bpp) - (width * bpp); i > -1; i -= width * bpp)
		{
			file.seekg((i * 4) + offset + 18, std::ios::beg);
			file.read(&face[j], width * bpp);
			j += width * bpp;
		}

	}

	// Reads the bytes from cubemap normally
	else
	{
		for (unsigned int i = 0; i < width * height * bpp; i += width * bpp)
		{
			file.seekg((i * 4) + offset + 18, std::ios::beg);
			file.read(&face[i], width * bpp);
		}
	}
}