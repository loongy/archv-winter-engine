/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLFRAGMENTSHADER_H_
#define _WNTR_OPENGLFRAGMENTSHADER_H_

#include "../FragmentShader.h"

namespace wntr {
    
    class openglRenderContext;

	class openglFragmentShader : public FragmentShader {
	public:
#ifndef _MSC_VER
		openglFragmentShader(openglFragmentShader&&) = default;
		openglFragmentShader& operator= (openglFragmentShader&&) = default;
#endif
		openglFragmentShader(const openglFragmentShader&) = delete;
		openglFragmentShader& operator= (const openglFragmentShader&) = delete;

		explicit openglFragmentShader(const std::string& code, const openglRenderContext* context);
		virtual ~openglFragmentShader();

		unsigned getOpenGLId() const;

	protected:

	private:
		unsigned _opengl_id;

	};

}

#endif