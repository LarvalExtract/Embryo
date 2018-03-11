#pragma once

#include <string>

void ImportWAV(
	const std::string& filePath,
	char*& leftChannelData,
	char*& rightChannelData,
	unsigned int& dataLength,
	unsigned int& sampleRate,
	unsigned short& bitsPerSample);