/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Framebuffer.h"
#include "Renderbuffer.h"
#include "impl-opengl/openglFramebuffer.h"
#include "impl-opengl/openglRenderContext.h"

#include <cassert>

namespace wntr {

	std::shared_ptr<Framebuffer> Framebuffer::Create(unsigned num_renderbuffers, std::shared_ptr<Renderbuffer>* renderbuffers, const std::shared_ptr<Depthbuffer>& depthbuffer, const RenderContext* context) {
		switch (context->getApi()) {
		case API::OPENGL:
			return std::make_shared<openglFramebuffer>(num_renderbuffers, renderbuffers, depthbuffer, static_cast<const openglRenderContext*>(context));
		default:
			return nullptr;
		}
	}

	Framebuffer::Framebuffer(unsigned num_renderbuffers, std::shared_ptr<Renderbuffer>* renderbuffers, const std::shared_ptr<Depthbuffer>& depthbuffer, API api)
	: _api(api) {
		assert(renderbuffers || !num_renderbuffers);

		for (unsigned i = 0; i < num_renderbuffers; i++) {
			assert(renderbuffers[i]);
			_renderbuffers[renderbuffers[i]->getRenderbufferId()] = renderbuffers[i];
		}

		_depthbuffer = _depthbuffer;
	}



	std::shared_ptr<Renderbuffer> Framebuffer::getRenderbuffer(renderbuffer_id id) const {
		auto it = _renderbuffers.find(id);
		if (it != _renderbuffers.end())
			return it->second;
		return nullptr;
	}

	const std::shared_ptr<Depthbuffer>& Framebuffer::getDepthbuffer() const {
		return _depthbuffer;
	}

	API Framebuffer::getApi() const {
		return _api;
	}

}