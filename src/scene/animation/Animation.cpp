/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include "Animation.h"

namespace wntr {

	std::shared_ptr<Animation> Animation::Create(double duration, bool loop) {
		return std::make_shared<Animation>(duration, loop);
	}

	Animation::Animation(double duration, bool loop)
	: _loop(loop), _duration(duration), _total_elapse(0.0) {
	}

	const component_hierarchy& Animation::hierarchy() const {
		return _Hierarchy;
	}

	void Animation::animate(double elapse) {
		_total_elapse += elapse;
		if (_total_elapse > _duration) {
			if (_loop) {
				_total_elapse = _total_elapse - (((int)_total_elapse/(int)_duration) * _duration);
				onAnimate(elapse);
			}
			else {
				_total_elapse = _duration;
			}
		}
		else {
			onAnimate(elapse);
		}
	}

	void Animation::onAnimate(double elapse) {
	}

	double Animation::getDuration() const {
		return _duration;
	}

	double Animation::getTotalElapse() const {
		return _total_elapse;
	}

	const component_hierarchy Animation::_Hierarchy(std::vector<std::type_index>{ typeid(Component), typeid(Animation) });

}