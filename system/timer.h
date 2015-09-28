/*
solution:	glib
project:	system
file:		timer.h
author:		cj
*/

#pragma once

#include <exception>
#include <Windows.h>
#include "../common/uncopyable.h"

struct PerformanceCounterException : std::exception {
	const char* what(void) throw() {
		return "no performance counter available";
	}
};

namespace SYS {

	class Timer : private GEN::Uncopyable {
	private:
		LONGLONG _frequency;
		LONGLONG _start;
	public:
		Timer(void);

		void Start(void);
		float Stop(void);
	};

} // namespace SYS