#include "import_audio_wav.h"
#include "file.h"

#include <fstream>
#include <iostream>

char* CopyFileChunk(char* pMemBlock, std::ifstream& file, unsigned int offset, unsigned int size)
{
	file.seekg(offset, std::ios::beg);
	file.read(pMemBlock, size);

	return pMemBlock;
}

unsigned int GetSubchunkOffset(const std::string& subchunkId, std::ifstream& file, unsigned int offset, unsigned int fileSize)
{
	unsigned short memblockSize = 100;
	unsigned int curByteOffset = offset;

	if ((fileSize - curByteOffset) < memblockSize)
	{
		if ((fileSize - curByteOffset) == 0)
			return -1;

		memblockSize = fileSize - curByteOffset;
	}

	char* memblock = new char[memblockSize];
	memblock = CopyFileChunk(memblock, file, offset, memblockSize);

	while (curByteOffset < fileSize)
	{
		for (unsigned int i = 0; i < memblockSize; i++)
		{
			if (memblock[i] == subchunkId[0])
			{
				i++;
				curByteOffset++;
				if (memblock[i] == subchunkId[1])
				{
					i++;
					curByteOffset++;
					if (memblock[i] == subchunkId[2])
					{
						i++;
						curByteOffset++;

						//This is what will execute once subchunk has been found
						if (memblock[i] == subchunkId[3])
						{
							i++;
							curByteOffset++;

							delete[] memblock;

							return (curByteOffset - 4);

						}
						else
							curByteOffset++;
					}
					else
						curByteOffset++;
				}
				else
					curByteOffset++;
			}
			else
				curByteOffset++;
		}
		//If "subchunk" wasn't found, get the next block of memory
		memblock = CopyFileChunk(memblock, file, curByteOffset - 4, memblockSize);
		curByteOffset -= 4;
	}
	//Return -1 if subchunk could not be found at all
	return -1;
}

void ImportWAV(
	const std::string& filePath,
	char*& leftChannelData,
	char*& rightChannelData,
	unsigned int& dataLength,
	unsigned int& sampleRate,
	unsigned short& bitsPerSample)
{
	std::ifstream file("res/sounds/" + filePath, std::ios::binary);
	std::string fileName = filePath.substr(filePath.rfind("/") + 1, filePath.length());

	if (!file.is_open())
	{
		std::cout << "Error: Couldn't open " << filePath << std::endl;
		return;
	}

	file.seekg(0, std::ios::end);
	unsigned int fileSize = file.tellg();

	// Find the offset of the fmt subchunk within the wav file
	unsigned int fmtOffset = GetSubchunkOffset("fmt ", file, 0, fileSize);

	if (fmtOffset != -1)
	{
		char header[24];
		file.seekg(fmtOffset, std::ios::beg);
		file.read(header, 24);

		unsigned short format;
		unsigned short channels;

		memcpy(&format,		   &header[8], 2);
		memcpy(&channels,	   &header[10], 2);
		memcpy(&sampleRate,	   &header[12], 4);
		memcpy(&bitsPerSample, &header[22], 2);

		unsigned int dataOffset = GetSubchunkOffset("data", file, fmtOffset + 24, fileSize) + 4;	
		file.seekg(dataOffset, std::ios::beg);

		char dlBytes[4];
		file.read(&dlBytes[0], 4);
		memcpy(&dataLength, &dlBytes[0], 4);

		dataLength /= 2;

		leftChannelData = new char[dataLength];
		rightChannelData = new char[dataLength];

		// If WAV is mono, duplicate mono channel in to left and right buffers
		if (channels == 1)
		{
			char curSample[2];
			for (unsigned int i = 0; i < dataLength; i += 2)
			{
				file.read(&curSample[0], 2);
				memcpy(&leftChannelData[i], &curSample, 2);
				memcpy(&rightChannelData[i], &curSample, 2);
			}
		}

		// If WAV is stereo, extract each channel in to respective buffers
		else if (channels == 2)
		{
			char curSample[2];
			for (unsigned int i = 0; i < dataLength; i += 2)
			{
				file.read(&curSample[0], 2);
				memcpy(&leftChannelData[i], &curSample, 2);
				file.read(&curSample[0], 2);
				memcpy(&rightChannelData[i], &curSample, 2);
			}
		}
	}

	else
	{
		std::cout << "Error: Couldn't find format subchunk in specified file. Please ensure the file you specified is a valid WAV file." << std::endl;
	}

	file.close();
}