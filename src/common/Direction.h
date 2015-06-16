/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_DIRECTION_H_
#define _WNTR_DIRECTION_H_

#include "Component.h"

namespace wntr {

	class Direction : public Component {
	public:
		static std::shared_ptr<Direction> Create(float x, float y, float z);
		static std::shared_ptr<Direction> Create(const glm::vec3& direction);

#ifndef _MSC_VER
		Direction(Direction&&) = default;
		Direction& operator=(Direction&&) = default;
#endif
		Direction(const Direction&) = delete;
		Direction& operator=(const Direction&) = delete;

		explicit Direction(const glm::vec3& direction);
		virtual ~Direction() = default;

		virtual const component_hierarchy& hierarchy() const override;

		const glm::vec3& getDirection() const;

	protected:

	private:
		static const component_hierarchy _Hierarchy;

		glm::vec3 _direction;

	};

}

#endif