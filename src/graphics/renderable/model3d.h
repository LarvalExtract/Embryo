#pragma once

#include "renderable.h"
#include <graphics/texture.h>
#include <vector>

class Model3D : public Renderable
{
public:
	Model3D(const std::string &fileName);

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;
	void SetDiffuseTexture(const std::string &fileName);

	float specularity;
	float glossiness;

private:
	static bool ImportModel(
		const std::string& filePath,
		std::vector<Vec3<float>> &in_positions,
		std::vector<Vec3<float>> &in_normals,
		std::vector<Vec2<float>> &in_texCoords,
		std::vector<unsigned short> &indices);

	static bool ImportOBJ(
		const std::string &filePath,
		std::vector<Vec3<float>> &in_positions,
		std::vector<Vec3<float>> &in_normals,
		std::vector<Vec2<float>> &in_texCoords,
		std::vector<std::vector<unsigned int>> &out_indices
	);

	static void GetIndices(std::ifstream &file, std::vector<unsigned short> &index, unsigned int count);
	static void GetUVs(std::ifstream& file, std::vector<Vec2<float>> &uv, unsigned int count);
	static void GetVertex(std::ifstream &file, std::vector<Vec3<float>> &vertex, unsigned int count);

	Texture2D *diffuseMap;
	Texture2D *normalMap;		// Currently unused
	Texture2D *specularMap;		// Currently unused

	static Texture2D* const defaultTexture;
};