/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglRenderbuffer.h"
#include "openglRenderContext.h"
#include "openglTextureResource.h"

#include <cassert>

namespace wntr {
    
    openglRenderbuffer::openglRenderbuffer(renderbuffer_id id, unsigned width, unsigned height, TEXTURE_FORMAT format, const openglRenderContext* context)
    : Renderbuffer(id, context->getApi()) {
        assert(context);

        _resource = std::make_shared<openglTextureResource>(width, height, format, USAGE::DEFAULT, context);
        
        // store the current texture
        GLint currently_bound_texture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &currently_bound_texture);
        
        glBindTexture(GL_TEXTURE_2D, _resource->getOpenGLId());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        GLenum internal_format;
        GLenum upload_format;
        GLenum upload_data_type;
        switch (_resource->getPixelFormat()) {
            case TEXTURE_FORMAT::R_UBYTE:
                internal_format = GL_R8;
                upload_format = GL_RED;
                upload_data_type = GL_UNSIGNED_BYTE;
                break;
            case TEXTURE_FORMAT::RGB_UBYTE:
                internal_format = GL_RGB8;
                upload_format = GL_RGB;
                upload_data_type = GL_UNSIGNED_BYTE;
                break;
            case TEXTURE_FORMAT::BGR_UBYTE:
                internal_format = GL_RGB8;
                upload_format = GL_BGR;
                upload_data_type = GL_UNSIGNED_BYTE;
                break;
            case TEXTURE_FORMAT::RGBA_UBYTE:
                internal_format = GL_RGBA8;
                upload_format = GL_RGBA;
                upload_data_type = GL_UNSIGNED_BYTE;
                break;
            case TEXTURE_FORMAT::BGRA_UBYTE:
                internal_format = GL_RGBA8;
                upload_format = GL_BGRA;
                upload_data_type = GL_UNSIGNED_BYTE;
                break;
            case TEXTURE_FORMAT::R_FLOAT16:
                internal_format = GL_R16F;
                upload_format = GL_RED;
                upload_data_type = GL_FLOAT;
                break;
            case TEXTURE_FORMAT::RGB_FLOAT16:
                internal_format = GL_RGB16F;
                upload_format = GL_RGB;
                upload_data_type = GL_FLOAT;
                break;
            case TEXTURE_FORMAT::BGR_FLOAT16:
                internal_format = GL_RGB16F;
                upload_format = GL_BGR;
                upload_data_type = GL_FLOAT;
                break;
            case TEXTURE_FORMAT::RGBA_FLOAT16:
                internal_format = GL_RGBA16F;
                upload_format = GL_RGBA;
                upload_data_type = GL_FLOAT;
                break;
            case TEXTURE_FORMAT::BGRA_FLOAT16:
                internal_format = GL_RGBA16F;
                upload_format = GL_BGRA;
                upload_data_type = GL_FLOAT;
                break;
            case TEXTURE_FORMAT::R_FLOAT32:
                internal_format = GL_R32F;
                upload_format = GL_RED;
                upload_data_type = GL_FLOAT;
                break;
            case TEXTURE_FORMAT::RGB_FLOAT32:
                internal_format = GL_RGB32F;
                upload_format = GL_RGB;
                upload_data_type = GL_FLOAT;
                break;
            case TEXTURE_FORMAT::BGR_FLOAT32:
                internal_format = GL_RGB32F;
                upload_format = GL_BGR;
                upload_data_type = GL_FLOAT;
                break;
            case TEXTURE_FORMAT::RGBA_FLOAT32:
                internal_format = GL_RGBA32F;
                upload_format = GL_RGBA;
                upload_data_type = GL_FLOAT;
                break;
            case TEXTURE_FORMAT::BGRA_FLOAT32:
                internal_format = GL_RGBA32F;
                upload_format = GL_BGRA;
                upload_data_type = GL_FLOAT;
                break;
            default:
                throw std::logic_error("error: openglRenderbuffer::openglRenderbuffer: encountered an unsupported data format");
        }
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, _resource->getWidth(), _resource->getHeight(), 0, upload_format, upload_data_type, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        // restore the previoustexture
        glBindTexture(GL_TEXTURE_2D, currently_bound_texture);
    }
    
    std::shared_ptr<TextureResource> openglRenderbuffer::getResource() const {
        return _resource;
    }
    
}