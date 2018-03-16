#pragma once

#include <iostream>

#include "windows.h"

// Colour codes used to colour text in console
enum class ColourCode : char
{
	Black = 0x00,
	Blue = 0x01,
	Green = 0x02,
	Cyan = 0x03,
	Red = 0x04,
	Magenta = 0x05,
	Brown = 0x06,
	White = 0x07,
	Grey = 0x08,
	BrightBlue = 0x09,
	BrightGreen = 0x0A,
	BrightCyan = 0x0B,
	BrightRed = 0x0C,
	BrightMagenta = 0x0D,
	BrightYellow = 0x0E,
	BrightWhite = 0x0F
};

// Log type used to determine how to print a message
enum class LogType
{
	None,
	Log,
	Warning,
	Error
};

// Adds two colour codes to make the console text foreground/background colour
ColourCode operator+(ColourCode lhs, const ColourCode &rhs);

// Singleton console logger
class Logger
{
public:
	~Logger();

	// Set the colour of the next message
	Logger& operator<<(ColourCode code);
	
	// Print a message with the current colour
	template<class T>
	Logger& operator<<(T message)
	{
		std::cerr << message;

		// Restore console state to original
		//SetConsoleTextAttribute(hStdOut, csbi.wAttributes);

		return *this;
	}
	
	// Returns singleton logger
	static Logger& Log(LogType type = LogType::Log);

	static char* const strLog;
	static char* const strWarning;
	static char* const strError;
private:
	Logger();

	static Logger *pLogger;

	static HANDLE hStdOut;
	//CONSOLE_SCREEN_BUFFER_INFO csbi;
	//CONSOLE_SCREEN_BUFFER_INFO colourCsbi;
};