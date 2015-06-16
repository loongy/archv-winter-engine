/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLGEOMETRYSHADER_H_
#define _WNTR_OPENGLGEOMETRYSHADER_H_

#include "../GeometryShader.h"

namespace wntr {
    
    class openglRenderContext;
    
	class openglGeometryShader : public GeometryShader {
	public:
#ifndef _MSC_VER
		openglGeometryShader(openglGeometryShader&&) = default;
		openglGeometryShader& operator= (openglGeometryShader&&) = default;
#endif
		openglGeometryShader(const openglGeometryShader&) = delete;
		openglGeometryShader& operator= (const openglGeometryShader&) = delete;
        
		explicit openglGeometryShader(const std::string& code, const openglRenderContext* context);
		virtual ~openglGeometryShader();
        
		unsigned getOpenGLId() const;
        
	protected:
        
	private:
		unsigned _opengl_id;
        
	};
    
}

#endif