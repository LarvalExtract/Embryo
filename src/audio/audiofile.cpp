#include "audiofile.h"
#include <utilities/console.h>

AudioFile::AudioFile(const std::string &audioFilePath) :
	fileSize(0),
	channelDataLength(0),
	sampleRate(0),
	bitsPerSample(0),
	numChannels(0)
{
	hAudioFile = std::ifstream("res/sounds/" + audioFilePath, std::ios::binary);

	if (!hAudioFile.is_open())
	{
		Console::Log(LogType::Error) << "Couldn't open " << audioFilePath << "\n";
		return;
	}

	fileName = audioFilePath.substr(audioFilePath.rfind("/") + 1, audioFilePath.length());
	hAudioFile.seekg(0, std::ios::end);
	fileSize = hAudioFile.tellg();
	hAudioFile.seekg(0, std::ios::beg);
}

AudioFile::~AudioFile()
{
	for (unsigned char i = 0; i < channelData.size(); i++)
		delete channelData[i];

	hAudioFile.close();
}