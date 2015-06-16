/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include "RenderEffect.h"

#include <graphics/AttributeArray.h>
#include <graphics/AttributeBuffer.h>
#include <graphics/Program.h>
#include <graphics/RenderContext.h>
#include <graphics/Uniform.h>

namespace wntr {

    RenderEffect::RenderEffect(const RenderContext* context) {
        float positions[] = {
			-1.0f, 1.0, 0.0f,
			1.0, 1.0, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
		};
		unsigned elements[] = {
			0, 1, 2,
			2, 3, 0,
		};
        
		auto position_buffer = AttributeBuffer::Create(positions, 4, ATTRIBUTE_POSITION, FORMAT::FLOAT3, USAGE::DEFAULT, context);
		auto element_buffer = AttributeBuffer::Create(elements, 6, ATTRIBUTE_ELEMENTS, FORMAT::UINT, USAGE::DEFAULT, context);
		assert(position_buffer);
        assert(element_buffer);
        
		_array = AttributeArray::Create(POLYGON_TYPE::TRIANGLE, context); assert(_array);
		_array->setAttribute(position_buffer);
		_array->setAttribute(element_buffer);
    }
    
    void RenderEffect::bind (RenderContext* context) {
        assert(context);
        context->setAttributeArray(_array);
        context->setProgram(_program);
        context->setBlendState(_blend_state);
        context->setDepthStencilState(_depth_stencil_state);
        //context->setRasterizerState(_rasterizer_state);
    }
    
    void RenderEffect::draw(RenderContext* context) {
        assert(context);
		context->setAttributeArray(_array);
		context->draw();
    }
    
    void RenderEffect::setUniform(uniform_id id, const void* data, unsigned size, const RenderContext* context) {
        setUniform(id, data, size, USAGE::DEFAULT, context);
    }
    
    void RenderEffect::setUniform(uniform_id id, const void* data, unsigned size, USAGE usage, const RenderContext* context) {
        
        auto it = _uniforms.find(id);
        if (it == _uniforms.end()) {
            _uniforms[id] = Uniform::Create(data, size, usage, context);
            assert(_uniforms[id]);
        }
        else {
            assert(it->second);
            it->second->setData(data, size, context);
        }
        
        assert(_program);
        _program->bindUniform(_uniforms[id], id, context);
    }
    
    void RenderEffect::setTexture(texture_id id, const std::shared_ptr<Texture>& texture, const RenderContext* context) {
        assert(_program);
        _program->bindTexture(texture, id, context);
    }
    
    void RenderEffect::setSampler(sampler_id id, const std::shared_ptr<Sampler>& sampler, const RenderContext* context) {
        assert(_program);
        _program->bindSampler(sampler, id, context);
    }
    
}