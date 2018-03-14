#pragma once

#include <AL/al.h>

#include <string>
#include <fstream>

class AudioBuffer
{
public:
	AudioBuffer(const std::string& filePath);
	~AudioBuffer();

	ALuint& GetLeftChannelID();
	ALuint& GetRightChannelID();

private:
	ALuint bufferLeft, bufferRight;

	void Init(const std::string& filePath);
};