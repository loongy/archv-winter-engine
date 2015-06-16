/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_FRAMEBUFFER_H_
#define _WNTR_FRAMEBUFFER_H_

#include "CommonGraphics.h"

#include <memory>
#include <unordered_map>

namespace wntr {

	class Depthbuffer;
	class Renderbuffer;
	class RenderContext;

	class Framebuffer {
	public:
		static std::shared_ptr<Framebuffer> Create(unsigned num_renderbuffers, std::shared_ptr<Renderbuffer>* renderbuffers, const std::shared_ptr<Depthbuffer>& depthbuffer, const RenderContext* context);

#ifndef _MSC_VER
		Framebuffer(Framebuffer&&) = default;
		Framebuffer& operator= (Framebuffer&&) = default;
#endif
		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator= (const Framebuffer&) = delete;

		explicit Framebuffer(unsigned num_renderbuffers, std::shared_ptr<Renderbuffer>* renderbuffers, const std::shared_ptr<Depthbuffer>& depthbuffer, API api);
		virtual ~Framebuffer() = default;

		
		std::shared_ptr<Renderbuffer> getRenderbuffer(renderbuffer_id id) const;
		const std::shared_ptr<Depthbuffer>& getDepthbuffer() const;

		API getApi() const;

	protected:
		std::unordered_map<renderbuffer_id, std::shared_ptr<Renderbuffer>> _renderbuffers;
		std::shared_ptr<Depthbuffer> _depthbuffer;

		const API _api;

	private:
	};

}

#endif