/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include "TransformAnimation3D.h"

namespace wntr {

	std::shared_ptr<TransformAnimation3D> TransformAnimation3D::Create(const std::shared_ptr<Transform3D>& transform, double duration, bool loop) {
		return std::make_shared<TransformAnimation3D>(nullptr, transform, duration, loop);
	}

	std::shared_ptr<TransformAnimation3D> TransformAnimation3D::Create(const std::function<void(const std::shared_ptr<Transform3D>, double)>& on_transform_animation, const std::shared_ptr<Transform3D>& transform, double duration, bool loop) {
		return std::make_shared<TransformAnimation3D>(on_transform_animation, transform, duration, loop);
	}

	TransformAnimation3D::TransformAnimation3D(const std::function<void(const std::shared_ptr<Transform3D>, double)>& on_transform_animation, const std::shared_ptr<Transform3D>& transform, double duration, bool loop)
	: Animation(duration, loop), _transform(transform), onTransformAnimation(on_transform_animation) {
	}

	const component_hierarchy& TransformAnimation3D::hierarchy() const {
		return _Hierarchy;
	}

	void TransformAnimation3D::onAnimate(double elapse) {
		if (onTransformAnimation) {
			onTransformAnimation(_transform, elapse);
		}
	}

	const component_hierarchy TransformAnimation3D::_Hierarchy({typeid(Component), typeid(Animation), typeid(TransformAnimation3D)});

}