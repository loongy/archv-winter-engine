/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Uniform.h"
#include "impl-opengl/openglRenderContext.h"
#include "impl-opengl/openglUniform.h"

namespace wntr {
    
    std::shared_ptr<Uniform> Uniform::Create(const void* data, unsigned size, USAGE usage, const RenderContext* context) {
        switch (context->getApi()) {
            case API::OPENGL:
                return std::make_shared<openglUniform>(data, size, usage, static_cast<const openglRenderContext*>(context));
            default:
                return nullptr;
        }
    }

	Uniform::Uniform(USAGE usage, API api)
	: _size(0), _usage(usage), _api(api) {

	}

	unsigned Uniform::getSize() const {
		return _size;
	}

	USAGE Uniform::getUsage() const {
		return _usage;
	}

	API Uniform::getApi() const {
		return _api;
	}

}