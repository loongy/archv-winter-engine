/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLVERTEXSHADER_H_
#define _WNTR_OPENGLVERTEXSHADER_H_

#include "../VertexShader.h"

namespace wntr {
    
    class openglRenderContext;

	class openglVertexShader : public VertexShader {
	public:
#ifndef _MSC_VER
		openglVertexShader(openglVertexShader&&) = default;
		openglVertexShader& operator= (openglVertexShader&&) = default;
#endif
		openglVertexShader(const openglVertexShader&) = delete;
		openglVertexShader& operator= (const openglVertexShader&) = delete;

		explicit openglVertexShader(const std::string& code, const openglRenderContext* context);
		virtual ~openglVertexShader();

		unsigned getOpenGLId() const;

	protected:

	private:
		unsigned _opengl_id;

	};

}

#endif