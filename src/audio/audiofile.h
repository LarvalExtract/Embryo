#pragma once

#include <string>
#include <fstream>
#include <vector>

enum class Channel
{
	Left,
	Right
};

class AudioFile
{
public:
	virtual ~AudioFile();

	std::string fileName;
	unsigned int fileSize;
	std::vector<char*> channelData;
	unsigned int channelDataLength;
	unsigned int sampleRate;
	unsigned short bitsPerSample;
	unsigned short numChannels;

protected:
	AudioFile(const std::string &audioFilePath);

	std::ifstream hAudioFile;
};