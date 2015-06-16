/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLTEXTURE_H_
#define _WNTR_OPENGLTEXTURE_H_

#include "../Texture.h"

namespace wntr {
    
    class openglRenderContext;
    class openglTextureResource;
    
	class openglTexture : public Texture {
    public:
#ifndef _MSC_VER
        openglTexture (openglTexture&&) = default;
        openglTexture& operator= (openglTexture&&) = default;
#endif
        openglTexture(const openglTexture&) = delete;
        openglTexture& operator=(const openglTexture&) = delete;
        
        explicit openglTexture(const void* data, unsigned width, unsigned height, TEXTURE_FORMAT format, USAGE usage, const openglRenderContext* context);
        virtual ~openglTexture();
        
        virtual std::shared_ptr<TextureResource> getResource() const override;
        
    protected:
        
    private:
        std::shared_ptr<openglTextureResource> _resource;
	};
    
}

#endif