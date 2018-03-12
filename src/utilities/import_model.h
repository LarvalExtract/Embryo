#pragma once

#include <string>
#include <vector>
#include <maths/Vec3.h>
#include <maths/Vec2.h>

void ImportModel(
	const std::string& filePath, 
	std::vector<Vec3<float>> &in_positions, 
	std::vector<Vec3<float>> &in_normals, 
	std::vector<Vec2<float>> &in_texCoords, 
	std::vector<unsigned short> &indices);