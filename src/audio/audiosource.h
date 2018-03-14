#pragma once

#include <audio/AudioBuffer.h>
#include <maths/maths.h>

#include <utilities/logger.h>

class AudioSource
{
public:
	AudioSource() : soundPos(0, 0, 0), listenerPos(0, 0, 0) {};

	virtual ~AudioSource();

	virtual void Attenuate() { Logger::Log(LogType::Warning) << "Not attenuating!\n"; };

	void Play();
	void Pause();
	void Stop();

	void SetName(const std::string& name);

	void SetGain(float value);
	void SetPitch(float value);
	void SetLoop(const bool& value);

	Vec3<float> GetPosition() { return soundPos; };

	std::string GetName();

protected:
	ALuint leftChannel, rightChannel;

	Vec3<float> soundPos;
	Vec3<float> listenerPos;

	std::string name;
};

// Create a positional audio source which emulates a physical location in space
class AudioSourceLocal : public AudioSource
{
public:
	AudioSourceLocal(const std::string& filePath);

	void Attenuate() override; // Calculate audio volume from listener position

	void SetPosition(float x, float y, float z);

private:
	
};

// Create a non-positional audio source which "plays everywhere"
class AudioSourceGlobal : public AudioSource
{
public:
	AudioSourceGlobal(const std::string& filePath);

	void Attenuate() override;

private:

};