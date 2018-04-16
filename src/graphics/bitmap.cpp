#include "bitmap.h"
#include <utilities/console.h>
#include <fstream>
#include <picopng/decodepng.h>

/*	
Generic header struct for bitmap files
This struct stores the entire header of a certain file type, as well as the offsets at which
bitmap properties are located at within the header.
This is where file-type specific code needs to go
*/

// TODO: Add support for other file formats
struct Header
{
	unsigned int widthOffset;
	unsigned int heightOffset;
	unsigned int bitsPerPixelOffset;
	unsigned int pixelDataOffset;
	char* data;
	unsigned int length;

	Header() : widthOffset(0), heightOffset(0), bitsPerPixelOffset(0), pixelDataOffset(0), data(nullptr), length(0) {};

	Header(FileType fileType)
	{
		switch (fileType)
		{
		case FileType::TGA:
			widthOffset = 12;
			heightOffset = 14;
			bitsPerPixelOffset = 16;
			pixelDataOffset = 18;	// Will need to add value in byte 0 of tga file to this
			length = 18;
			data = new char[length];
			break;

		case FileType::PNG:
			widthOffset = 16;
			heightOffset = 20;
			bitsPerPixelOffset = 21;
			break;

		case FileType::JPEG:

			break;

		case FileType::TIFF:

			break;

		case FileType::DDS:

			break;
		}
	}

	~Header()
	{
		delete data;
	}
};

FileType GetFileType(const std::string &imageFileName);

Bitmap::Bitmap(const std::string &imageFilePath, bool getPixelData) :
	width(0),
	height(0),
	bitsPerPixel(0),
	pixelData(nullptr),
	fileType(FileType::None)
{
	std::ifstream hImageFile("res/bitmaps/" + imageFilePath, std::ios::binary);

	if (!hImageFile.is_open())
	{
		Console::Log(LogType::Error) << "Couldn't open " << imageFilePath << "\n";
		return;
	}

	// Determine the file type of image file
	fileType = GetFileType(imageFilePath);
	
	// Error if the file type of the image file is incompatible
	if (fileType == FileType::None)
	{
		Console::Log(LogType::Error) << imageFilePath << " is not a supported bitmap file type.\n";
		return;
	}

	// Construct file-type specific header from file
	Header header(fileType);

	// Copy the header from the image file
	hImageFile.seekg(0, std::ios::beg);
	hImageFile.read(header.data, header.length);

	// Retrieve width, height and bit depth of image file
	memcpy(&width, &header.data[header.widthOffset], 2);
	memcpy(&height, &header.data[header.heightOffset], 2);
	memcpy(&bitsPerPixel, &header.data[header.bitsPerPixelOffset], 1);

	// Retrieve pixels from image file only if argument is true
	if (getPixelData == true)
	{
		// Allocate memory for bitmap pixel data
		unsigned int pixelDataLength = width * height * (bitsPerPixel / 8);
		pixelData = new char[pixelDataLength];

		// Copy bitmap pixel data from file to memory
		hImageFile.seekg(header.pixelDataOffset, std::ios::beg);
		hImageFile.read(pixelData, pixelDataLength);
	}

	hImageFile.close();
}

void Bitmap::GetHeader()
{

}

void Bitmap::GetData()
{
	switch (fileType)
	{
	case FileType::TGA:

		break;

	case FileType::PNG:

		break;
	}
}

Bitmap::~Bitmap()
{
	if (pixelData != nullptr)
		delete pixelData;
}

FileType GetFileType(const std::string &imageFilePath)
{
	std::string fileExtension = imageFilePath.substr(imageFilePath.rfind("."), imageFilePath.length());
	
	if (fileExtension == ".tga")
		return FileType::TGA;

	else if (fileExtension == ".png")
		return FileType::PNG;

	else if (fileExtension == ".jpeg")
		return FileType::JPEG;

	else if (fileExtension == ".tif")
		return FileType::TIFF;

	else if (fileExtension == ".dds")
		return FileType::DDS;

	else
		return FileType::None;
}