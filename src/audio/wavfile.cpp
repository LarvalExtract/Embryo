#include "wavfile.h"

WavFile::WavFile(const std::string &wavFileName) :
	AudioFile(wavFileName), bufferSize(64)
{
	// Failed to open file
	if (fileSize == 0)
		return;

	// File is not a wav file
	if (fileName.substr(fileName.rfind("."), fileName.length()) != ".wav")
		return;

	// Copy WAV file header to memory
	char header[24];
	hAudioFile.seekg(GetSubchunkOffset("fmt "), std::ios::beg);
	hAudioFile.read(header, 24);

	// Get info from WAV file header
	memcpy(&numChannels, &header[10], 2);
	memcpy(&sampleRate, &header[12], 4);
	memcpy(&bitsPerSample, &header[22], 2);

	const unsigned char bytesPerSample = bitsPerSample / 8;

	// Get audio samples from WAV file
	hAudioFile.seekg(GetSubchunkOffset("data") + 4, std::ios::beg);
	char dataLengthBytes[4];
	hAudioFile.read(&dataLengthBytes[0], 4);
	memcpy(&channelDataLength, &dataLengthBytes[0], 4);

	// WAV is mono, copy samples to channelData vector
	if (numChannels == 1)
	{
		channelData.push_back(new char[channelDataLength]);
		hAudioFile.read(&channelData[0][0], channelDataLength);		
	}

	// WAV is stereo, interleave throgh samples and copy appropriate samples to left[0] and right[1] channels
	else if (numChannels == 2)
	{
		channelDataLength /= 2;

		channelData.push_back(new char[channelDataLength]);
		channelData.push_back(new char[channelDataLength]);

		for (unsigned int i = 0; i < channelDataLength; i += bytesPerSample)
		{
			hAudioFile.read(&channelData[(char)Channel::Left][i], bytesPerSample);
			hAudioFile.read(&channelData[(char)Channel::Right][i], bytesPerSample);
		}
	}
}

int WavFile::GetSubchunkOffset(const std::string &subchunkID)
{
	// Current byte offset in file
	unsigned int curByteOffset = 12;

	// Validation
	if (fileSize - curByteOffset < bufferSize)
	{
		if (fileSize - curByteOffset == 0)
			return -1;

		else
			bufferSize = fileSize - curByteOffset;
	}

	// Allocate memory of bufferSize length as a temporary buffer to store a chunk of wav file for fast iteration
	char* fileBlock = new char[bufferSize];

	while (curByteOffset < fileSize)
	{
		// Copy chunk of wav file of bufferSize length to memory
		hAudioFile.seekg(curByteOffset - 4, std::ios::beg);
		hAudioFile.read(fileBlock, bufferSize);

		// Iterate through block of wav file to find fmt subchunk
		for (unsigned char i = 0; i < bufferSize; i++)
		{
			if (fileBlock[i] == subchunkID[0])
			{
				i++;
				curByteOffset++;
				if (fileBlock[i] == subchunkID[1])
				{
					i++;
					curByteOffset++;
					if (fileBlock[i] == subchunkID[2])
					{
						i++;
						curByteOffset++;
						if (fileBlock[i] == subchunkID[3])
						{
							// fmt subchunk found, clear memory and return offset of fmt subchunk
							delete[] fileBlock;

							return curByteOffset - 3 - 4;
						}
					}
				}
			}

			else
				curByteOffset++;
		}
	}

	// Couldn't find fmt subchunk, return -1 as error
	return -1;
}