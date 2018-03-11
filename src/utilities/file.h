#pragma once

#include <iostream>
#include <string>
#include <fstream>

namespace File
{
	std::string Load(std::string filePath);

	std::string GetFileName(const std::string& filePath);

	char charToNum(const char byte);
	unsigned charToNum_u(const unsigned char byte);

	short shortToNum(const char bytes[2]);
	unsigned short shortToNum_u(const unsigned char bytes[2]);

	int intToNum(const char bytes[4]);
	unsigned intToNum_u(const unsigned char bytes[4]);
};
