/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglRenderContext.h"
#include "openglTexture.h"
#include "openglTextureResource.h"

#include <cassert>

namespace wntr {
    
    openglTexture::openglTexture(const void* data, unsigned width, unsigned height, TEXTURE_FORMAT format, USAGE usage, const openglRenderContext* context)
    : Texture(context->getApi())  {
        assert(context);

        _resource = std::make_shared<openglTextureResource>(width, height, format, usage, context);
        assert(_resource);
        
        // save the current texture
        GLint currently_bound_texture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &currently_bound_texture);
        
        GLenum internal_format;
        GLenum upload_format;
        GLenum upload_data_type;
        glBindTexture(GL_TEXTURE_2D, _resource->getOpenGLId());
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
                throw std::logic_error("error: openglTexture::openglTexture: encountered an unsupported data format");
        }
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, _resource->getWidth(), _resource->getHeight(), 0, upload_format, upload_data_type, data);
        context->glGenerateMipmap(GL_TEXTURE_2D);
        
        // restore the previously used texture
        glBindTexture(GL_TEXTURE_2D, currently_bound_texture);
    }
    
    openglTexture::~openglTexture() {
        
    }
    
    std::shared_ptr<TextureResource> openglTexture::getResource() const {
        return _resource;
    }
    
}