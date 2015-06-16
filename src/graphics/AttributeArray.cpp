/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "AttributeArray.h"
#include "AttributeBuffer.h"
#include "impl-opengl/openglAttributeArray.h"
#include "impl-opengl/openglRenderContext.h"

#include <cassert>

namespace wntr {
    
    std::shared_ptr<AttributeArray> AttributeArray::Create(POLYGON_TYPE poly_type, const RenderContext* context) {
        switch (context->getApi()) {
            case API::OPENGL:
                return std::make_shared<openglAttributeArray>(poly_type, static_cast<const openglRenderContext*>(context));
            default:
                return nullptr;
        }
    }

	AttributeArray::AttributeArray(POLYGON_TYPE poly_type, API api) 
	: _alignment_required(true), _aligned_program(nullptr), _poly_type(poly_type), _api(api) {

	}

	void AttributeArray::setAttribute(const std::shared_ptr<AttributeBuffer>& attr) {
		assert(attr);

		if (attr->getApi() != _api)
			throw std::invalid_argument("error: AttributeArray::setAttribute: AttributeArray implements a different API");

		auto id = attr->getId();
		if (id == ATTRIBUTE_ELEMENTS) {
			_element_buffer = attr;
			return;
		}

		_attribute_buffers.remove_if([id](const std::shared_ptr<AttributeBuffer>& elem)->bool {
			return elem->getId() == id;
		});

		_attribute_buffers.add(attr);
		_alignment_required = true;
	}

	void AttributeArray::setPolygon(POLYGON_TYPE poly_type) {
		_poly_type = poly_type;
	}

	unsigned AttributeArray::getAttributeCount() const {
		return _attribute_buffers.size();
	}

	std::shared_ptr<AttributeBuffer> AttributeArray::getAttribute(attribute_id id) const {
		if (id == ATTRIBUTE_ELEMENTS)
			return _element_buffer;

		for (auto& attr : _attribute_buffers)
			if (attr->getId() == id)
				return attr;

		return nullptr;
	}

	const std::shared_ptr<AttributeBuffer>& AttributeArray::getAttributeByIndex(unsigned i) const {
		assert(i < _attribute_buffers.size());
		return _attribute_buffers.at(i);
	}

	POLYGON_TYPE AttributeArray::getPolygonType() const {
		return _poly_type;
	}

	const API AttributeArray::getApi() const {
		return _api;
	}

}
