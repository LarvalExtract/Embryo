#pragma once

#include <chrono>

class Timer
{
public:
	Timer();

	void Reset();
	double Elapsed() const;
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};