/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_APPLICATION_H_
#define _WNTR_APPLICATION_H_

#include <memory>

namespace wntr {

	class Application {
	public:
		static std::shared_ptr<Application> Create();

#ifndef _MSC_VER
		Application(Application&&) = default;
		Application& operator=(Application&&) = default;
#endif
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		explicit Application() = default;
		virtual ~Application() = default;

		virtual void update() = 0;
		virtual void kill() = 0;

	protected:

	private:
		static bool _IsCreated;

	};

}

#endif