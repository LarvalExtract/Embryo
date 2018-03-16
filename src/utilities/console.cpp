#include "console.h"

#include <algorithm>
#include <chrono>

Console* Console::pConsole = nullptr;
HANDLE Console::hStdOut = nullptr;

char* const Console::strWarning = "Warning: ";
char* const Console::strError = "Error: ";
bool Console::bIsRunning = false;
ConCmds Console::conCmds = {};
ConVars Console::conVars = {};

Console::Console()
{
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// Store console state
	//GetConsoleScreenBufferInfo(hStdOut, &csbi);
}

Console::~Console()
{
	delete pConsole;
}

Console& Console::operator<<(ColourCode code)
{
	// Set text colour
	SetConsoleTextAttribute(hStdOut, static_cast<char>(code));

	// Return a singleton pointer (should never be nullptr)
	return *pConsole;
}

Console& Console::Log(LogType type)
{
	// Create an instance of pConsole if it doesn't already exist
	if (pConsole == nullptr)
		pConsole = new Console();

	// Print timestamp if the line continuation log type has not been chosen
	// TO-DO: Move this into a function
	// TO-DO: Replace literals with consts
	if (type != LogType::None)
	{
		char buffer[12];
		tm timeInfo;
		time_t rawTime;
		time(&rawTime);
		localtime_s(&timeInfo, &rawTime);

		SetConsoleTextAttribute(hStdOut, static_cast<char>(ColourCode::White));
		strftime(buffer, 12, "[%H:%M:%S] ", &timeInfo);

		std::cerr << buffer;
	}

	// Appends a log type tag to the start of the message and colours the line
	switch (type)
	{
	case LogType::Log:
		break;
	case LogType::Warning:
		SetConsoleTextAttribute(hStdOut, static_cast<char>(ColourCode::BrightYellow));
		std::cerr << strWarning;
		break;
	case LogType::Error:
		SetConsoleTextAttribute(hStdOut, static_cast<char>(ColourCode::BrightRed));
		std::cerr << strError;
		break;
	default:
		break;
	}

	return *pConsole;
}

bool Console::AddCommand(std::string cmdName, FuncPtrS funcPtr)
{
	// Convert command name to lowercase
	std::transform(cmdName.begin(), cmdName.end(), cmdName.begin(), tolower);

	ConCmd conCmd;

	conCmd.first = cmdName;
	conCmd.second.delegate = funcPtr;
	conCmd.second.argument = "";

	std::pair<ConCmds::const_iterator, bool> result = conCmds.insert(conCmd);

	return result.second;
}

// TO-DO: MOVE THIS ELSEWHERE!!
bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }

void Console::CleanArgument(std::string &argument)
{
	// Remove prefix and suffix spaces
	size_t firstPos = argument.find_first_not_of(VK_SPACE);

	if (firstPos == std::string::npos)
	{
		argument.erase();
		return;
	}

	argument = argument.substr(firstPos, argument.find_last_not_of(VK_SPACE) - firstPos + 1);

	// Remove duplicate spaces
	std::string::iterator end = std::unique(argument.begin(), argument.end(), BothAreSpaces);
	argument.erase(end, argument.end());
}

bool Console::AddVar(std::string varName)
{
	return AddVar(varName, "");
}

bool Console::AddVar(std::string varName, std::string value)
{
	// Convert variable name to lowercase
	std::transform(varName.begin(), varName.end(), varName.begin(), tolower);

	ConVar conVar;
	conVar.first = varName;
	conVar.second.value = value;
	conVar.second.defaultValue = value;

	std::pair<ConVars::const_iterator, bool> result = conVars.insert(conVar);

	return result.second;
}

ConVar::first_type Console::GetVar(std::string varName)
{
	// Convert variable name to lowercase
	std::transform(varName.begin(), varName.end(), varName.begin(), tolower);

	ConVars::const_iterator it = conVars.find(varName);

	return it != conVars.end() ? it->second.value : "";
}

bool Console::SetVar(std::string varName, std::string value)
{
	// Convert variable name to lowercase
	std::transform(varName.begin(), varName.end(), varName.begin(), tolower);

	ConVars::iterator it = conVars.find(varName);

	if (it != conVars.end())
	{
		it->second.value = value;

		return true;
	}

	return false;
}

