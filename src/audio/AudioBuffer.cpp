#include "AudioBuffer.h"
#include <utilities/import_audio_wav.h>
#include <iostream>

AudioBuffer::AudioBuffer(const std::string& filePath)
{
	Init(filePath);
}

AudioBuffer::~AudioBuffer()
{
	alDeleteBuffers(1, &bufferLeft);
	alDeleteBuffers(1, &bufferRight);
}

ALuint& AudioBuffer::GetLeftChannelID()
{
	return bufferLeft;
}

ALuint& AudioBuffer::GetRightChannelID()
{
	return bufferRight;
}

void AudioBuffer::Init(const std::string& filePath)
{
	char* leftChannelData = nullptr;
	char* rightChannelData = nullptr;
	unsigned int channelDataLength;

	unsigned int sampleRate;
	unsigned short bitsPerSample;
	
	ImportWAV(filePath, leftChannelData, rightChannelData, channelDataLength, sampleRate, bitsPerSample);

	alGenBuffers(1, &bufferLeft);
	alGenBuffers(1, &bufferRight);

	alBufferData(bufferLeft,  AL_FORMAT_MONO16, leftChannelData,  channelDataLength, sampleRate);
	alBufferData(bufferRight, AL_FORMAT_MONO16, rightChannelData, channelDataLength, sampleRate);
	
	delete[] leftChannelData;
	delete[] rightChannelData;
}