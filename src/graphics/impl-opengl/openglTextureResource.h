/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLTEXTURERESOURCE_H_
#define _WNTR_OPENGLTEXTURERESOURCE_H_

#include "../TextureResource.h"

namespace wntr {
    
    class openglRenderContext;
    
	class openglTextureResource : public TextureResource {
    public:
#ifndef _MSC_VER
        openglTextureResource(openglTextureResource&&) = default;
        openglTextureResource& operator=(openglTextureResource&&) = default;
#endif
        openglTextureResource(const openglTextureResource&) = delete;
        openglTextureResource& operator=(const openglTextureResource&) = delete;
        
        explicit openglTextureResource(unsigned width, unsigned height, TEXTURE_FORMAT format, USAGE usage, const openglRenderContext* context);
        virtual ~openglTextureResource();
        
        unsigned getOpenGLId() const;
        
    protected:
        
    private:
        unsigned _opengl_id;
	};
    
}

#endif