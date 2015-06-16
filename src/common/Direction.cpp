/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Direction.h"

namespace wntr {

	std::shared_ptr<Direction> Direction::Create(float x, float y, float z) {
		return std::make_shared<Direction>(glm::vec3(x, y, z));
	}

	std::shared_ptr<Direction> Direction::Create(const glm::vec3& direction) {
		return std::make_shared<Direction>(direction);
	}

	Direction::Direction(const glm::vec3& direction) 
	: _direction(glm::normalize(direction)) {

	}

	const component_hierarchy& Direction::hierarchy() const {
		return _Hierarchy;
	}

	const glm::vec3& Direction::getDirection() const {
		return _direction;
	}

	const component_hierarchy Direction::_Hierarchy({ typeid(Component), typeid(Direction) });

}