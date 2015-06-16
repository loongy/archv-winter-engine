/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#pragma once
#ifndef _WNTR_SHADOWMAP_H_
#define _WNTR_SHADOWMAP_H_

#include <common/Component.h>
#include <common/utility.h>

namespace wntr {

	class Framebuffer;
	class RenderContext;

	class ShadowMap : public Component {
	public:
		static std::shared_ptr<ShadowMap> Create(unsigned width, unsigned height, const RenderContext* context);

#ifndef _MSC_VER
		ShadowMap(ShadowMap&&) = default;
		ShadowMap& operator=(ShadowMap&&) = default;
#endif
		ShadowMap(const ShadowMap&) = delete;
		ShadowMap& operator=(const ShadowMap&) = delete;

		explicit ShadowMap(unsigned width, unsigned height, const RenderContext* context);
		virtual ~ShadowMap() = default;

		virtual const component_hierarchy& hierarchy() const override;

		unsigned getWidth() const;
		unsigned getHeight() const;

		const std::shared_ptr<Framebuffer>& getFramebuffer() const;

	protected:

	private:
		static const component_hierarchy _Hierarchy;

		std::shared_ptr<Framebuffer> _framebuffer;

	};

}

#endif