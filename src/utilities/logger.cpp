#include "logger.h"

Logger* Logger::pLogger = nullptr;
HANDLE Logger::hStdOut = nullptr;

char* const Logger::strLog = "Log:";
char* const Logger::strWarning = "Warning:";
char* const Logger::strError = "Error:";

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

	return Log();
}

Logger& Logger::Log(LogType type)
{
	// Create an instance of pLogger if it doesn't already exist
	if (pLogger == nullptr)
		pLogger = new Logger();

	// Appends a log type tag to the start of the message and colours the line
	switch (type)
	{
	case LogType::Log:
		SetConsoleTextAttribute(hStdOut, static_cast<char>(ColourCode::White));
		std::cerr << strLog << ' ';
		break;
	case LogType::Warning:
		SetConsoleTextAttribute(hStdOut, static_cast<char>(ColourCode::BrightYellow));
		std::cerr << strWarning << ' ';
		break;
	case LogType::Error:
		SetConsoleTextAttribute(hStdOut, static_cast<char>(ColourCode::BrightRed));
		std::cerr << strError << ' ';
		break;
	default:
		SetConsoleTextAttribute(hStdOut, static_cast<char>(ColourCode::White));
		break;
	}

	return *pLogger;
}

ColourCode operator+(ColourCode lhs, const ColourCode &rhs)
{
	return static_cast<ColourCode>(static_cast<char>(lhs) | static_cast<char>(rhs) << 4);;
}