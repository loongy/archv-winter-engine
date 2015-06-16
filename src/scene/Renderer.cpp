/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include <cassert>

#include <common/Entity.h>
#include <common/Transform3D.h>

#include "Renderer.h"

namespace wntr {
    
    Renderer::Renderer(const std::shared_ptr<RenderContext>& context)
    : Manager(), _context(context) {
        assert(context);
    }
    
}