/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#pragma once
#ifndef _WNTR_RENDEREFFECT_H_
#define _WNTR_RENDEREFFECT_H_

#include <common/utility.h>
#include <graphics/CommonGraphics.h>

#include <unordered_map>

namespace wntr {
    
    class AttributeArray;
    class BlendState;
    class DepthStencilState;
    class Entity;
    class Program;
    class RasterizerState;
    class RenderContext;
    class Sampler;
    class Texture;
    class Uniform;
    
    class RenderEffect {
    public:
#ifndef _MSC_VER
		RenderEffect(RenderEffect&&) = default;
		RenderEffect& operator=(RenderEffect&&) = default;
#endif
		RenderEffect(const RenderEffect&) = delete;
		RenderEffect& operator=(const RenderEffect&) = delete;
        
        explicit RenderEffect(const RenderContext* context);
		virtual ~RenderEffect() = default;
        
        virtual void bind(RenderContext* context);
        virtual void draw(RenderContext* context);
        
        void setUniform(uniform_id id, const void* data, unsigned size, const RenderContext* context);
        void setUniform(uniform_id id, const void* data, unsigned size, USAGE usage, const RenderContext* context);
        void setTexture(texture_id id, const std::shared_ptr<Texture>& texture, const RenderContext* context);
        void setSampler(sampler_id id, const std::shared_ptr<Sampler>& sampler, const RenderContext* context);
        
        template<typename T>
        void setUniform(uniform_id id, const T& data, const RenderContext* context);
        template<typename T>
        void setUniform(uniform_id id, const T* data, const RenderContext* context);
        template<typename T>
        void setUniform(uniform_id id, const std::shared_ptr<T> data, const RenderContext* context);
        template<typename T>
        void setUniform(uniform_id id, const T& data, USAGE usage, const RenderContext* context);
        template<typename T>
        void setUniform(uniform_id id, const T* data, USAGE usage, const RenderContext* context);
        template<typename T>
        void setUniform(uniform_id id, const std::shared_ptr<T> data, USAGE usage, const RenderContext* context);
        
    protected:
        std::shared_ptr<AttributeArray> _array;
        std::unordered_map<uniform_id, std::shared_ptr<Uniform>> _uniforms;
        
        std::shared_ptr<Program> _program;
        std::shared_ptr<BlendState> _blend_state;
        std::shared_ptr<DepthStencilState> _depth_stencil_state;
        std::shared_ptr<RasterizerState> _rasterizer_state;
        
    private:
        
    };
    
    template<typename T>
    void RenderEffect::setUniform(uniform_id id, const T& data, const RenderContext* context) {
        setUniform(id, (const void*)&data, sizeof(T), context);
    }
    
    template<typename T>
    void RenderEffect::setUniform(uniform_id id, const std::shared_ptr<T> data, const RenderContext* context) {
        setUniform(id, (const void*)data.get(), sizeof(T), context);
    }
    
    template<typename T>
    void RenderEffect::setUniform(uniform_id id, const T* data, const RenderContext* context) {
        setUniform(id, (const void*)data, sizeof(T), context);
    }
    
    template<typename T>
    void RenderEffect::setUniform(uniform_id id, const T& data, USAGE usage, const RenderContext* context) {
        setUniform(id, (const void*)&data, sizeof(T), usage, context);
    }
    
    template<typename T>
    void RenderEffect::setUniform(uniform_id id, const std::shared_ptr<T> data, USAGE usage, const RenderContext* context) {
        setUniform(id, (const void*)data.get(), sizeof(T), usage, context);
    }
    
    template<typename T>
    void RenderEffect::setUniform(uniform_id id, const T* data, USAGE usage, const RenderContext* context) {
        setUniform(id, (const void*)data, sizeof(T), usage, context);
    }

}

#endif