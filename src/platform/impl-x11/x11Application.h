/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef __linux

#ifndef _WNTR_X11APPLICATION_H_
#define _WNTR_X11APPLICATION_H_

#include <unordered_map>

#include "../Application.h"

struct _XDisplay;

namespace wntr {

	class x11Window;
    
	class x11Application : public Application {
		friend x11Window;

	public:
#ifndef _MSC_VER
		x11Application(x11Application&&) = default;
		x11Application& operator=(x11Application&&) = default;
#endif
		x11Application(const x11Application&) = delete;
		x11Application& operator=(const x11Application&) = delete;

		explicit x11Application();
		virtual ~x11Application();

		virtual void update() override;
		virtual void kill() override;

	protected:

	private:
		static _XDisplay* _Display;
		static std::unordered_map<unsigned long, x11Window*> _Windows;

	};

}

#endif
#endif
