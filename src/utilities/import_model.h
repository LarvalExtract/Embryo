#pragma once

#include <string>
#include <vector>
#include <maths/vec3.h>
#include <maths/vec2.h>

void ImportModel(
	const std::string& filePath, 
	std::vector<vec3>& in_positions, 
	std::vector<vec3>& in_normals, 
	std::vector<vec2>& in_texCoords, 
	std::vector<unsigned short>& indices);