/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_ATTRIBUTEARRAY_H_
#define _WNTR_ATTRIBUTEARRAY_H_

#include "CommonGraphics.h"

#include <common/utility.h>

#include <memory>

namespace wntr {

	class AttributeBuffer;
	class Program;
	class RenderContext;

	class AttributeArray {
	public:
		static std::shared_ptr<AttributeArray> Create(POLYGON_TYPE poly_type, const RenderContext* context);

#ifndef _MSC_VER
		AttributeArray(AttributeArray&&) = default;
		AttributeArray& operator=(AttributeArray&&) = default;
#endif
		AttributeArray(const AttributeArray&) = delete;
		AttributeArray& operator=(const AttributeArray&) = delete;

		explicit AttributeArray(POLYGON_TYPE poly_type, API api);
		virtual ~AttributeArray() = default;

		virtual void align(const Program* program, const RenderContext* context) = 0;

		void setAttribute(const std::shared_ptr<AttributeBuffer>& attr);
		void setPolygon(POLYGON_TYPE poly_type);

		unsigned                                getAttributeCount() const;
		std::shared_ptr<AttributeBuffer>        getAttribute(attribute_id id) const;
		const std::shared_ptr<AttributeBuffer>& getAttributeByIndex(unsigned i) const;
		
		POLYGON_TYPE getPolygonType() const;
		const API    getApi() const;

	protected:
		bool _alignment_required;
		const Program* _aligned_program;

		std::shared_ptr<AttributeBuffer> _element_buffer;
		shared_vector<AttributeBuffer> _attribute_buffers;

		POLYGON_TYPE _poly_type;
		const API _api;

	private:

	};

}

#endif