/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Program.h"
#include "impl-opengl/openglFragmentShader.h"
#include "impl-opengl/openglGeometryShader.h"
#include "impl-opengl/openglProgram.h"
#include "impl-opengl/openglRenderContext.h"
#include "impl-opengl/openglVertexShader.h"

#include <cassert>

namespace wntr {
    
    std::shared_ptr<Program> Program::Create(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<GeometryShader>& gs, const std::shared_ptr<FragmentShader>& fs, const RenderContext* context) {
        switch (context->getApi()) {
            case API::OPENGL:
                return std::make_shared<openglProgram>(std::static_pointer_cast<openglVertexShader>(vs), std::static_pointer_cast<openglGeometryShader>(gs), std::static_pointer_cast<openglFragmentShader>(fs), (const openglRenderContext*)context);
            default:
                return nullptr;
        }
    }

	Program::Program(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<GeometryShader>& gs, const std::shared_ptr<FragmentShader>& fs, API api)
	: _vertex_shader(vs), _geometry_shader(gs), _fragment_shader(fs), _api(api) {
        assert(_vertex_shader);
        assert(_vertex_shader->getApi() == _api);
        assert(_fragment_shader);
        assert(_fragment_shader->getApi() == _api);
        assert(_geometry_shader ? _geometry_shader->getApi() == _api : true);
	}

	const std::shared_ptr<VertexShader>& Program::getVertexShader() const {
		return _vertex_shader;
	}
    
    const std::shared_ptr<GeometryShader>& Program::getGeometryShader() const {
		return _geometry_shader;
	}

	const std::shared_ptr<FragmentShader>& Program::getFragmentShader() const {
		return _fragment_shader;
	}

	API Program::getApi() const {
		return _api;
	}

}