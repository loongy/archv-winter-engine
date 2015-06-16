/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Depthbuffer.h"
#include "impl-opengl/openglDepthbuffer.h"
#include "impl-opengl/openglRenderContext.h"

namespace wntr {

	std::shared_ptr<Depthbuffer> Depthbuffer::Create(unsigned width, unsigned height, TEXTURE_FORMAT format, const RenderContext* context) {
		switch (context->getApi()) {
		case API::OPENGL:
			return std::make_shared<openglDepthbuffer>(width, height, format, static_cast<const openglRenderContext*>(context));
		default:
			return nullptr;
		}
	}

	Depthbuffer::Depthbuffer(API api) 
	: Texture(api) {

	}

}