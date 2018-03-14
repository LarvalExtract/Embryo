#include "AudioSource.h"

//// AudioSource base class
AudioSource::~AudioSource()
{
	Stop();

	alDeleteSources(1, &leftChannel);
	alDeleteSources(1, &rightChannel);
}

void AudioSource::Play()
{
	alSourcePlay(leftChannel);
	alSourcePlay(rightChannel);
}

void AudioSource::Pause()
{
	alSourcePause(leftChannel);
	alSourcePause(rightChannel);
}

void AudioSource::Stop()
{
	alSourceStop(leftChannel);
	alSourceStop(rightChannel);
}

void AudioSource::SetName(const std::string& name)
{
	this->name = name;
}

void AudioSource::SetGain(float value)
{
	alSourcef(leftChannel, AL_GAIN, value);
	alSourcef(rightChannel, AL_GAIN, value);
}

void AudioSource::SetPitch(float value)
{
	alSourcef(leftChannel, AL_PITCH, value);
	alSourcef(rightChannel, AL_PITCH, value);
}

void AudioSource::SetLoop(const bool& value)
{
	alSourcei(leftChannel, AL_LOOPING, value);
	alSourcei(rightChannel, AL_LOOPING, value);
}

std::string AudioSource::GetName()
{
	return name;
}


//// AudioSourceLocal sub-class
//// Inherits from AudioSource
AudioSourceLocal::AudioSourceLocal(const std::string& filePath) :
	AudioSource()
{
	alGenSources(1, &leftChannel);
	alSource3f(leftChannel, AL_POSITION, soundPos.x, soundPos.y, soundPos.z);

	alGenSources(1, &rightChannel);
	alSource3f(rightChannel, AL_POSITION, soundPos.x, soundPos.y, soundPos.z);

	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

	AudioBuffer buffer(filePath);

	name = filePath.substr(0, filePath.rfind("."));

	alSourceQueueBuffers(leftChannel, 1, &buffer.GetLeftChannelID());
	alSourceQueueBuffers(rightChannel, 1, &buffer.GetRightChannelID());

	alSourcei(leftChannel, AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcei(rightChannel, AL_SOURCE_RELATIVE, AL_TRUE);
}

void AudioSourceLocal::Attenuate()
{
	alGetListener3f(AL_POSITION, &listenerPos.x, &listenerPos.y, &listenerPos.z);

	//alGetSource3f(leftChannel, AL_POSITION, &soundPos.x, &soundPos.y, &soundPos.z);

	alSourcef(leftChannel, AL_GAIN, AL_REFERENCE_DISTANCE / (AL_REFERENCE_DISTANCE + AL_ROLLOFF_FACTOR * (Maths::Distance(listenerPos, soundPos))));
	alSourcef(rightChannel, AL_GAIN, AL_REFERENCE_DISTANCE / (AL_REFERENCE_DISTANCE + AL_ROLLOFF_FACTOR * (Maths::Distance(listenerPos, soundPos))));
}

void AudioSourceLocal::SetPosition(float x, float y, float z)
{
	alSource3f(leftChannel, AL_POSITION, x, y, z);
	alSource3f(rightChannel, AL_POSITION, x, y, z);

	soundPos.x = x;
	soundPos.y = y;
	soundPos.z = z;
}


//// AudioSourceGlobal sub-class
//// Inherits from AudioSource
AudioSourceGlobal::AudioSourceGlobal(const std::string& filePath) :
	AudioSource()
{
	alGenSources(1, &leftChannel);
	alGenSources(1, &rightChannel);

	AudioBuffer buffer(filePath);

	name = filePath.substr(0, filePath.rfind("."));

	alSourceQueueBuffers(leftChannel, 1, &buffer.GetLeftChannelID());
	alSourceQueueBuffers(rightChannel, 1, &buffer.GetRightChannelID());

	alSourcei(leftChannel,  AL_SOURCE_RELATIVE, AL_FALSE);
	alSourcei(rightChannel, AL_SOURCE_RELATIVE, AL_FALSE);
}

void AudioSourceGlobal::Attenuate()
{
	alGetListener3f(AL_POSITION, &listenerPos.x, &listenerPos.y, &listenerPos.z);

	alSource3f(leftChannel,  AL_POSITION, listenerPos.x, listenerPos.y, listenerPos.z);
	alSource3f(rightChannel, AL_POSITION, listenerPos.x, listenerPos.y, listenerPos.z);
}