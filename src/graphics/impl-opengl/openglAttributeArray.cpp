/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglAttributeArray.h"
#include "openglAttributeBuffer.h"
#include "openglProgram.h"
#include "openglRenderContext.h"

#include <cassert>
#include <iostream>

namespace wntr {
    
    openglAttributeArray::openglAttributeArray(POLYGON_TYPE poly_type, const openglRenderContext* context)
    : AttributeArray(poly_type, context->getApi()) {
        assert(context);
        context->glGenVertexArrays(1, &_opengl_id);
    }
    
    openglAttributeArray::~openglAttributeArray() {
        glDeleteVertexArrays(1, &_opengl_id);
        _opengl_id = 0;
    }
    
    void openglAttributeArray::align(const Program* program, const RenderContext* context) {
        assert(context);
        
        if (context->getApi() != API::OPENGL)
            throw std::invalid_argument("error: openglAttributeArray::align: expected RenderContext to implement API::OPENGL");
        
        if (!program) {
            _aligned_program = nullptr;
            return;
        }
        if (program->getApi() != API::OPENGL)
            throw std::invalid_argument("error: openglAttributeArray::align: expected Program to implement API::OPENGL");
        
        if (_aligned_program != program || _alignment_required) {
			// store current array
			GLint currently_bound_vertex_array;
			GLint currently_bound_array_buffer;
			GLint currently_bound_element_array_buffer;
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currently_bound_vertex_array);
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currently_bound_array_buffer);
			glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currently_bound_element_array_buffer);

            auto opengl_context = static_cast<const openglRenderContext*>(context);
            opengl_context->glBindVertexArray(_opengl_id);
            
            for (auto attrib_id : program->getAttributes()) {
                auto attrib = getAttribute(attrib_id);
                auto attrib_loc = program->getAttributeLocation(attrib_id);
                
				if (attrib_loc < 0)
					continue;
                
                if (attrib) {
                    opengl_context->glBindBuffer(GL_ARRAY_BUFFER, static_cast<openglAttributeBuffer*>(attrib.get())->getOpenGLId());
                    opengl_context->glEnableVertexAttribArray(attrib_loc);
                    
                    GLuint  num_component = 0;
                    switch (attrib->getFormat()) {
                        case FORMAT::FLOAT:
                        case FORMAT::INT:
                        case FORMAT::UINT:
                            num_component = 1;
                            break;
                        case FORMAT::FLOAT2:
                        case FORMAT::INT2:
                        case FORMAT::UINT2:
                            num_component = 2;
                            break;
                        case FORMAT::FLOAT3:
                        case FORMAT::INT3:
                        case FORMAT::UINT3:
                            num_component = 3;
                            break;
                        case FORMAT::FLOAT4:
                        case FORMAT::INT4:
                        case FORMAT::UINT4:
                            num_component = 4;
                            break;
                        default:
                            throw std::logic_error("error: openglAttributeArray::align: encountered an unsupported data format");
                    }
                    if (attrib->getFormat() == FORMAT::INT || attrib->getFormat() == FORMAT::INT2
                     || attrib->getFormat() == FORMAT::INT3 || attrib->getFormat() == FORMAT::INT4)
                        opengl_context->glVertexAttribIPointer(attrib_loc, num_component, GL_INT, attrib->getStride(), 0);
                    else
                        opengl_context->glVertexAttribPointer(attrib_loc, num_component, GL_FLOAT, false, attrib->getStride(), 0);
                }
                else {
                    opengl_context->glBindBuffer(GL_ARRAY_BUFFER, 0);
                    opengl_context->glDisableVertexAttribArray(attrib_loc);
                }
            }
            
            if (_element_buffer) {
                opengl_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<openglAttributeBuffer*>(_element_buffer.get())->getOpenGLId());
            }
            
            _aligned_program = program;
            _alignment_required = false;
            
            // restore previous current array state
			opengl_context->glBindVertexArray(currently_bound_vertex_array); 
			opengl_context->glBindBuffer(GL_ARRAY_BUFFER, currently_bound_array_buffer);
			opengl_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currently_bound_element_array_buffer);
        }
    }
    
    unsigned openglAttributeArray::getOpenGLId() const {
        return _opengl_id;
    }
    
}