#include "import_model.h"
#include <utilities/file.h>

#include <fstream>
#include <iostream>

void GetVertex(std::ifstream &file, std::vector<Vec3<float>> &vertex, unsigned int count)
{
	char test[4];
	Vec3<float> currentVertex;

	for (unsigned int i = 0; i < count; i++)
	{
		file.read(&test[0], 4);
		memcpy(&currentVertex.x, &test[0], sizeof(float));
		file.read(&test[0], 4);
		memcpy(&currentVertex.y, &test[0], sizeof(float));
		file.read(&test[0], 4);
		memcpy(&currentVertex.z, &test[0], sizeof(float));
		vertex.push_back(currentVertex);
	}
}

void GetUVs(std::ifstream& file, std::vector<Vec2<float>> &uv, unsigned int count)
{
	char test[4];
	Vec2<float> currentUV;

	for (unsigned int i = 0; i < count; i++)
	{
		file.read(&test[0], 4);
		memcpy(&currentUV.x, &test[0], sizeof(float));
		file.read(&test[0], 4);
		memcpy(&currentUV.y, &test[0], sizeof(float));
		uv.push_back(currentUV);
	}
}

void GetIndices(std::ifstream &file, std::vector<unsigned short> &index, unsigned int count)
{
	char test[2];
	unsigned short currentIndex;

	for (unsigned int i = 0; i < count; i++)
	{
		file.read(&test[0], 2);
		memcpy(&currentIndex, &test[0], sizeof(unsigned short));
		index.push_back(currentIndex);
	}
}

void ImportModel(
	const std::string& filePath, 
	std::vector<Vec3<float>> &in_positions,
	std::vector<Vec3<float>> &in_normals,
	std::vector<Vec2<float>> &in_texCoords,
	std::vector<unsigned short> &indices)
{
	std::ifstream file("res/models/" + filePath, std::ios::binary);
	std::string fileName = filePath.substr(filePath.rfind("/") + 1, filePath.length());

	if (!file.is_open())
	{
		std::cout << "Error: Couldn't open " << fileName << std::endl;
		return;
	}

	char header[13];
	file.seekg(0, std::ios::beg);
	file.read(&header[0], 13);

	if (header[0] == 'M' && header[1] == 'M' && header[2] == 'F')
	{
		unsigned char errors;
		unsigned int  numVertices;
		unsigned int  numIndices;

		memcpy(&errors,		 &header[4], 1);
		memcpy(&numVertices, &header[5], 4);
		memcpy(&numIndices,  &header[9], 4);

		file.seekg(13, std::ios::beg);

		in_positions.reserve(numVertices);
		in_normals.reserve(numVertices);
		in_texCoords.reserve(numVertices);
		indices.reserve(numIndices);

		GetVertex	(file, in_positions, numVertices);
		GetVertex	(file, in_normals, numVertices);
		GetUVs		(file, in_texCoords, numVertices);
		GetIndices	(file, indices, numIndices);

		file.close();
	}

	else
	{
		std::cout << "Error: Selected file is not an MMF file. Use obj-mmf.exe to convert a wavefront obj model file in to an appropriate MMF file." << std::endl;
		return;
	}
}