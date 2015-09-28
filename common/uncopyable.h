/*
solution:	glib
project:	common
file:		uncopyable.h
author:		cj
*/

#pragma once

namespace GEN {

	class Uncopyable {
	private:
		Uncopyable(const Uncopyable& other);
		Uncopyable& operator=(const Uncopyable& other);
	public:
		Uncopyable(void) { }
	};

} // namespace GEN