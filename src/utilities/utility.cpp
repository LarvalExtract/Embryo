#include "utility.h"

namespace Utility
{
	bool BothAreSpaces(char lhs, char rhs) 
	{ 
		return (lhs == rhs) && (lhs == ' ');
	}
}