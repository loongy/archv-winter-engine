/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLBLENDSTATE_H_
#define _WNTR_OPENGLBLENDSTATE_H_

#include "../BlendState.h"

namespace wntr {
    
    class openglRenderContext;
    
	class openglBlendState : public BlendState {
    public:
#ifndef _MSC_VER
        openglBlendState(openglBlendState&&) = default;
        openglBlendState& operator=(openglBlendState&&) = default;
#endif
        openglBlendState(const openglBlendState&) = delete;
        openglBlendState& operator=(const openglBlendState&) = delete;
        
        explicit openglBlendState(const BlendConfig& config, const openglRenderContext* context);
        virtual ~openglBlendState() = default;
        
        virtual void setConfig (const BlendConfig& config, const RenderContext* context) override;
        
    protected:
        
    private:
        
	};
    
}

#endif