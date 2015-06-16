/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Renderbuffer.h"
#include "impl-opengl/openglRenderbuffer.h"
#include "impl-opengl/openglRenderContext.h"

namespace wntr {

	std::shared_ptr<Renderbuffer> Renderbuffer::Create(renderbuffer_id id, unsigned width, unsigned height, TEXTURE_FORMAT format, const RenderContext* context) {
		switch (context->getApi()) {
		case API::OPENGL:
			return std::make_shared<openglRenderbuffer>(id, width, height, format, static_cast<const openglRenderContext*>(context));
		default:
			return nullptr;
		}
	}

	Renderbuffer::Renderbuffer(renderbuffer_id id, API api)
	: Texture(api), _id(id) {

	}

	renderbuffer_id Renderbuffer::getRenderbufferId() const {
		return _id;
	}

}