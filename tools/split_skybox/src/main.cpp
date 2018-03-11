#include <string>
#include <fstream>
#include <iostream>

void GetSkyboxFace(std::ifstream& file, char*& data, unsigned int offset, unsigned short width, unsigned short height, unsigned short bpp)
{
	for (short i = 0; i < width * height * bpp; i += width * bpp)
	{
		file.seekg((i * 4) + offset);
		file.read(&data[i], width * bpp);
	}
}

void CreateTGAFile(const std::string& name, char*& data, const unsigned int& length, char*& header)
{
	std::cout << "Generating " << name << ".tga...";
	std::ofstream outFile(name + "tga");
	outFile.write(header, 18);
	outFile.write(data, length);
	outFile.close();
	std::cout << "Done!" << "\n" << std::endl;
}

int main(int argc, char* argv[])
{
	std::cout
		<< "This simple command-line tool is used to \"split\" an input skybox texture" << "\n"
		<< "in to 6 seperate textures for each face of a skybox." << "\n"
		<< "The input skybox image must be an unwrapped cube (example: https://opengameart.org/sites/default/files/Daylight%20Box%20UV.png)." << "\n"
		<< "The width of the image must be a power of 2, and the height must be 3/4 of the width (e.g. 2048 x 1536 or 4096 x 3072)." << "\n"
		<< std::endl;

	std::ifstream file(argv[1], std::ios::binary | std::ios::beg);

	if (!file.is_open())
	{
		std::cout << "Error: Couldn't open " << argv[1] << std::endl;
		std::cin.get();
		return -1;
	}

	char header[18];
	file.read(header, 18);

	unsigned short width;
	memcpy(&width, &header[12], 2);
	unsigned short height;
	memcpy(&height, &header[14], 2);
	unsigned short bpp;
	memcpy(&bpp, &header[16], 2);

	width  /= 4;
	height /= 3;
	bpp    /= 8;	// Convert bits-per-pixel to bytes-per-pixel

	if (width != height)
	{
		std::cout
			<< "Error: Skybox texture dimensions are invalid." << "\n"
			<< "Please ensure the width is a power of 2 and the hight is 3/4 the width." << "\n"
			<< "e.g. 2048 x 1536 or 4096 x 3072" << std::endl;

		std::cin.get();
		return -1;
	}

	char* faceData = new char[width * height * bpp];

	GetSkyboxFace(file, faceData, width * bpp * 1 + 18, width, height, bpp);
	CreateTFAFile("top", faceData, width * height * bpp, header);

	GetSkyboxFace(file, faceData, width * bpp * 4 + 18, width, height, bpp);
	CreateTFAFile("front", faceData, width * height * bpp, header);

	GetSkyboxFace(file, faceData, width * bpp * 5 + 18, width, height, bpp);
	CreateTFAFile("left", faceData, width * height * bpp, header);

	GetSkyboxFace(file, faceData, width * bpp * 6 + 18, width, height, bpp);
	CreateTFAFile("back", faceData, width * height * bpp, header);

	GetSkyboxFace(file, faceData, width * bpp * 7 + 18, width, height, bpp);
	CreateTFAFile("right", faceData, width * height * bpp, header);

	GetSkyboxFace(file, faceData, width * bpp * 9 + 18, width, height, bpp);
	CreateTFAFile("bottom", faceData, width * height * bpp, header);

	delete[] faceData;
	file.close();

	return 0;
}