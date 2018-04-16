#include "cubemap.h"
#include "bitmap.h"
#include <utilities/console.h>

Cubemap::Cubemap(const std::string &imageFileName) :
	Texture(GL_TEXTURE_CUBE_MAP)
{
	Bitmap bitmap(imageFileName, true);

	unsigned short faceWidth = bitmap.width / 4;
	unsigned short faceHeight = bitmap.height / 3;
	unsigned char  bytesPerPixel = bitmap.bitsPerPixel / 8;

	if (faceWidth != faceHeight)
	{
		Console::Log(LogType::Error)
			<< "Cubemap texture dimensions are invalid.\n"
			<< "Please ensure the width is a power of 2 and the height is 3/4 the width.\n"
			<< "e.g. 2048 x 1536 or 4096 x 3072\n";
		return;
	}

	unsigned int faceDataLength = faceWidth * faceHeight * bytesPerPixel;	// Number of bytes to store all pixels of cubemap face
	unsigned int faceLineLength = faceWidth * bytesPerPixel;				// Number of bytes to store horizontal line of pixels of cubemap face
	char* faceData = new char[faceDataLength];

	glGenTextures(1, &hTexture);
	glBindTexture(target, hTexture);

	// Extract individual faces from cubemap image and store them in to seperate glTexture cubemap faces
	// Bottom
	ImportCubemapFace(bitmap.pixelData, faceData, (faceDataLength * 0) + (faceLineLength * 1), faceDataLength, faceLineLength, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, faceWidth, faceHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);

	// Left
	ImportCubemapFace(bitmap.pixelData, faceData, (faceDataLength * 4) + (faceLineLength * 0), faceDataLength, faceLineLength, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, faceWidth, faceHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);

	// Front
	ImportCubemapFace(bitmap.pixelData, faceData, (faceDataLength * 4) + (faceLineLength * 1), faceDataLength, faceLineLength, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, faceWidth, faceHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);

	// Right
	ImportCubemapFace(bitmap.pixelData, faceData, (faceDataLength * 4) + (faceLineLength * 2), faceDataLength, faceLineLength, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, faceWidth, faceHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);

	// Back
	ImportCubemapFace(bitmap.pixelData, faceData, (faceDataLength * 4) + (faceLineLength * 3), faceDataLength, faceLineLength, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, faceWidth, faceHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);
	
	// Top
	ImportCubemapFace(bitmap.pixelData, faceData, (faceDataLength * 8) + (faceLineLength * 1), faceDataLength, faceLineLength, true);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, faceWidth, faceHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, faceData);

	delete[] faceData;

	// Set cubemap parameters
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Cubemap::ImportCubemapFace(
	char*& cubemapSrc,
	char*& faceDst,
	unsigned int offset,
	unsigned int faceDataLength,
	unsigned int faceLineLength,	
	const bool& flipVertical)
{
	// Reads the bytes from cubemap in vertical reverse order. Necessary for OpenGL cubemaps
	if (flipVertical == true)
	{
		unsigned int j = 0;
		for (int i = faceDataLength - faceLineLength; i > -1; i -= faceLineLength)
		{
			memcpy(&faceDst[j], &cubemapSrc[(i * 4) + offset], faceLineLength);
			j += faceLineLength;
		}
	}

	// Reads the bytes from cubemap normally
	else
	{
		for (unsigned int i = 0; i < faceDataLength; i += faceLineLength)
		{		
			memcpy(&faceDst[i], &cubemapSrc[(i * 4) + offset], faceLineLength);
		}
	}
}