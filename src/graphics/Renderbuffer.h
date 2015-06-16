/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_RENDERBUFFER_H_
#define _WNTR_RENDERBUFFER_H_

#include "Texture.h"

namespace wntr {

	class Renderbuffer : public Texture {
	public:
		static std::shared_ptr<Renderbuffer> Create(renderbuffer_id id, unsigned width, unsigned height, TEXTURE_FORMAT format, const RenderContext* context);

#ifndef _MSC_VER
		Renderbuffer(Renderbuffer&&) = default;
		Renderbuffer& operator= (Renderbuffer&&) = default;
#endif
		Renderbuffer(const Renderbuffer&) = delete;
		Renderbuffer& operator= (const Renderbuffer&) = delete;

		explicit Renderbuffer(renderbuffer_id id, API api);
		virtual ~Renderbuffer() = default;

		renderbuffer_id getRenderbufferId() const;

	protected:

	private:
		renderbuffer_id _id;

	};

}

#endif