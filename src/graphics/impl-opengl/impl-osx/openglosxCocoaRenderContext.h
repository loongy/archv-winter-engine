/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef __APPLE__

#pragma once
#ifndef _WNTR_OPENGLOSXCOCOARENDERCONTEXT_H_
#define _WNTR_OPENGLOSXCOCOARENDERCONTEXT_H_

#include <memory>

namespace wntr {
    
    class osxWindow;
    
    class openglosxCocoaRenderContext {
    public:
        openglosxCocoaRenderContext(openglosxCocoaRenderContext&&) = default;
		openglosxCocoaRenderContext& operator=(openglosxCocoaRenderContext&&) = default;
        
		openglosxCocoaRenderContext(const openglosxCocoaRenderContext&) = delete;
		openglosxCocoaRenderContext& operator=(const openglosxCocoaRenderContext&) = delete;
        
        explicit openglosxCocoaRenderContext(const std::shared_ptr<osxWindow>& window, unsigned width, unsigned height, bool vsync, bool fullscreen);
		virtual ~openglosxCocoaRenderContext();
        
        void swapBuffers();
        
        void lock();
        void unlock();
        
        void setVSync(bool vsync);
        
        void* self();
        
    protected:
        
    private:
        void* _nsview;
        
    };
    
}

#endif
#endif
