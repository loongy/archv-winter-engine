/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_ATTRIBUTEBUFFER_H_
#define _WNTR_ATTRIBUTEBUFFER_H_

#include "CommonGraphics.h"

#include <memory>

namespace wntr {

	class RenderContext;

	class AttributeBuffer {
	public:
		static std::shared_ptr<AttributeBuffer> Create(const void* data, unsigned count, attribute_id id, FORMAT format, USAGE usage, const RenderContext* context);

#ifndef _MSC_VER
		AttributeBuffer(AttributeBuffer&&) = default;
		AttributeBuffer& operator= (AttributeBuffer&&) = default;
#endif
		AttributeBuffer(const AttributeBuffer&) = delete;
		AttributeBuffer& operator= (const AttributeBuffer&) = delete;

		explicit AttributeBuffer(unsigned count, attribute_id id, FORMAT format, USAGE usage, API api);
		virtual ~AttributeBuffer() = default;

		virtual void setData(const void* data, unsigned count, FORMAT format, const RenderContext* context) = 0;

		unsigned     getCount() const;
		unsigned     getStride() const;
		attribute_id getId() const;
		FORMAT       getFormat() const;

		USAGE getUsage() const;
		API   getApi() const;

	protected:
		unsigned _count;
		unsigned _stride;
		attribute_id _id;
		FORMAT _format;
		const USAGE _usage;
		const API _api;

	private:
	};

}

#endif