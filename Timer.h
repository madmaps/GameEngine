#ifndef __TIMER__
#define __TIMER__
#include <chrono>

class Timer
{
public:
	Timer();
	~Timer();
	void start();
	void stop();
	double getTimeLapse();
protected:
	std::chrono::high_resolution_clock::time_point timePoint1;
	std::chrono::high_resolution_clock::time_point timePoint2;
	bool stopTimer;
private:
};
#endif
