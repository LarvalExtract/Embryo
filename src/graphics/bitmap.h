#pragma once

#include <string>

enum class FileType
{
	None = -1,
	TGA,
	PNG,
	JPEG,
	TIFF,
	DDS
};

// Enter the file path of an image file to copy a bitmap's contents to memory
// The second argument determines whether you want the actual pixel data or not
// false means only the bitmap's properties will be obtained, not the actual pixel data itself
class Bitmap
{
public:
	Bitmap(const std::string &imageFilePath, bool getPixelData);
	~Bitmap();

	unsigned short	width;
	unsigned short	height;
	unsigned char	bitsPerPixel;
	char*			pixelData;
	FileType		fileType;

private:
	bool bGetPixelData;

	void GetHeader();
	void GetData();
};