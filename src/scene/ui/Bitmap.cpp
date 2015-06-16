/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Bitmap.h"

#include <graphics/AttributeArray.h>
#include <graphics/AttributeBuffer.h>
#include <graphics/Program.h>
#include <graphics/RenderContext.h>

#include <iostream>

namespace wntr {
    
    std::shared_ptr<Bitmap> Bitmap::Create(float width, float height, const RenderContext* context) {
        return Create(width, height, nullptr, context);
    }
    
    std::shared_ptr<Bitmap> Bitmap::Create(float width, float height, const std::shared_ptr<Texture>& texture, const RenderContext* context) {
        return std::make_shared<Bitmap>(width, height, texture, context);
    }
    
    Bitmap::Bitmap(float width, float height, const std::shared_ptr<Texture>& texture, const RenderContext* context)
    : Component(), _width(width), _height(height), _texture(texture) {
    }
    
    const component_hierarchy& Bitmap::hierarchy() const {
        return _Hierarchy;
    }

	void Bitmap::setWidth(float width) {
		_width = width;
	}

	void Bitmap::setHeight(float height) {
		_height = height;
	}
    
    void Bitmap::setTexture(const std::shared_ptr<Texture>& texture) {
        _texture = texture;
    }
    
    float Bitmap::getWidth() const {
        return _width;
    }
    
    float Bitmap::getHeight() const {
        return _height;
    }

    const std::shared_ptr<Texture>& Bitmap::getTexture() const {
        return _texture;
    }
    
    const component_hierarchy Bitmap::_Hierarchy({typeid(Component),typeid(Bitmap)});
    
}