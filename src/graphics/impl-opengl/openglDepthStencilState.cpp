/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglDepthStencilState.h"
#include "openglRenderContext.h"

#include <cassert>

namespace wntr {
 
    openglDepthStencilState::openglDepthStencilState(const DepthStencilConfig& config, const openglRenderContext* context)
    : DepthStencilState(config, context->getApi()) {
        assert(context);
    }
    
    
    void openglDepthStencilState::setConfig (const DepthStencilConfig& config, const RenderContext* context) {
        assert(context);
        _config = config;
    }
    
}