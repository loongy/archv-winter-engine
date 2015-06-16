/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLDEPTHBUFFER_H_
#define _WNTR_OPENGLDEPTHBUFFER_H_

#include "../Depthbuffer.h"

namespace wntr {
    
    class openglRenderContext;
    class openglTextureResource;
    
	class openglDepthbuffer : public Depthbuffer {
    public:
#ifndef _MSC_VER
        openglDepthbuffer(openglDepthbuffer&&) = default;
        openglDepthbuffer& operator=(openglDepthbuffer&&) = default;
#endif
        openglDepthbuffer(const openglDepthbuffer&) = delete;
        openglDepthbuffer& operator=(const openglDepthbuffer&) = delete;
        
        explicit openglDepthbuffer(unsigned width, unsigned height, TEXTURE_FORMAT format, const openglRenderContext* context);
        virtual ~openglDepthbuffer() = default;
        
        virtual std::shared_ptr<TextureResource> getResource() const override;
        
    protected:
        
    private:
        std::shared_ptr<openglTextureResource> _resource;
        
	};
    
}

#endif