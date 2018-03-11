#include "file.h"

std::string File::Load(std::string filePath)
{
	std::ifstream file(filePath);
	std::string line;
	std::string output;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			output.append(line + "\n");
		}
	}

	return output;
}

std::string File::GetFileName(const std::string& filePath)
{
	return filePath.substr(filePath.rfind("/") + 1, filePath.length());
}

char File::charToNum(const char byte)
{
	char result = 0;
	return result = (char)byte;
}

unsigned File::charToNum_u(const unsigned char byte)
{
	unsigned char result = 0;
	return result = (unsigned char)byte;
}

short File::shortToNum(const char bytes[2])
{
	short result = 0;
	return result =   short((char)(bytes[1]) << 8 |
							(char)(bytes[0]));
}

unsigned short File::shortToNum_u(const unsigned char bytes[2])
{
	unsigned short result = 0;
	return result =   short((unsigned char)(bytes[1]) << 8 |
							(unsigned char)(bytes[0]));
}

int File::intToNum(const char bytes[4])
{
	int result = 0;
	return result =     int((char)(bytes[3]) << 24 |
							(char)(bytes[2]) << 16 |
							(char)(bytes[1]) <<  8 |
							(char)(bytes[0]));
}

unsigned File::intToNum_u(const unsigned char bytes[4])
{
	unsigned int result = 0;
	return result = unsigned int((unsigned char)(bytes[3]) << 24 |
								 (unsigned char)(bytes[2]) << 16 |
								 (unsigned char)(bytes[1]) <<  8 |
								 (unsigned char)(bytes[0]));
}