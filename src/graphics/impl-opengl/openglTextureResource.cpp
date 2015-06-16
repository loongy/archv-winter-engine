/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglRenderContext.h"
#include "openglTextureResource.h"

#include <cassert>

namespace wntr {
    
    openglTextureResource::openglTextureResource(unsigned width, unsigned height, TEXTURE_FORMAT format, USAGE usage, const openglRenderContext* context)
    : TextureResource(width, height, format, usage, context->getApi()) {
        assert(context);
        glGenTextures(1, &_opengl_id);
    }
    
    openglTextureResource::~openglTextureResource() {
        glDeleteTextures(1, &_opengl_id);
        _opengl_id = 0;
    }
    
    unsigned openglTextureResource::getOpenGLId() const {
        return _opengl_id;
    }
    
}