#include "logger.h"

Logger* Logger::pLogger = nullptr;

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
	SetConsoleTextAttribute(hStdOut, static_cast<WORD>(code));

	return Log();
}

Logger& Logger::operator<<(const char *pMessage)
{
	std::cout << pMessage;

	// Restore console state to original
	SetConsoleTextAttribute(hStdOut, csbi.wAttributes);

	return Log();
}

Logger& Logger::Log()
{
	if (pLogger == nullptr)
		pLogger = new Logger();

	return *pLogger;
}

ColourCode operator+(ColourCode lhs, const ColourCode &rhs)
{
	uint16_t a = static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs) << 4;

	return static_cast<ColourCode>(a);
}