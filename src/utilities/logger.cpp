#include "logger.h"

Logger* Logger::pLogger = nullptr;

const char* Logger::strLog = "Log:";
const char* Logger::strWarning = "Warning:";
const char* Logger::strError = "Error:";

Logger::Logger()
{
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// Store console state
	GetConsoleScreenBufferInfo(hStdOut, &csbi);
}

Logger::~Logger()
{
	delete pLogger;
}

Logger& Logger::operator<<(ColourCode code)
{
	// Set text colour
	GetConsoleScreenBufferInfo(hStdOut, &colourCsbi);
	SetConsoleTextAttribute(hStdOut, static_cast<char>(code));

	return Log();
}

Logger& Logger::operator<<(const char *pMessage)
{
	std::cerr << pMessage;

	// Restore console state to original
	SetConsoleTextAttribute(hStdOut, csbi.wAttributes);

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
		std::cerr << strLog << ' ';
		break;
	case LogType::Warning:
		*pLogger << ColourCode::BrightYellow;
		std::cerr << strWarning << ' ';
		break;
	case LogType::Error:
		*pLogger << ColourCode::BrightRed;
		std::cerr << strError << ' ';
		break;
	default:
		break;
	}

	return *pLogger;
}

ColourCode operator+(ColourCode lhs, const ColourCode &rhs)
{
	return static_cast<ColourCode>(static_cast<char>(lhs) | static_cast<char>(rhs) << 4);;
}