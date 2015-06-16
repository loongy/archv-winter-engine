/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef _WIN32

#pragma once
#ifndef _WNTR_WIN32APPLICATION_H_
#define _WNTR_WIN32APPLICATION_H_

#include "../Application.h"

namespace wntr {

	class win32Application : public Application {
	public:
#ifndef _MSC_VER
		win32Application(win32Application&&) = default;
		win32Application& operator=(win32Application&&) = default;
#endif
		win32Application(const win32Application&) = delete;
		win32Application& operator=(const win32Application&) = delete;

		explicit win32Application() = default;
		virtual ~win32Application() = default;

		virtual void update() override;
		virtual void kill() override;

	protected:

	private:

	};

}

#endif
#endif