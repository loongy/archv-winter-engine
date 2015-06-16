/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#pragma once
#ifndef _WNTR_ANIMATOR_H_
#define _WNTR_ANIMATOR_H_

#include <common/Entity.h>
#include <common/Manager.h>

#include "Animation.h"

namespace wntr {

	class Animator : public Manager {
	public:
		static std::shared_ptr<Animator> Create();

#ifndef _MSC_VER
		Animator(Animator&&) = default;
		Animator& operator=(Animator&&) = default;
#endif
		Animator(const Animator&) = delete;
		Animator& operator=(const Animator&) = delete;

		explicit Animator() = default;
		virtual ~Animator() = default;

		virtual void animate(double elapse);

	protected:

	private:

	};

}

#endif