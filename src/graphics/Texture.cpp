/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Texture.h"
#include "impl-opengl/openglRenderContext.h"
#include "impl-opengl/openglTexture.h"

namespace wntr {
    
    std::shared_ptr<Texture> Texture::Create (const void* data, unsigned width, unsigned height, TEXTURE_FORMAT format, USAGE usage, const RenderContext* context) {
        switch (context->getApi()) {
            case API::OPENGL:
                return std::make_shared<openglTexture>(data, width, height, format, usage, static_cast<const openglRenderContext*>(context));
            default:
                return nullptr;
        }
    }

	Texture::Texture(API api) 
	: _api(api) {

	}

	API Texture::getApi() const {
		return _api;
	}

}