#include "game.h"

int main()
{
	Matrix4x4 test(1.0f);
	Matrix4x4 test2(1.3f);
	Matrix4x4 test3;

	test3 = test * test2;


	Game game;

	game.Start();
	
	return 0;
}