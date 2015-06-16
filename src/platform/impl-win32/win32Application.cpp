/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef _WIN32

#include "win32Application.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

namespace wntr {

	void win32Application::update() {
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void win32Application::kill() {
		std::exit(0);
	}

}

#endif