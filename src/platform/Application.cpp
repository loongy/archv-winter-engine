/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Application.h"

#ifdef _WIN32
#include "impl-win32/win32Application.h"
#elif __APPLE__
#include "impl-osx/osxApplication.h"
#elif __linux
#include "impl-x11/x11Application.h"
#endif

#include <stdexcept>

namespace wntr {

	std::shared_ptr<Application> Application::Create() {
		if (_IsCreated) {
			throw std::runtime_error("error: Application::Application: an instance of Appplication has already been created");
		}
		_IsCreated = true;
#ifdef _WIN32
		return std::make_shared<win32Application>();
#elif __APPLE__
		return std::make_shared<osxApplication>();
#elif __linux
		return std::make_shared<x11Application>();
#endif
	}

	bool Application::_IsCreated = false;

}