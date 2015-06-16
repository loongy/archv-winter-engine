/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLATTRIBUTEBUFFER_H_
#define _WNTR_OPENGLATTRIBUTEBUFFER_H_

#include "../AttributeBuffer.h"

namespace wntr {
    
    class openglRenderContext;
    
	class openglAttributeBuffer : public AttributeBuffer {
    public:
#ifndef _MSC_VER
        openglAttributeBuffer(openglAttributeBuffer&&) = default;
        openglAttributeBuffer& operator=(openglAttributeBuffer&&) = default;
#endif
        openglAttributeBuffer(const openglAttributeBuffer&) = delete;
        openglAttributeBuffer& operator=(const openglAttributeBuffer&) = delete;
        
        explicit openglAttributeBuffer(const void* data, unsigned count, attribute_id id, FORMAT format, USAGE usage, const openglRenderContext* context);
        virtual ~openglAttributeBuffer();
        
        virtual void setData(const void* data, unsigned count, FORMAT format, const RenderContext* context) override;
        
        unsigned getOpenGLId() const;
        
    protected:
        
    private:    
        unsigned _opengl_id;
	};
    
}

#endif