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
#ifndef _WNTR_OPENGLOSXRENDERCONTEXT_H_
#define _WNTR_OPENGLOSXRENDERCONTEXT_H_

#include "../openglRenderContext.h"

namespace wntr {
    
    class openglosxCocoaRenderContext;
    class osxWindow;
    
    class openglosxRenderContext : public openglRenderContext {
    public:
#ifndef _MSC_VER
		openglosxRenderContext(openglosxRenderContext&&) = default;
		openglosxRenderContext& operator= (openglosxRenderContext&&) = default;
#endif
		openglosxRenderContext(const openglosxRenderContext&) = delete;
		openglosxRenderContext& operator= (const openglosxRenderContext&) = delete;
        
		explicit openglosxRenderContext(const std::shared_ptr<osxWindow>& window, unsigned width, unsigned height, bool vsync, bool fullscreen);
		virtual ~openglosxRenderContext();
        
		virtual void swapBuffers() override;
        
		virtual void lock() override;
		virtual void unlock() override;
        
        virtual void setVsync(bool vsync) override;
        
		const std::shared_ptr<osxWindow>& getWindow() const;
        
    protected:
        
    private:
        void getOpenGLFunctionPointers();
        
        std::shared_ptr<osxWindow> _window;
        openglosxCocoaRenderContext* _gl;
        
    };
    
}

#endif
#endif
