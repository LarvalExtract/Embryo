#pragma once

#include "model3d.h"

class ModelOBJ : public Model3D
{
public:
	ModelOBJ(const std::string &objFileName);
	~ModelOBJ();

private:
	struct ObjFace
	{
		int v[3];
		int vt[3];
		int vn[3];

		ObjFace() :v{ -1 }, vt{ -1 }, vn{ -1 } {}
	};

	typedef std::vector< std::vector<Vec3<float>>* > vec3Lists;
	typedef std::vector< std::vector<Vec2<float>>* > vec2Lists;
	typedef std::vector< std::vector<ObjFace>* >	 objFaceLists;

	static bool ImportOBJ(
		const std::string &filePath,
		std::vector<Vec3<float>> &in_positions,
		std::vector<Vec3<float>> &in_normals,
		std::vector<Vec2<float>> &in_texCoords,
		std::vector<std::vector<unsigned int>> &out_indices
	);

	// Extract a Vec3 from v or vn line in OBJ file
	static Vec3<float> GetVec3FromOBJLine(std::string &line);

	// Extract a Vec2 from vt line in OBJ file
	static Vec2<float> GetVec2FromOBJLine(std::string &line);

	// Extract dave data from f line in OBJ file
	static ObjFace GetFaceFromOBJLine(std::string &line, unsigned int numUVs, unsigned int numNormals);

	// Compare two floats to determine whether they are the same (used for indexing)
	static bool Compare(float a, float b);

	// Use OBJ indices to generate un-indexed vertex arrays
	static void GenerateVertexArrays(
		vec3Lists &out_positions,
		vec3Lists &out_normals,
		vec2Lists &out_texCoords,
		const std::vector<Vec3<float>> &in_positions,
		const std::vector<Vec3<float>> &in_normals,
		const std::vector<Vec2<float>> &in_texCoords,
		const std::vector<ObjFace> &in_indexList);

	// Compare an input Vertex to map of vertices
	// If similar, return the index. Otherwise, return false
	static bool GetSimilarVertexIndex(
		Vec3<float> &in_positions,
		Vec3<float> &in_normals,
		Vec2<float> &in_texCoords,
		std::vector<Vec3<float>> &out_positions,
		std::vector<Vec3<float>> &out_normals,
		std::vector<Vec2<float>> &out_texCoords,
		unsigned int &result);

	// Iterate through an array of vertices to find similar vertices and generate a list of indices
	static void GenerateIndices(
		vec3Lists &in_positions,
		vec3Lists &in_normals,
		vec2Lists &in_texCoords,
		vec3Lists &out_positions,
		vec3Lists &out_normals,
		vec2Lists &out_texCoords,
		std::vector<std::vector<unsigned int>> &out_indices);
};