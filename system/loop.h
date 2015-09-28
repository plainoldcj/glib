/*
solution:	glib
project:	system
file:		loop.h
author:		cj
*/

#pragma once

#include "timer.h"

namespace SYS {

	template<typename FUNC>
	void EnterLoop(FUNC& frameFunc) {
		bool done = false;
		MSG message;

		SYS::Timer timer;
		float secsPassed = 0.0f;

		while(!done) {
			timer.Start();

			if(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
				if(WM_QUIT == message.message) {
					done = true;
				}

				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			frameFunc(secsPassed);

			secsPassed = timer.Stop();
		}
	}

} // namespace SYS