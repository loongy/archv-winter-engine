/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLATTRIBUTEARRAY_H_
#define _WNTR_OPENGLATTRIBUTEARRAY_H_

#include "../AttributeArray.h"

namespace wntr {
    
    class openglRenderContext;

	class openglAttributeArray : public AttributeArray {
	public:
#ifndef _MSC_VER
        openglAttributeArray(openglAttributeArray&&) = default;
        openglAttributeArray& operator=(openglAttributeArray&&) = default;
#endif
        openglAttributeArray(const openglAttributeArray&) = delete;
        openglAttributeArray& operator=(const openglAttributeArray&) = delete;
        
        explicit openglAttributeArray(POLYGON_TYPE poly_type, const openglRenderContext* context);
        virtual ~openglAttributeArray();
        
        virtual void align(const Program* program, const RenderContext* context) override;
        
        unsigned getOpenGLId() const;
        
	protected:
        
	private:
        unsigned _opengl_id;
        
	};

}

#endif