/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLDEPTHSTENCILSTATE_H_
#define _WNTR_OPENGLDEPTHSTENCILSTATE_H_

#include "../DepthStencilState.h"

namespace wntr {
    
    class openglRenderContext;
    
	class openglDepthStencilState : public DepthStencilState {
	public:
#ifndef _MSC_VER
        openglDepthStencilState(openglDepthStencilState&&) = default;
        openglDepthStencilState& operator=(openglDepthStencilState&&) = default;
#endif
        openglDepthStencilState(const openglDepthStencilState&) = delete;
        openglDepthStencilState& operator=(const openglDepthStencilState&) = delete;
        
        explicit openglDepthStencilState(const DepthStencilConfig& config, const openglRenderContext* context);
        virtual ~openglDepthStencilState() = default;
        
        virtual void setConfig(const DepthStencilConfig& config, const RenderContext* context) override;
        
	protected:
        
	private:
        
	};
    
}

#endif