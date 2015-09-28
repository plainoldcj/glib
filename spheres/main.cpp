/*
solution:	glib
project:	spheres
file:		main.cpp
author:		cj
*/

#include <Windows.h>
#include "../common/config.h"
#include "../system/loop.h"
#include "window.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
	try {
		COM::Config& config = COM::Config::Instance();
		int width = config.Get("width", 800);
		int height = config.Get("height", 600);

		Window window(width, height);
		SYS::EnterLoop(window);
	} catch(std::exception&) {
		MessageBox(NULL, TEXT("caught exception,\nview logfile for more information"), TEXT("Error"), MB_OK);
	}

	return 0;
}