void Console::ConsoleLoop()
{
	if (bIsRunning)
		return;

	bIsRunning = true;

	HANDLE hConsoleBuf = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
	SetConsoleMode(hConsoleBuf, ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

	HWND hConsole = GetConsoleWindow();

	INPUT_RECORD inputRecord;
	DWORD noEventsRead;

	std::string buffer;
	buffer.reserve(256);

	ConCmds::const_iterator result;
	ConVars::iterator varResult;

	while (bIsRunning)
	{
		if (ReadConsoleInput(hStdIn, &inputRecord, 1, &noEventsRead))
		{
			if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
			{
				// Process console input if any key other than backspace or any of the modifier keys was pressed
				if (inputRecord.Event.KeyEvent.uChar.AsciiChar != VK_BACK &&
					!(inputRecord.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | LEFT_CTRL_PRESSED 
						| RIGHT_CTRL_PRESSED | RIGHT_ALT_PRESSED | SHIFT_PRESSED)))

				{
					// Delete console input
					//FlushConsoleInputBuffer(hStdIn);

					// Write entered character to input buffer if it isn't the return key
					if (inputRecord.Event.KeyEvent.uChar.AsciiChar != VK_RETURN)
						WriteConsoleInput(hStdIn, &inputRecord, 1, &noEventsRead);
					
					// Change console screen to input buffer
					SetConsoleActiveScreenBuffer(hConsoleBuf);

					// Wait for input from user
					std::getline(std::cin, buffer);

					size_t cmdPos = buffer.find_first_not_of(VK_SPACE);

					// Process the input if it contains non-spaces
					if (cmdPos != std::string::npos)
					{
						size_t argPos = buffer.find_first_of(VK_SPACE, cmdPos);

						// Convert command to lowercase
						if (argPos == std::string::npos)
							std::transform(buffer.begin(), buffer.begin(), buffer.begin(), tolower);
						else
							std::transform(buffer.begin(), buffer.begin() + argPos, buffer.begin(), tolower);

						result = conCmds.find(buffer.substr(cmdPos, argPos - cmdPos));
						varResult = conVars.find(buffer.substr(cmdPos, argPos - cmdPos));

						// Process console command if found
						if (result != conCmds.end())
						{
							// Call delegate using string argument parameter
							if (argPos != std::string::npos)
								result->second.delegate(buffer.substr(argPos + 1, buffer.length() - argPos - 1));
							// Call delegate using empty string argument parameter
							else
								result->second.delegate("");
						}
						// Process console variable if found
						else if (varResult != conVars.end())
						{
							ConVars::iterator varResult = conVars.find(buffer.substr(cmdPos, argPos - cmdPos));

							// Remove trailing spaces
							std::string strValue = buffer.substr(argPos + 1, buffer.length() - argPos - 1);
							CleanArgument(strValue);

							if (varResult != conVars.end())
							{
								// Print the variable's value if there are no arguments after the command
								if (argPos == std::string::npos || strValue.empty())
								{
									Log() << ColourCode::Magenta << varResult->first << ColourCode::White << " = " << ColourCode::Magenta << varResult->second.value 
										<< ColourCode::White <<  " (default: " << varResult->second.defaultValue << ")\n";
								}
								else
								{
									varResult->second.value = strValue;

									Log() << "Set variable " << ColourCode::Magenta << varResult->first << ColourCode::White
										<< " to " << ColourCode::Magenta << strValue << "\n";
								}
							}	
						}
						else
						{
							Log(LogType::Error) << ColourCode::BrightWhite << buffer.substr(cmdPos, argPos - cmdPos) << ColourCode::BrightRed << " is not a recognised command or variable.\n";
						}
					}

					// Restore log screen
					SetConsoleActiveScreenBuffer(hStdOut);
				}
			}
		}
	}
}

void Console::CmdTest(std::string argument)
{
	// Remove trailing spaces
	CleanArgument(argument);

	if (argument.empty())
		Log(LogType::Error) << "You didn't enter an argument for the command test function!\n";
	else
		Log() << ColourCode::Green << argument << " (" << argument.length() << ")\n";
}

ColourCode operator+(ColourCode lhs, const ColourCode &rhs)
{
	return static_cast<ColourCode>(static_cast<char>(lhs) | static_cast<char>(rhs) << 4);
}