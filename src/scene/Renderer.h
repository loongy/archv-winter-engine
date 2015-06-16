/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_RENDERER_H_
#define _WNTR_RENDERER_H_

#include <common/Manager.h>

namespace wntr {
    
    class RenderContext;
    class RenderScheme;
    
    class Renderer : public Manager {
    public:
#ifndef _MSC_VER
		Renderer(Renderer&&) = default;
		Renderer& operator=(Renderer&&) = default;
#endif
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
        
        explicit Renderer(const std::shared_ptr<RenderContext>& context);
		virtual ~Renderer() = default;
        
        virtual void draw() = 0;
        
        const std::shared_ptr<RenderContext>& getContext() const;
        
    protected:
        std::shared_ptr<RenderContext> _context;
        
    private:
    };
    
}

#endif
