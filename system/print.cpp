/*
solution:	glib
project:	system
file:		print.cpp
author:		cj
*/

#include "opengl.h"
#include "print.h"

// todo
#include "../common/common.h"

#define PRINT_BUFFER_SIZE 2048
#define PRINT_SPACING 2

namespace SYS {

	static const int fontListBase = 1000;

	int printPosX = 0;
	int printPosY = 0;
	int printOffset = 0;

	void Relocate(int offX, int offY) {
		printPosX = offX;
		printPosY = offY;
		glx.WindowPos2i(printPosX, printPosY);
	}

	void Print(const char* format, ...) {
		char buffer[PRINT_BUFFER_SIZE];

		va_list args;
		va_start(args, format);
		vsprintf_s(buffer, PRINT_BUFFER_SIZE, format, args);

		glListBase(fontListBase);
	
		/*
		int i = 0;
		while('\0' != buffer[i]) {
			if('\n' == buffer[i]) {
				printPosY -= printOffset;
				glx.WindowPos2i(printPosX, printPosY);
			} else {
				glCallLists(1, GL_UNSIGNED_BYTE, buffer + i);
			}
			++i;
		}
		*/
		glCallLists(strlen(buffer), GL_UNSIGNED_BYTE, buffer);

		printPosY -= printOffset;
		glx.WindowPos2i(printPosX, printPosY);
	}

	void RenderingContext::PrepareFont(void) {
		SelectObject(_deviceContext.GetNativeHandle(), GetStockObject(SYSTEM_FONT));

		/*
		FIXED one, zero;
		one.fract = zero.fract = 0;
		one.value = 1;
		zero.value = 0;

		MAT2 mat = { 
			one, zero,
			zero, one
		};
		GLYPHMETRICS metrics;
		if(GDI_ERROR == GetGlyphOutline(_deviceContext.GetNativeHandle(), 0, GGO_METRICS,
			&metrics, 0, NULL, &mat))
		{
			COM::log << "RenderingContext::PrepareFont: GetGlyphOutline failed" << std::endl;
			printOffset = 20;
		} else {
			printOffset = metrics.gmBlackBoxY;
		}
		*/
		TEXTMETRIC metric;
		GetTextMetrics(_deviceContext.GetNativeHandle(), &metric);
		printOffset = metric.tmHeight + PRINT_SPACING;

		if(FALSE == wglUseFontBitmaps(_deviceContext.GetNativeHandle(), 0, 255, fontListBase)) {
			COM::log << "RenderingContext::PrepareFont: wglUseFontBitmaps failed with code "
				<< GetLastError() << std::endl;
		}
	}

} // namespace SYS