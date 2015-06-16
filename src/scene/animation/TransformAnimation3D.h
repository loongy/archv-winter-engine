/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#pragma once
#ifndef _WNTR_TRANSFORMANIMATION3D_H_
#define _WNTR_TRANSFORMANIMATION3D_H_

#include "Animation.h"

namespace wntr {
	
	class Transform3D;

	class TransformAnimation3D : public Animation {
	public:
		static std::shared_ptr<TransformAnimation3D> Create(const std::shared_ptr<Transform3D>& transform, double duration, bool loop = false);
		static std::shared_ptr<TransformAnimation3D> Create(const std::function<void(const std::shared_ptr<Transform3D>, double)>& on_transform_animation, const std::shared_ptr<Transform3D>& transform, double duration, bool loop = false);

#ifndef _MSC_VER
		TransformAnimation3D(TransformAnimation3D&&) = default;
		TransformAnimation3D& operator=(TransformAnimation3D&&) = default;
#endif
		TransformAnimation3D(const TransformAnimation3D&) = delete;
		TransformAnimation3D& operator=(const TransformAnimation3D&) = delete;

		explicit TransformAnimation3D(const std::function<void(const std::shared_ptr<Transform3D>, double)>& on_transform_animation, const std::shared_ptr<Transform3D>& transform, double duration, bool loop);
		virtual ~TransformAnimation3D() = default;

		virtual const component_hierarchy& hierarchy() const override;

		virtual void onAnimate(double elapse) override;

		std::function<void(const std::shared_ptr<Transform3D>, double)> onTransformAnimation;

	protected:

	private:
		static const component_hierarchy _Hierarchy;

		std::shared_ptr<Transform3D> _transform;

	};

}

#endif