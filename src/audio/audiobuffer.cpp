#include "AudioBuffer.h"
#include "wavfile.h"

AudioBuffer::AudioBuffer(const std::string& filePath)
{
	alGenBuffers(1, &bufferLeft);
	alGenBuffers(1, &bufferRight);

	WavFile wavFile(filePath);

	alBufferData(bufferLeft, AL_FORMAT_MONO16, wavFile.channelData[(char)Channel::Left], wavFile.channelDataLength, wavFile.sampleRate);
	alBufferData(bufferRight, AL_FORMAT_MONO16, wavFile.channelData[(char)Channel::Right], wavFile.channelDataLength, wavFile.sampleRate);
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