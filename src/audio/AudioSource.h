#pragma once

#include <audio/AudioBuffer.h>
#include <maths/_maths.h>

class AudioSource
{
public:
	AudioSource() : soundPos(0, 0, 0), listenerPos(0, 0, 0) {};

	virtual ~AudioSource();

	virtual void Attenuate() { std::cout << "Not attenuating!" << std::endl; };

	void Play();
	void Pause();
	void Stop();

	void SetName(const std::string& name);

	void SetGain(const float& value);
	void SetPitch(const float& value);
	void SetLoop(const bool& value);

	vec3 GetPosition() { return soundPos; };

	std::string GetName();

protected:
	ALuint leftChannel, rightChannel;

	vec3 soundPos;
	vec3 listenerPos;

	std::string name;
};

// Create a positional audio source which emulates a physical location in space
class AudioSourceLocal : public AudioSource
{
public:
	AudioSourceLocal(const std::string& filePath);

	void Attenuate() override; // Calculate audio volume from listener position

	void SetPosition(const float& x, const float& y, const float& z);

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