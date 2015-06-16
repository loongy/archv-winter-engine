/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include "RenderPass.h"

#include <graphics/Program.h>
#include <graphics/RenderContext.h>
#include <graphics/Uniform.h>

namespace wntr {

    void RenderPass::bind (RenderContext* context) {
        assert(context);
        context->setAttributeArray(nullptr);
        context->setProgram(_program);
        context->setBlendState(_blend_state);
        context->setDepthStencilState(_depth_stencil_state);
        //context->setRasterizerState(_rasterizer_state);
    }
    
    void RenderPass::setUniform(uniform_id id, const void* data, unsigned size, const RenderContext* context) {
        setUniform(id, data, size, USAGE::DEFAULT, context);
    }
    
    void RenderPass::setUniform(uniform_id id, const void* data, unsigned size, USAGE usage, const RenderContext* context) {
        
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
    
    void RenderPass::setTexture(texture_id id, const std::shared_ptr<Texture>& texture, const RenderContext* context) {
        assert(_program);
        _program->bindTexture(texture, id, context);
    }
    
    void RenderPass::setSampler(sampler_id id, const std::shared_ptr<Sampler>& sampler, const RenderContext* context) {
        assert(_program);
        _program->bindSampler(sampler, id, context);
    }

    
}