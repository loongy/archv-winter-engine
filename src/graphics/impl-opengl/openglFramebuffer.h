/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLFRAMEBUFFER_H_
#define _WNTR_OPENGLFRAMEBUFFER_H_

#include "../Framebuffer.h"

namespace wntr {

	class openglRenderContext;
    
	class openglFramebuffer : public Framebuffer {
    public:
#ifndef _MSC_VER
        openglFramebuffer(openglFramebuffer&&) = default;
        openglFramebuffer& operator=(openglFramebuffer&&) = default;
#endif
        openglFramebuffer(const openglFramebuffer&) = delete;
        openglFramebuffer& operator=(const openglFramebuffer&) = delete;
        
        explicit openglFramebuffer(unsigned num_renderbuffers, std::shared_ptr<Renderbuffer>* renderbuffers, const std::shared_ptr<Depthbuffer>& depthbuffer, const openglRenderContext* context);
        virtual ~openglFramebuffer();
        
        unsigned getMaxWidth() const;
        unsigned getMaxHeight() const;
        
        unsigned getOpenGLId() const;
        
    protected:
        
    private:
        unsigned _max_width;
        unsigned _max_height;
        
        unsigned _opengl_id;
        
	};
    
}

#endif