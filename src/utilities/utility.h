#pragma once

#include <string>

namespace Utility
{
	// Used by std::unique to eliminate both starting and trailing spaces in a string
	extern bool BothAreSpaces(char lhs, char rhs);
}