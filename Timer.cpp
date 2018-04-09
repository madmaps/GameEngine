#include "Timer.h"
#include <ctime>
#include <ratio>
#include <chrono>

Timer::Timer()
{
	stopTimer = true;
}

Timer::~Timer()
{
}

void Timer::start()
{
	stopTimer = false;
	timePoint1 = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
	stopTimer = true;
	timePoint1 = timePoint2;
}

double Timer::getTimeLapse()
{
	if(!stopTimer)
	{
		timePoint2 = std::chrono::high_resolution_clock::now();
	}
	std::chrono::duration<double> timeLapse = std::chrono::duration_cast<std::chrono::duration<double>>(timePoint2 - timePoint1);
	timePoint1 = timePoint2;
	return timeLapse.count();
}
