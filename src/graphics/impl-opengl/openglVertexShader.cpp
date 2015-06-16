/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglRenderContext.h"
#include "openglVertexShader.h"

#include <cassert>

namespace wntr {

	openglVertexShader::openglVertexShader(const std::string& code, const openglRenderContext* context)
	: VertexShader(context->getApi()) {
		assert(code != "");
		assert(context);

		if (context->getApi() != API::OPENGL)
			throw std::invalid_argument("error: openglVertexShader::openglVertexShader: expected RenderContext to implement API::OPENGL");

		_opengl_id = context->glCreateShader(GL_VERTEX_SHADER);

		const char* source = code.c_str();
		context->glShaderSource(_opengl_id, 1, &source, nullptr);
		context->glCompileShader(_opengl_id);

		GLint status;
		context->glGetShaderiv(_opengl_id, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			int log_size;
			context->glGetShaderiv(_opengl_id, GL_INFO_LOG_LENGTH, &log_size);
			log_size++;

			char* info_log = new char[log_size];
			if (!info_log)
				throw std::invalid_argument("error: openglVertexShader::openglVertexShader: could not compile GLSL: no error message available");

			context->glGetShaderInfoLog(_opengl_id, log_size, nullptr, info_log);
			std::string info_log_str(info_log);

			delete[] info_log;
			info_log = nullptr;

			throw std::invalid_argument("error: openglVertexShader::openglVertexShader: could not compile GLSL: " + info_log_str);
		}
	}

	openglVertexShader::~openglVertexShader() {
		glDeleteShader(_opengl_id);
		_opengl_id = 0;
	}

	unsigned openglVertexShader::getOpenGLId() const {
		return _opengl_id;
	}

}