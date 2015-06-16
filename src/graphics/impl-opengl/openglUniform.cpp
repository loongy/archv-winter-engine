/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglRenderContext.h"
#include "openglUniform.h"

#include <cassert>

namespace wntr {
    
    openglUniform::openglUniform(const void* data, unsigned size, USAGE usage, const openglRenderContext* context)
    : Uniform(usage, context->getApi()) {
        assert(context);
        context->glGenBuffers(1, &_opengl_id);
        setData(data, size, context);
    }
    
    openglUniform::~openglUniform() {
        glDeleteBuffers(1, &_opengl_id);
        _opengl_id = 0;
    }
    
    void openglUniform::setData(const void* data, unsigned size, const RenderContext* context) {
        assert(context);
        
        if (context->getApi() != API::OPENGL)
            throw std::invalid_argument("error: openglUniform::setData: expected RenderContext to implement API::OPENGL");
        
        auto opengl_context = static_cast<const openglRenderContext*>(context);
        
        if (data)
            _size = size;
        else
            _size = 0;
        
        // store current uniform
        GLint currently_bound_uniform;
        glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &currently_bound_uniform);
        
        opengl_context->glBindBuffer(GL_UNIFORM_BUFFER, _opengl_id);
        switch (_usage) {
            case USAGE::DEFAULT:
            case USAGE::IMMUTABLE:
                opengl_context->glBufferData(GL_UNIFORM_BUFFER, _size, data, GL_STATIC_DRAW);
                break;
            case USAGE::MUTABLE:
                opengl_context->glBufferData(GL_UNIFORM_BUFFER, _size, data, GL_DYNAMIC_DRAW);
                break;
            default:
                throw std::logic_error("error: openglUniform::setData: encountered an unsupported data usage");
        }
        
        // restore uniform
        opengl_context->glBindBuffer(GL_UNIFORM_BUFFER, currently_bound_uniform);
    }
    
    unsigned openglUniform::getOpenGLId() const {
        return _opengl_id;
    }
    
}