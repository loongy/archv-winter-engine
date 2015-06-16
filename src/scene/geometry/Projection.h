/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#pragma once
#ifndef _WNTR_PROJECTION_H_
#define _WNTR_PROJECTION_H_

#include <common/Common.h>

namespace wntr {

	class Projection : public Component {
	public:
#ifndef _MSC_VER
		Projection(Projection&&) = default;
		Projection& operator=(Projection&&) = default;
#endif
		Projection(const Projection&) = delete;
		const Projection& operator=(const Projection&) = delete;

		explicit Projection() = default;
		virtual ~Projection() = default;

		virtual const component_hierarchy& hierarchy() const override;

		virtual const glm::mat4& projection() const = 0;

	protected:

	private:
		static const component_hierarchy _Hierarchy;

	};

}

#endif
