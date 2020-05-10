#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <vector>

#define MEASURETIME(X)\
{\
	auto _start = std::chrono::high_resolution_clock::now();\
	X;\
	auto _end = std::chrono::high_resolution_clock::now();\
	long long _ellapsed_micros = std::chrono::duration_cast<std::chrono::microseconds> (_end - _start).count();\
	std::cout << "Execution time of '" << #X << "' : " << (float)_ellapsed_micros / 1000.0f << " ms" << std::endl;\
}

#define STARTCHRONO \
auto start = std::chrono::high_resolution_clock::now()

#define ENDCHRONO \
auto end = std::chrono::high_resolution_clock::now();\
long long ellapsed_micros = std::chrono::duration_cast<std::chrono::microseconds> (end - start).count();\

class Timer
{
private:
	std::chrono::steady_clock::time_point start;
	std::vector<long long> steps;

public:
	Timer();

	~Timer();

	void RegisterStep();
};

class Timer2
{
private:
	std::string& str;
	std::chrono::steady_clock::time_point start;

public:
	Timer2(std::string& _target);

	~Timer2();
};

//#define TIMER_ENABLED

#ifdef TIMER_ENABLED
#define TIMER(X) Timer X
#define REGISTERTIME(X) X.RegisterStep()
#else
#define TIMER(X)
#define REGISTERTIME(X)
#endif