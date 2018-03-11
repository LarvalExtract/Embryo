#include <string>
#include <fstream>
#include <iostream>

void GetSkyboxFace(std::ifstream& file, char*& data, unsigned int offset, unsigned short width, unsigned short height, unsigned short bpp)
{
	for (unsigned int i = 0; i < width * height * bpp; i += width * bpp)
	{
		file.seekg((i * 4) + offset + 18, std::ios::beg);
		file.read(&data[i], width * bpp);
	}
}

void FlipYAxis(char*& source, char*& flipped, unsigned short width, unsigned short height, unsigned short bpp)
{
	unsigned int j = 0;

	for (int i = (width * height * bpp) - (width * bpp); i > -1; i -= width * bpp)
	{
		memcpy(&flipped[j], &source[i], width * bpp);
		j += (width * bpp);
	}
}

void CreateTGAFile(const std::string& name, char*& data, const unsigned int& length, char* header)
{
	std::cout << "Generating " << name << ".tga...";
	std::ofstream outFile(name + "/" + name + ".tga");
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
		<< "\n"
		<< "The input skybox image must be an unwrapped cube" << "\n"
		<< "(example: https://opengameart.org/sites/default/files/Daylight%20Box%20UV.png)." << "\n"
		<< "\n"
		<< "The width of the image must be a power of 2, and the height must be 3/4 of the width (e.g. 2048 x 1536 or 4096 x 3072)." << "\n"
		<< std::endl;

	std::string fileName = argv[1];
	//std::string fileName = "D:\\Documents\\Visual Studio 2017\\Projects\\Embryo\\tools\\skybox\\Debug\\skybox_day.tga";
	std::ifstream file(argv[1], std::ios::binary | std::ios::beg);

	fileName = fileName.substr(fileName.rfind("\\") + 1, fileName.length());
	fileName.erase(fileName.length() - 4, 4);

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

	width /= 4;
	height /= 3;

	if (width != height)
	{
		std::cout
			<< "Error: Skybox texture dimensions are invalid." << "\n"
			<< "Please ensure the width is a power of 2 and the hight is 3/4 the width." << "\n"
			<< "e.g. 2048 x 1536 or 4096 x 3072" << std::endl;

		std::cin.get();
		return -1;
	}

	memcpy(&header[12], &width, 2);
	memcpy(&header[14], &height, 2);
	memcpy(&header[16], &bpp, 2);

	bpp /= 8;	// Convert bits-per-pixel to bytes-per-pixel

	char* faceData = new char[width * height * bpp];
	char* flipped = new char[width * height * bpp];

	bool flipYaxis = true;

	if (!flipYaxis)
	{																												//			A crude visualisation of a skybox texture
		GetSkyboxFace(file, faceData, ((width * height * bpp) * 0) + ((width * bpp) * 1), width, height, bpp);		//
		CreateTGAFile(fileName + "_bottom", faceData, width * height * bpp, header);								//			|------------|------------|------------|------------|	
																													//			|			 |************|			   |		    |	
		GetSkyboxFace(file, faceData, ((width * height * bpp) * 4) + ((width * bpp) * 0), width, height, bpp);		//			|	8, 0	 |****8, 1****|	  8, 2	   |    8, 3	|	
		CreateTGAFile(fileName + "_left", faceData, width * height * bpp, header);									//			|			 |************|			   |		    |	
																													//			|			 |************|			   |			|	
		GetSkyboxFace(file, faceData, ((width * height * bpp) * 4) + ((width * bpp) * 1), width, height, bpp);		//			|------------|------------|------------|------------|
		CreateTGAFile(fileName + "_front", faceData, width * height * bpp, header);									//			|************|************|************|************|	
																													//			|****4, 0****|****4, 1****|****4, 2****|****4, 3****|	
		GetSkyboxFace(file, faceData, ((width * height * bpp) * 4) + ((width * bpp) * 2), width, height, bpp);		//			|************|************|************|************|	
		CreateTGAFile(fileName + "_right", faceData, width * height * bpp, header);									//			|************|************|************|************|
																													//			|------------|------------|------------|------------|
		GetSkyboxFace(file, faceData, ((width * height * bpp) * 4) + ((width * bpp) * 3), width, height, bpp);		//			|			 |************|			   |		    |	
		CreateTGAFile(fileName + "_back", faceData, width * height * bpp, header);									//			|	0, 0	 |****0, 1****|	  0, 2	   |    0, 3	|	
																													//			|			 |************|			   |		    |	
		GetSkyboxFace(file, faceData, ((width * height * bpp) * 8) + ((width * bpp) * 1), width, height, bpp);		//			|			 |************|			   |		    |	
		CreateTGAFile(fileName + "_top", faceData, width * height * bpp, header);									//			|------------|------------|------------|------------|
	}

	else
	{
		GetSkyboxFace(file, faceData, ((width * height * bpp) * 0) + ((width * bpp) * 1), width, height, bpp);
		FlipYAxis(faceData, flipped, width, height, bpp);
		CreateTGAFile(fileName + "_bottom", flipped, width * height * bpp, header);

		GetSkyboxFace(file, faceData, ((width * height * bpp) * 4) + ((width * bpp) * 0), width, height, bpp);
		FlipYAxis(faceData, flipped, width, height, bpp);
		CreateTGAFile(fileName + "_left", flipped, width * height * bpp, header);

		GetSkyboxFace(file, faceData, ((width * height * bpp) * 4) + ((width * bpp) * 1), width, height, bpp);
		FlipYAxis(faceData, flipped, width, height, bpp);
		CreateTGAFile(fileName + "_front", flipped, width * height * bpp, header);

		GetSkyboxFace(file, faceData, ((width * height * bpp) * 4) + ((width * bpp) * 2), width, height, bpp);
		FlipYAxis(faceData, flipped, width, height, bpp);
		CreateTGAFile(fileName + "_right", flipped, width * height * bpp, header);

		GetSkyboxFace(file, faceData, ((width * height * bpp) * 4) + ((width * bpp) * 3), width, height, bpp);
		FlipYAxis(faceData, flipped, width, height, bpp);
		CreateTGAFile(fileName + "_back", flipped, width * height * bpp, header);

		GetSkyboxFace(file, faceData, ((width * height * bpp) * 8) + ((width * bpp) * 1), width, height, bpp);
		FlipYAxis(faceData, flipped, width, height, bpp);
		CreateTGAFile(fileName + "_top", flipped, width * height * bpp, header);
	}

	delete[] faceData;
	delete[] flipped;

	file.close();

	std::cin.get();

	return 0;
}