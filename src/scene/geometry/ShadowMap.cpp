/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include "ShadowMap.h"

#include <graphics/CommonGraphics.h>
#include <graphics/Depthbuffer.h>
#include <graphics/Framebuffer.h>
#include <graphics/Renderbuffer.h>
#include <graphics/TextureResource.h>

namespace wntr {

	std::shared_ptr<ShadowMap> ShadowMap::Create(unsigned width, unsigned height, const RenderContext* context) {
		return std::make_shared<ShadowMap>(width, height, context);
	}

	ShadowMap::ShadowMap(unsigned width, unsigned height, const RenderContext* context) {
		auto shadowbuffer = Renderbuffer::Create(TEXTURE_SHADOW, width, height, TEXTURE_FORMAT::R_FLOAT32, context); assert(shadowbuffer);
		auto depthbuffer = Depthbuffer::Create(width, height, TEXTURE_FORMAT::DEPTH24, context); assert(depthbuffer);
		_framebuffer = Framebuffer::Create(1, &shadowbuffer, depthbuffer, context); assert(_framebuffer);
	}

	const component_hierarchy& ShadowMap::hierarchy() const {
		return _Hierarchy;
	}

	unsigned ShadowMap::getWidth() const {
		return _framebuffer->getRenderbuffer(TEXTURE_SHADOW)->getResource()->getWidth();
	}

	unsigned ShadowMap::getHeight() const {
		return _framebuffer->getRenderbuffer(TEXTURE_SHADOW)->getResource()->getHeight();
	}

	const std::shared_ptr<Framebuffer>& ShadowMap::getFramebuffer() const {
		return _framebuffer;
	}

	const component_hierarchy ShadowMap::_Hierarchy({ typeid(Component), typeid(ShadowMap) });

}