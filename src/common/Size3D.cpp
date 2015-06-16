/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include <cassert>

#include "Size3D.h"

namespace wntr {
    
    std::shared_ptr<Size3D> Size3D::Create(float width, float height, float depth) {
        return std::make_shared<Size3D>(width, height, depth);
    }
    
    Size3D::Size3D(float width, float height, float depth)
    : Component(), _width(width), _height(height), _depth(depth) {
        assert(width > 0.0f);
        assert(height > 0.0f);
        assert(depth > 0.0f);
    }
    
    const component_hierarchy& Size3D::hierarchy() const {
        return _Hierarchy;
    }
    
    void Size3D::setWidth(float width) {
        assert(width > 0.0f);
        _width = width;
    }
    
    void Size3D::setHeight(float height) {
        assert(height > 0.0f);
        _height = height;
    }
    
    void Size3D::setDepth(float depth) {
        assert(depth > 0.0f);
        _depth = depth;
    }
    
    float Size3D::getWidth() const {
        return _width;
    }
    
    float Size3D::getHeight() const {
        return _height;
    }
    
    float Size3D::getDepth() const {
        return _depth;
    }
    
    const component_hierarchy Size3D::_Hierarchy({typeid(Component),typeid(Size3D)});
    
}