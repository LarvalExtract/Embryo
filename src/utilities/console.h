#pragma once

#include <iostream>

#include <string>
#include <unordered_map>

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
	Error,
	Success
};

// Adds two colour codes to make the console text foreground/background colour
ColourCode operator+(ColourCode lhs, const ColourCode &rhs);

// Console command/variable typedefs
typedef void(*FuncPtrS)(std::string s);

struct Var
{
	std::string value;
	std::string defaultValue;
};

typedef std::pair<std::string, Var> ConVar;
typedef std::unordered_map<ConVar::first_type, ConVar::second_type> ConVars;

struct Command
{
	FuncPtrS delegate;
	std::string argument;
	std::string desc;
};

typedef std::pair<std::string, Command> ConCmd;
typedef std::unordered_map<ConCmd::first_type, ConCmd::second_type> ConCmds;

// Singleton developer console
class Console
{
public:
	~Console();

	// Set the colour of the next message
	Console& operator<<(ColourCode code);
	
	// Print a message with the current colour
	template<class T>
	Console& operator<<(T message)
	{
		std::cerr << message;

		// Restore console state to original
		//SetConsoleTextAttribute(hStdOut, csbi.wAttributes);

		return *this;
	}
	
	// Returns singleton console
	static Console& Log(LogType type = LogType::Log);

	static bool AddCommand(std::string cmdName, FuncPtrS funcPtr);
	static bool AddCommand(std::string cmdName, FuncPtrS funcPtr, std::string desc);

	static void CleanArgument(std::string &argument);

	// Add console variable
	static bool AddVar(std::string varName);

	// Add console variable with a string value
	static bool AddVar(std::string varName, std::string value);

	// Add console variable with a Boolean value (int type to prevent bool overriding std::string)
	static bool AddVar(std::string varName, int value);
	
	static ConVar::first_type GetVar(std::string varName);
	static bool GetVarB(std::string varName);
	static float GetVarF(std::string varName);

	static bool SetVar(std::string varName, std::string value);

	static void ConsoleLoop();

	// Test console command
	static void CmdTest(std::string argument);

	// Print available commands
	static void CmdListC(std::string argument);

	// Print available variables
	static void CmdListV(std::string argument);

	static char* const strWarning;
	static char* const strError;
	static char* const strSuccess;

	static bool bIsRunning;
private:
	Console();

	static Console *pConsole;

	static HANDLE hStdOut;
	//CONSOLE_SCREEN_BUFFER_INFO csbi;
	//CONSOLE_SCREEN_BUFFER_INFO colourCsbi;

	// Console
	static ConVars conVars;
	static ConCmds conCmds;
};