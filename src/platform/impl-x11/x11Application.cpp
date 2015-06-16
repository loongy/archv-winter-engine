/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef __linux

#include <X11/X.h>
#include <X11/Xlib.h>

#include "x11Application.h"
#include "x11Window.h"

namespace wntr {

	x11Application::x11Application()
	: Application() {
		if (!_Display)
			_Display = XOpenDisplay(0);
	}

	x11Application::~x11Application() {
		if (_Display) {
			XCloseDisplay(_Display);
			_Display = nullptr;
		}
	}
    
	void x11Application::update() {
		assert(_Display);

		_XEvent evt;
		while(XPending(_Display)) {
			XNextEvent(_Display, &evt);
			auto it = _Windows.find(evt.xany.window);
			if (it != _Windows.end())
				it->second->processEvent(evt);
		}
	}

	void x11Application::kill() {
		if (_Display) {
			XCloseDisplay(_Display);
			_Display = nullptr;
		}
		std::exit(0);
	}

	_XDisplay* x11Application::_Display = nullptr;
	std::unordered_map<unsigned long, x11Window*> x11Application::_Windows;

}

#endif
