/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include <cassert>

#include "Color.h"

namespace wntr {

	std::shared_ptr<Color> Color::Create(float red, float green, float blue, float alpha) {
		return std::make_shared<Color>(red, green, blue, alpha);
	}

	Color::Color(float red, float green, float blue, float alpha) 
	: Component(), _color(red, green, blue, alpha) {
        assert(red >= 0.0f);
        assert(green >= 0.0f);
        assert(blue >= 0.0f);
        assert(alpha >= 0.0f);
	}

	const component_hierarchy& Color::hierarchy() const {
		return _Hierarchy;
	}

	float Color::getRed() const {
		return _color[0];
	}

	float Color::getGreen() const {
		return _color[1];
	}

	float Color::getBlue() const {
		return _color[2];
	}

	float Color::getAlpha() const {
		return _color[3];
	}

	const glm::vec4& Color::getColor() const {
		return _color;
	}

	const component_hierarchy Color::_Hierarchy({ typeid(Component), typeid(Color) });

}