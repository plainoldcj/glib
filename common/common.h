/*
solution:	glib
project:	common
file:		common.h
author:		cj
*/

#pragma once

#include <fstream>

#define COM_assert(expr) \
	if(!(expr)) { \
		COM::log << "assertion failed: " << #expr << " in " \
				 << __FILE__ << ", " << __LINE__ << std::endl; \
		throw COM::AssertionException(); \
	}

#define MAX_TOKEN 512

typedef struct ctoken_s {
	struct ctoken_s* next;
	char string[MAX_TOKEN];
	float f;
	int i;
} ctoken_t;

int COM_Tokenize(ctoken_t** tokens, const char* string);
void COM_FreeTokens(ctoken_t* tokens);

#define MAX_HASH 1024

static int COM_Hash(const char* str) {
	int len = strlen(str);
	int hash = 0;
	int i;

	for(i = 0; i < len; ++i) {
		hash = ((hash << 8) + str[i]) % MAX_HASH;
	}
	return hash;
}

namespace COM {

	struct FileNotFoundException : std::exception { };
	struct IOException : std::exception { };
	struct InvalidFormatException : std::exception { };
	struct AssertionException : std::exception { };
	struct GLException : std::exception { };
 
	typedef unsigned char uchar_t;
	typedef unsigned char byte_t;

	struct LogStream : std::ofstream { 
		LogStream(void) {
			open("logfile.txt");
		}
	};

	extern LogStream log;

	template<typename TYPE>
	inline TYPE Min(TYPE lhp, TYPE rhp) {
		if(lhp < rhp) return lhp;
		return rhp;
	}

	template<typename TYPE>
	inline TYPE Max(TYPE lhp, TYPE rhp) {
		if(lhp < rhp) return rhp;
		return lhp;
	}

	template<typename TYPE>
	inline TYPE Abs(TYPE value) {
		if(0 <= value) return value;
		return -value;
	}

	template<typename TYPE>
	inline void Clamp(TYPE& val, TYPE min, TYPE max) {
		if(min > val) val = min;
		if(max < val) val = max;
	} 

	template<typename TYPE>
	inline TYPE Clamp(TYPE val, TYPE min, TYPE max) {
		if(min > val) return min;
		if(max < val) return max;
		return val;
	} 

	inline bool AlmostEqual(float lhp, float rhp) {
		const float eps = 0.0001f;
		return eps > Abs(lhp - rhp);
	}

	static float Rand(float min, float max) {
		const int scale = 1000;
		const float f = 1.0f / scale;
		int r = std::rand() % scale;
		return min + (max - min) * r * f;
	}

} // namespace COM