#pragma once

// Encapsulates a game loop
class Screen
{
public:
	Screen();
	~Screen();

	void Initialise();
	void ProcessInput();
	void Update();
	void Draw();
private:

};