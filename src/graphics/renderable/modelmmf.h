#pragma once

#include "Model.h"

class ModelMMF : public Model
{
public:
	ModelMMF(const std::string &mmfFileName);
	~ModelMMF();
	
private:
	static bool ImportMMF(
		const std::string& filePath,
		std::vector<Vec3<float>> &in_positions,
		std::vector<Vec3<float>> &in_normals,
		std::vector<Vec2<float>> &in_texCoords,
		std::vector<unsigned short> &indices);

	static void GetIndices(std::ifstream &file, std::vector<unsigned short> &index, unsigned int count);
	static void GetUVs(std::ifstream& file, std::vector<Vec2<float>> &uv, unsigned int count);
	static void GetVertex(std::ifstream &file, std::vector<Vec3<float>> &vertex, unsigned int count);
};