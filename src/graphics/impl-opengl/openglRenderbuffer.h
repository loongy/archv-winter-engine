/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLRENDERBUFFER_H_
#define _WNTR_OPENGLRENDERBUFFER_H_

#include "../Renderbuffer.h"

namespace wntr {
    
    class openglRenderContext;
    class openglTextureResource;
    
	class openglRenderbuffer : public Renderbuffer {
    public:
#ifndef _MSC_VER
        openglRenderbuffer(openglRenderbuffer&&) = default;
        openglRenderbuffer& operator=(openglRenderbuffer&&) = default;
#endif
        openglRenderbuffer(const openglRenderbuffer&) = delete;
        openglRenderbuffer& operator=(const openglRenderbuffer&) = delete;
        
        explicit openglRenderbuffer(renderbuffer_id id, unsigned width, unsigned height, TEXTURE_FORMAT format, const openglRenderContext* context);
        virtual ~openglRenderbuffer() = default;
        
        virtual std::shared_ptr<TextureResource> getResource() const override;
        
    protected:
        
    private:
        std::shared_ptr<openglTextureResource> _resource;
	};
    
}

#endif