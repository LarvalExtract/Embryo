#pragma once

#include "audiofile.h"

class WavFile : public AudioFile
{
public:
	WavFile(const std::string &wavFileName);

private:
	const static char bufferSize;
	int GetSubchunkOffset(const std::string &subchunkID, const unsigned int startOffset);
};