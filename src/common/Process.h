/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#pragma once
#ifndef _WNTR_PROCESS_H_
#define _WNTR_PROCESS_H_

#include "utility.h"

namespace wntr {

	class Entity;

	class Process {
	public:
#ifndef _MSC_VER
		Process(Process&&) = default;
		Process& operator=(Process&&) = default;
#endif
		Process(const Process&) = delete;
		Process& operator=(const Process&) = delete;

		explicit Process() = default;
		virtual ~Process() = default;

		virtual void process(const shared_vector<Entity>& entities);

		virtual void onProcess(const std::shared_ptr<Entity>& entity) = 0;
        virtual bool onFilter(const std::shared_ptr<Entity>& entity) const;

	protected:

	private:

	};

};

#endif