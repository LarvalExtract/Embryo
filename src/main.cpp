#include "game.h"

int main()
{
	// TO-DO: Grab this string from somewhere else
	// TO-DO: Move this inside console class
	SetConsoleTitle("Embryo (developer console)");

	Game().Start();
	
	return 0;
}