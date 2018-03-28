#pragma once

#include "audiofile.h"

class WavFile : public AudioFile
{
public:
	WavFile(const std::string &wavFileName);

private:
	char bufferSize;
	int GetSubchunkOffset(const std::string &subchunkID);
};