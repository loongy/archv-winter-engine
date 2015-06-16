/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_FRAGMENTSHADER_H_
#define _WNTR_FRAGMENTSHADER_H_

#include "CommonGraphics.h"

#include <memory>
#include <string>

namespace wntr {

	class RenderContext;

	class FragmentShader {
	public:
		static std::shared_ptr<FragmentShader> Create(const std::string& code, const RenderContext* context);

#ifndef _MSC_VER
		FragmentShader(FragmentShader&&) = default;
		FragmentShader& operator= (FragmentShader&&) = default;
#endif
		FragmentShader(const FragmentShader&) = delete;
		FragmentShader& operator= (const FragmentShader&) = delete;

		explicit FragmentShader(API api);
		virtual ~FragmentShader() = default;

		API getApi() const;

	protected:
		const API _api;

	private:

	};

}

#endif