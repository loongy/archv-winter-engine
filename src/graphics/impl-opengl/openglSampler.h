/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLSAMPLER_H_
#define _WNTR_OPENGLSAMPLER_H_

#include "../Sampler.h"

namespace wntr {
    
    class openglRenderContext;
    
	class openglSampler : public Sampler {
	public:
#ifndef _MSC_VER
        openglSampler(openglSampler&&) = default;
        openglSampler& operator=(openglSampler&&) = default;
#endif
        openglSampler(const openglSampler&) = delete;
        openglSampler& operator=(const openglSampler&) = delete;
        
        explicit openglSampler(const SamplerConfig& config, const openglRenderContext* context);
        virtual ~openglSampler();
        
        virtual void setConfig(const SamplerConfig& config, const RenderContext* context) override;
        
        unsigned getOpenGLId() const;
        
    protected:
        
    private:
        void setSamplerParameters(const openglRenderContext* context);
        
        unsigned _opengl_id;
	};
    
}

#endif