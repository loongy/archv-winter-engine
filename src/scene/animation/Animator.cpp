/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include "Animator.h"

namespace wntr {

	std::shared_ptr<Animator> Animator::Create() {
		return std::make_shared<Animator>();
	}

	void Animator::animate(double elapse) {
		for (const auto& entity : _entities) {
			auto anim = entity->get<Animation>();
			if (anim)
				anim->animate(elapse);
		}
	}

}