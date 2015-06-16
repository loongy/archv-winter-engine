/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLUNIFORM_H_
#define _WNTR_OPENGLUNIFORM_H_

#include "../Uniform.h"

namespace wntr {
    
	class openglUniform : public Uniform {
    public:
#ifndef _MSC_VER
        openglUniform(openglUniform&&) = default;
        openglUniform& operator=(openglUniform&&) = default;
#endif
        openglUniform(const openglUniform&) = delete;
        openglUniform& operator=(const openglUniform&) = delete;
        
        explicit openglUniform(const void* data, unsigned size, USAGE usage, const openglRenderContext* context);
        virtual ~openglUniform();
        
        virtual void setData(const void* data, unsigned size, const RenderContext* context) override;
        
        unsigned getOpenGLId() const;
        
    protected:
        
    private:
        unsigned _opengl_id;
        
	};
    
}

#endif