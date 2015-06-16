/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_VERTEXSHADER_H_
#define _WNTR_VERTEXSHADER_H_

#include "CommonGraphics.h"

#include <memory>
#include <string>

namespace wntr {

	class RenderContext;

	class VertexShader {
	public:
		static std::shared_ptr<VertexShader> Create(const std::string& code, const RenderContext* context);

#ifndef _MSC_VER
		VertexShader(VertexShader&&) = default;
		VertexShader& operator= (VertexShader&&) = default;
#endif
		VertexShader(const VertexShader&) = delete;
		VertexShader& operator= (const VertexShader&) = delete;

		explicit VertexShader(API api);
		virtual ~VertexShader() = default;

		API getApi() const;

	protected:
		const API _api;

	private:
	};

}

#endif