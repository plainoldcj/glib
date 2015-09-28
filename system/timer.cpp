/*
solution:	glib
project:	system
file:		timer.cpp
author:		cj
*/

#include "timer.h"

namespace SYS {

	Timer::Timer(void) {
		if(!QueryPerformanceFrequency((LARGE_INTEGER*)&_frequency)) {
			throw PerformanceCounterException();
		}
	}

	void Timer::Start(void) {
		QueryPerformanceCounter((LARGE_INTEGER*)&_start);
	}

	float Timer::Stop(void) {
		LONGLONG stop;
		QueryPerformanceCounter((LARGE_INTEGER*)&stop);
		return (float)(stop - _start) / _frequency;
	}

} // namespace SYS