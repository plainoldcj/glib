/*
solution:	glib
project:	system
file:		print.h
author:		cj
*/

/*
this printing facility will be
replaced soon.
*/

#pragma once

#include <string>

namespace SYS {

	extern int printOffset;

	void Relocate(int offX, int offY);
	void Print(const char* format, ...); 

} // namespace SYS