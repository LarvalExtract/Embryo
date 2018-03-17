#include "import_bitmap.h"

#include "console.h"

#include <iostream>

void ImportBitmap(
	const std::string& filePath,
	char*& imageData,
	unsigned int& imageDataLength,
	unsigned short& width,
	unsigned short& height,
	unsigned char& bpp)
{
	std::ifstream file("res/bitmaps/" + filePath, std::ios::binary);

	if (!file.is_open())
	{
		Console::Log(LogType::Error) << "Couldn't open " << filePath << "\n";
		return;
	}
	
	char header[18];
	file.read(header, 18);

	memcpy(&width,  &header[12], 2);
	memcpy(&height, &header[14], 2);
	memcpy(&bpp,    &header[16], 1);

	imageDataLength = width * height * (bpp / 8);
	imageData = new char[imageDataLength];

	file.read(imageData, imageDataLength);
	file.close();
}

void GetCubemapFace(
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