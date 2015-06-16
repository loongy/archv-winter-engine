/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglAttributeBuffer.h"
#include "openglRenderContext.h"

#include <cassert>

namespace wntr {
    
    openglAttributeBuffer::openglAttributeBuffer(const void* data, unsigned count, attribute_id id, FORMAT format, USAGE usage, const openglRenderContext* context)
    : AttributeBuffer(count, id, format, usage, context->getApi()) {
        assert(context);
		context->glGenBuffers(1, &_opengl_id);
        setData(data, count, format, context);
    }
    
    openglAttributeBuffer::~openglAttributeBuffer() {
        glDeleteBuffers(1, &_opengl_id);
        _opengl_id = 0;
    }
    
    void openglAttributeBuffer::setData(const void* data, unsigned count, FORMAT format, const RenderContext* context) {
        assert(context);
        if (context->getApi() != API::OPENGL)
            throw std::invalid_argument("error: openglAttributeBuffer::setData: expected RenderContext to implement API::OPENGL");
        
        if (data) {
            _count = count;
            _stride = (unsigned)sizeOfFormat(format);
        }
        else {
            _count = 0;
            _stride = 0;
        }
		_format = format;
        
        auto opengl_context = static_cast<const openglRenderContext*>(context);
        if (_id == ATTRIBUTE_ELEMENTS) {
            GLint currently_bound_buffer;
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currently_bound_buffer);
            
            opengl_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _opengl_id);
            switch (getUsage()) {
                case USAGE::DEFAULT:
                case USAGE::IMMUTABLE:
                    opengl_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count*_stride, data, GL_STATIC_DRAW);
                    break;
                case USAGE::MUTABLE:
                    opengl_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count*_stride, data, GL_DYNAMIC_DRAW);
                    break;
                default:
                    throw std::logic_error("error: openglAttributeBuffer::setData: encountered an unsupported data usage");
            }

            opengl_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currently_bound_buffer);
        }
        else {
            GLint currently_bound_buffer;
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currently_bound_buffer);
            
            opengl_context->glBindBuffer(GL_ARRAY_BUFFER, _opengl_id);
            switch (getUsage()) {
                case USAGE::DEFAULT:
                case USAGE::IMMUTABLE:
                    opengl_context->glBufferData(GL_ARRAY_BUFFER, _count*_stride, data, GL_STATIC_DRAW);
                    break;
                case USAGE::MUTABLE:
                    opengl_context->glBufferData(GL_ARRAY_BUFFER, _count*_stride, data, GL_DYNAMIC_DRAW);
                    break;
                default:
                    throw std::logic_error("error: openglAttributeBuffer::setData: encountered an unsupported data usage");
            }

            opengl_context->glBindBuffer(GL_ARRAY_BUFFER, currently_bound_buffer);
        }
    }
    
    unsigned openglAttributeBuffer::getOpenGLId() const {
        return _opengl_id;
    }
    
}