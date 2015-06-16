/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#pragma once
#ifndef _WNTR_ANIMATION_H_
#define _WNTR_ANIMATION_H_

#include <common/Component.h>

namespace wntr {

	class Animation : public Component {
	public:
		static std::shared_ptr<Animation> Create(double duration, bool loop = false);

#ifndef _MSC_VER
		Animation(Animation&&) = default;
		Animation& operator=(Animation&&) = default;
#endif
		Animation(const Animation&) = delete;
		Animation& operator=(const Animation&) = delete;

		explicit Animation(double duration, bool loop);
		virtual ~Animation() = default;

		virtual const component_hierarchy& hierarchy() const override;

		virtual void animate(double elapse);

		virtual void onAnimate(double elapse);

		double getDuration() const;
		double getTotalElapse() const;

	protected:

	private:
		static const component_hierarchy _Hierarchy;

		bool _loop;
		double _duration;
		double _total_elapse;

	};

}

#endif