/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_DEPTHBUFFER_H_
#define _WNTR_DEPTHBUFFER_H_

#include "Texture.h"

namespace wntr {

	class Depthbuffer : public Texture {
	public:
		static std::shared_ptr<Depthbuffer> Create(unsigned width, unsigned height, TEXTURE_FORMAT format, const RenderContext* context);

#ifndef _MSC_VER
		Depthbuffer(Depthbuffer&&) = default;
		Depthbuffer& operator= (Depthbuffer&&) = default;
#endif
		Depthbuffer(const Depthbuffer&) = delete;
		Depthbuffer& operator= (const Depthbuffer&) = delete;

		explicit Depthbuffer(API api);
		virtual ~Depthbuffer() = default;

	protected:
        
	private:
        
	};

}

#endif