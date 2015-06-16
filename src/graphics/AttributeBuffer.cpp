/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "AttributeBuffer.h"
#include "impl-opengl/openglAttributeBuffer.h"
#include "impl-opengl/openglRenderContext.h"

namespace wntr {
    
    std::shared_ptr<AttributeBuffer> AttributeBuffer::Create(const void* data, unsigned count, attribute_id id, FORMAT format, USAGE usage, const RenderContext* context) {
        switch (context->getApi()) {
            case API::OPENGL:
                return std::make_shared<openglAttributeBuffer>(data, count, id, format, usage, static_cast<const openglRenderContext*>(context));
            default:
                return nullptr;
        }
    }

	AttributeBuffer::AttributeBuffer(unsigned count, attribute_id id, FORMAT format, USAGE usage, API api)
	: _count(count), _stride(sizeOfFormat(format)), _id(id), _format(format), _usage(usage), _api(api) {

	}

	unsigned AttributeBuffer::getCount() const {
		return _count;
	}

	unsigned AttributeBuffer::getStride() const {
		return _stride;
	}

	attribute_id AttributeBuffer::getId() const {
		return _id;
	}

	FORMAT AttributeBuffer::getFormat() const {
		return _format;
	}

	USAGE AttributeBuffer::getUsage() const {
		return _usage;
	}

	API AttributeBuffer::getApi() const {
		return _api;
	}

}