#include "logger.h"

#include <algorithm>
#include <chrono>

Logger* Logger::pLogger = nullptr;
HANDLE Logger::hStdOut = nullptr;

char* const Logger::strWarning = "Warning: ";
char* const Logger::strError = "Error: ";
bool Logger::bIsRunning = false;
ConCmds Logger::conCmds = {};
ConVars Logger::conVars = {};

Logger::Logger()
{
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// Store console state
	//GetConsoleScreenBufferInfo(hStdOut, &csbi);
}

Logger::~Logger()
{
	delete pLogger;
}

Logger& Logger::operator<<(ColourCode code)
{
	// Set text colour
	SetConsoleTextAttribute(hStdOut, static_cast<char>(code));

	// Return a singleton pointer (should never be nullptr)
	return *pLogger;
}

Logger& Logger::Log(LogType type)
{
	// Create an instance of pLogger if it doesn't already exist
	if (pLogger == nullptr)
		pLogger = new Logger();

	// Print timestamp
	// TO-DO: Move this into a function
	tm *timeInfo;
	char buffer[80];
	time_t rawTime;
	time(&rawTime);
	timeInfo = localtime(&rawTime);

	SetConsoleTextAttribute(hStdOut, static_cast<char>(ColourCode::White));
	strftime(buffer, 80, "[%H:%M:%S] ", timeInfo);
	std::cerr << buffer;

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

	return *pLogger;
}

bool Logger::AddCommand(std::string cmdName, FuncPtrS funcPtr)
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

void Logger::CleanArgument(std::string &argument)
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

bool Logger::AddVar(std::string varName)
{
	return AddVar(varName, "");
}

bool Logger::AddVar(std::string varName, std::string value)
{
	// Convert variable name to lowercase
	std::transform(varName.begin(), varName.end(), varName.begin(), tolower);

	std::pair<ConVars::const_iterator, bool> result = conVars.insert(ConVar(varName, value));

	return result.second;
}

ConVar::first_type Logger::GetVar(std::string varName)
{
	// Convert variable name to lowercase
	std::transform(varName.begin(), varName.end(), varName.begin(), tolower);

	ConVars::const_iterator it = conVars.find(varName);

	return it != conVars.end() ? it->second : "";
}

bool Logger::SetVar(std::string varName, std::string value)
{
	// Convert variable name to lowercase
	std::transform(varName.begin(), varName.end(), varName.begin(), tolower);

	ConVars::iterator it = conVars.find(varName);

	if (it != conVars.end())
	{
		it->second = value;

		return true;
	}

	return false;
}

void Logger::ConsoleLoop()
{
	if (bIsRunning)
		return;

	bIsRunning = true;

	HANDLE hConsoleBuf = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
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
				if (inputRecord.Event.KeyEvent.uChar.AsciiChar == VK_RETURN)
				{
					// Delete console input
					FlushConsoleInputBuffer(hStdIn);

					// Change console screen to input buffer
					SetConsoleActiveScreenBuffer(hConsoleBuf);

					// Wait for input from user
					std::getline(std::cin, buffer);

					size_t actualStart = buffer.find_first_not_of(VK_SPACE);

					// Process the input if it contains non-spaces
					if (actualStart != std::string::npos)
					{
						// Remove prefix spaces before command/variable
						//buffer.erase(0, buffer.find_first_not_of(VK_SPACE));

						// Convert command to lowercase
						size_t argPos = buffer.find_first_of(VK_SPACE, actualStart);

						if (argPos == std::string::npos)
							std::transform(buffer.begin(), buffer.begin(), buffer.begin(), tolower);
						else
							std::transform(buffer.begin(), buffer.begin() + argPos, buffer.begin(), tolower);

						result = conCmds.find(buffer.substr(actualStart, argPos - actualStart));
						varResult = conVars.find(buffer.substr(actualStart, argPos - actualStart));

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
							ConVars::iterator varResult = conVars.find(buffer.substr(actualStart, argPos - actualStart));

							if (varResult != conVars.end())
							{
								// Remove trailing spaces
								std::string strValue = buffer.substr(argPos + 1, buffer.length() - argPos - 1);
								CleanArgument(strValue);

								varResult->second = strValue;

								Log() << "Set variable " << ColourCode::Magenta << varResult->first << ColourCode::White
									<< " to " << ColourCode::Magenta << varResult->second << "\n";
							}	
						}
						else
						{
							Log(LogType::Error) << "Unknown command!\n";
						}
					}

					// Restore log screen
					SetConsoleActiveScreenBuffer(hStdOut);
				}
			}
		}
	}
}

void Logger::CmdTest(std::string argument)
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