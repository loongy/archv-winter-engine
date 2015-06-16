/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_COLOR_H_
#define _WNTR_COLOR_H_

#include "Component.h"

namespace wntr {

	class Color : public Component {
	public:
		static std::shared_ptr<Color> Create(float red, float green, float blue, float alpha=1.0f);

#ifndef _MSC_VER
		Color(Color&&) = default;
		Color& operator=(Color&&) = default;
#endif
		Color(const Color&) = delete;
		Color& operator=(const Color&) = delete;

		explicit Color(float red, float green, float blue, float alpha);
		virtual ~Color() = default;

		virtual const component_hierarchy& hierarchy() const override;

		float getRed() const;
		float getGreen() const;
		float getBlue() const;
		float getAlpha() const;
		const glm::vec4& getColor() const;

	protected:

	private:
		static const component_hierarchy _Hierarchy;

		glm::vec4 _color;

	};

}

#endif