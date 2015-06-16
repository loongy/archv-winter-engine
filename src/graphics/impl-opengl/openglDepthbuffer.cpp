/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglDepthbuffer.h"
#include "openglRenderContext.h"
#include "openglTextureResource.h"

#include <cassert>

namespace wntr {
    
    openglDepthbuffer::openglDepthbuffer(unsigned width, unsigned height, TEXTURE_FORMAT format, const openglRenderContext* context)
    : Depthbuffer(context->getApi()) {
        assert(context);

        _resource = std::make_shared<openglTextureResource>(width, height, format, USAGE::DEFAULT, context);
        
        // store current texture
        GLint currently_bound_texture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &currently_bound_texture);
        
        glBindTexture(GL_TEXTURE_2D, _resource->getOpenGLId());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        GLenum internal_format;
        switch (_resource->getPixelFormat()) {
            case TEXTURE_FORMAT::DEPTH24:
                internal_format = GL_DEPTH_COMPONENT24;
                break;
            case TEXTURE_FORMAT::DEPTH24_STENCIL8:
                internal_format = GL_DEPTH_COMPONENT24;
                break;
            case TEXTURE_FORMAT::DEPTH32:
                internal_format = GL_DEPTH_COMPONENT32;
                break;
            case TEXTURE_FORMAT::DEPTH32_STENCIL8:
                internal_format = GL_DEPTH_COMPONENT32;
                break;
            default:
                throw std::logic_error("error: OpenGL32Depthbuffer::OpenGL32Depthbuffer: encountered an unsupported data format");
        }
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, _resource->getWidth(), _resource->getHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        
        // restore the previous texture
        glBindTexture(GL_TEXTURE_2D, currently_bound_texture);
    }
    
    std::shared_ptr<TextureResource> openglDepthbuffer::getResource() const {
        return _resource;
    }
    
}