/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglBlendState.h"
#include "openglRenderContext.h"

#include <cassert>

namespace wntr {
    
    openglBlendState::openglBlendState(const BlendConfig& config, const openglRenderContext* context)
    : BlendState(config, context->getApi()) {
        assert(context);
    }
    
    void openglBlendState::setConfig (const BlendConfig& config, const RenderContext* context) {
        assert(context);
        _config = config;
    }
    
}