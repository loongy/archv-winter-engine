/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Transform2D.h"

namespace wntr {
    
    std::shared_ptr<Transform2D> Transform2D::Create(const glm::vec2& position, float rotation) {
        return Create(position, glm::vec2(1.0f,1.0f), rotation);
    }
    
    std::shared_ptr<Transform2D> Transform2D::Create(const glm::vec2& position, const glm::vec2& scale, float rotation) {
        return std::make_shared<Transform2D>(position, scale, rotation);
    }
    
    Transform2D::Transform2D(const glm::vec2& position, const glm::vec2& scale, float rotation)
    : Transform(), _transform_did_change(false), _position(position), _scale(scale), _rotation(rotation) {
        _transform_mat = glm::translate(glm::vec3(_position.x, _position.y, 0.0f));
		_transform_mat = glm::rotate(_transform_mat, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        _transform_mat = glm::scale(_transform_mat, glm::vec3(_scale.x, _scale.y, 1.0f));
    }
    
    const component_hierarchy& Transform2D::hierarchy() const {
        return _Hierarchy;
    }
    
    const glm::mat4x4& Transform2D::matrix() const {
        if (_transform_did_change) {
			_transform_mat = glm::translate(glm::vec3(_position.x, _position.y, 0.0f));
            _transform_mat = glm::rotate(_transform_mat, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));
            _transform_mat = glm::scale(_transform_mat, glm::vec3(_scale.x, _scale.y, 1.0f));
        }
		return _transform_mat;
    }
    
    void Transform2D::translate(const glm::vec2& translation) {
        _transform_did_change = true;
        _position += translation;
    }
    
    void Transform2D::scale(const glm::vec2& scale) {
        _transform_did_change = true;
        _scale *= scale;
    }
    
    void Transform2D::rotate(float rotation) {
        _transform_did_change = true;
        _rotation += rotation;
    }
    
    void Transform2D::setPosition(const glm::vec2& position) {
        _transform_did_change = true;
        _position = position;
    }
    
    void Transform2D::setScale(const glm::vec2& scale) {
        _transform_did_change = true;
        _scale = scale;
    }
    
    void Transform2D::setRotation(float rotation) {
        _transform_did_change = true;
        _rotation = rotation;
    }
    
    const glm::vec2& Transform2D::getPosition() const {
        return _position;
    }
    
    const glm::vec2& Transform2D::getScale() const {
        return _scale;
    }
    
    float Transform2D::getRotation() const {
        return _rotation;
    }
    
    const component_hierarchy Transform2D::_Hierarchy({typeid(Component),typeid(Transform),typeid(Transform2D)});
    
}