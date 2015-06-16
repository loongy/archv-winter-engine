/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_GEOMETRYSHADER_H_
#define _WNTR_GEOMETRYSHADER_H_

#include "CommonGraphics.h"

#include <memory>
#include <string>

namespace wntr {
    
	class RenderContext;
    
	class GeometryShader {
	public:
		static std::shared_ptr<GeometryShader> Create(const std::string& code, const RenderContext* context);
        
#ifndef _MSC_VER
		GeometryShader(GeometryShader&&) = default;
		GeometryShader& operator= (GeometryShader&&) = default;
#endif
		GeometryShader(const GeometryShader&) = delete;
		GeometryShader& operator= (const GeometryShader&) = delete;
        
		explicit GeometryShader(API api);
		virtual ~GeometryShader() = default;
        
		API getApi() const;
        
	protected:
		const API _api;
        
	private:
        
	};
    
}

#endif