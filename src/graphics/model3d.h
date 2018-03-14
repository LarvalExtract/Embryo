#pragma once

#include "Renderable3D.h"

// Render 3D models from loaded MMF files
class Model3D : public Renderable3D
{
public:
	Model3D(const std::string &filePath);

	static bool ImportModel(
		const std::string& filePath,
		std::vector<Vec3<float>> &in_positions,
		std::vector<Vec3<float>> &in_normals,
		std::vector<Vec2<float>> &in_texCoords,
		std::vector<unsigned short> &indices);

	static void GetIndices(std::ifstream &file, std::vector<unsigned short> &index, unsigned int count);
	static void GetUVs(std::ifstream& file, std::vector<Vec2<float>> &uv, unsigned int count);
	static void GetVertex(std::ifstream &file, std::vector<Vec3<float>> &vertex, unsigned int count);

	void SetTexture(const std::string &filePath);
	void SetNormalMap(const std::string &filePath);
	void SetSpecularMap(const std::string &filePath);

	virtual void Draw();
private:

};