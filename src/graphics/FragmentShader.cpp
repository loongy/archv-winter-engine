/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "FragmentShader.h"
#include "impl-opengl/openglFragmentShader.h"
#include "impl-opengl/openglRenderContext.h"

namespace wntr {
    
    std::shared_ptr<FragmentShader> FragmentShader::Create(const std::string& code, const RenderContext* context) {
        switch (context->getApi()) {
            case API::OPENGL:
                return std::make_shared<openglFragmentShader>(code, static_cast<const openglRenderContext*>(context));
            default:
                return nullptr;
        }
    }

	FragmentShader::FragmentShader(API api)
	: _api(api) {

	}

	API FragmentShader::getApi() const {
		return _api;
	}

}