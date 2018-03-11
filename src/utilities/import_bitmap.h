#pragma once

#include <string>
#include <fstream>

void ImportBitmap(
	const std::string& filePath,
	char*& imageData,
	unsigned int& imageDataLength,
	unsigned short& width,
	unsigned short& height,
	unsigned char& bpp);

void GetCubemapFace(
	std::ifstream& file,
	char*& face,
	unsigned int offset,
	unsigned short width,
	unsigned short height,
	unsigned short bpp,
	const bool& flipVertical);