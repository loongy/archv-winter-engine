/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include <cassert>

#include "Size2D.h"

namespace wntr {
    
    std::shared_ptr<Size2D> Size2D::Create(float width, float height) {
        return std::make_shared<Size2D>(width, height);
    }
    
    Size2D::Size2D(float width, float height)
    : Component(), _width(width), _height(height) {
        assert(width > 0.0f);
        assert(height > 0.0f);
    }
    
    const component_hierarchy& Size2D::hierarchy() const {
        return _Hierarchy;
    }
    
    void Size2D::setWidth(float width) {
        assert(width > 0.0f);
        _width = width;
    }
    
    void Size2D::setHeight(float height) {
        assert(height > 0.0f);
        _height = height;
    }
    
    float Size2D::getWidth() const {
        return _width;
    }
    
    float Size2D::getHeight() const {
        return _height;
    }
    
    const component_hierarchy Size2D::_Hierarchy({typeid(Component),typeid(Size2D)});
    
}