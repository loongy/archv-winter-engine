/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_UNIFORM_H_
#define _WNTR_UNIFORM_H_

#include "CommonGraphics.h"

#include <memory>

namespace wntr {

	class RenderContext;

	class Uniform {
	public:
		static std::shared_ptr<Uniform> Create(const void* data, unsigned size, USAGE usage, const RenderContext* context);

#ifndef _MSC_VER
		Uniform(Uniform&&) = default;
		Uniform& operator= (Uniform&&) = default;
#endif
		Uniform(const Uniform&) = delete;
		Uniform& operator= (const Uniform&) = delete;

		explicit Uniform(USAGE usage, API api);
		virtual ~Uniform() = default;

		virtual void setData(const void* data, unsigned size, const RenderContext* context) = 0;

		unsigned getSize() const;
		USAGE getUsage() const;
		API getApi() const;

	protected:
		unsigned _size;
		const USAGE _usage;
		const API _api;

	private:

	};

}

#endif