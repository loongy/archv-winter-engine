/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Transform3D.h"

namespace wntr {
    
    std::shared_ptr<Transform3D> Transform3D::Create(const glm::vec3& position, const glm::quat& rotation) {
        return Create(position, glm::vec3(1.0f,1.0f,1.0f), rotation);
    }
    
    std::shared_ptr<Transform3D> Transform3D::Create(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation) {
        return std::make_shared<Transform3D>(position, scale, rotation);
    }
    
    Transform3D::Transform3D(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation)
    : Transform(), _transform_did_change(false), _position(position), _scale(scale), _rotation(rotation) {
		_transform_mat = glm::translate(_position);
        _transform_mat = glm::rotate(_transform_mat, glm::angle(_rotation), glm::axis(_rotation));
		_transform_mat = glm::scale(_transform_mat, _scale);
    }
    
    const component_hierarchy& Transform3D::hierarchy() const {
        return _Hierarchy;
    }
    
    const glm::mat4& Transform3D::matrix() const {
        if (_transform_did_change) {
            _transform_mat = glm::translate(_position) * glm::toMat4(_rotation) * glm::scale(_scale);
        }
        return _transform_mat;
    }
    
    void Transform3D::translate(const glm::vec3& translation) {
        _transform_did_change = true;
        _position += translation;
    }
    
    void Transform3D::scale(const glm::vec3& scale) {
        _transform_did_change = true;
        _scale *= scale;
    }
    
    void Transform3D::rotate(const glm::quat& rotation) {
        _transform_did_change = true;
        _rotation = glm::normalize(rotation) * _rotation;
    }
    
    void Transform3D::rotateInParentSpace (const glm::quat& rotation) {
        auto rot = glm::normalize(_rotation * glm::normalize(rotation) * glm::conjugate(_rotation));
        rotate(rot);
    }

	void Transform3D::setMatrix(const glm::mat4& matrix) {
		double a = matrix[0][0]; double e = matrix[1][0]; double i = matrix[2][0]; double m = matrix[3][0];
		double b = matrix[0][1]; double f = matrix[1][1]; double j = matrix[2][1]; double n = matrix[3][1];
		double c = matrix[0][2]; double g = matrix[1][2]; double k = matrix[2][2]; double o = matrix[3][2];

		_position.x = m;
		_position.y = n;
		_position.z = o;

		_scale.x = sqrt(a*a + b*b + c*c);
		_scale.y = sqrt(e*e + f*f + g*g);
		_scale.z = sqrt(i*i + j*j + k*k);

		double half_euler_x = atan2(j / _scale.z, k / _scale.z)*0.5f;
		double half_euler_y = -asin(i / _scale.z)*0.5f;
		double half_euler_z = atan2(e / _scale.y, a / _scale.x)*0.5f;

		_rotation.w = cos(half_euler_x)*cos(half_euler_y)*cos(half_euler_z) + sin(half_euler_x)*sin(half_euler_y)*sin(half_euler_z);
		_rotation.x = sin(half_euler_x)*cos(half_euler_y)*cos(half_euler_z) - cos(half_euler_x)*sin(half_euler_y)*sin(half_euler_z);
		_rotation.y = cos(half_euler_x)*sin(half_euler_y)*cos(half_euler_z) + sin(half_euler_x)*cos(half_euler_y)*sin(half_euler_z);
		_rotation.z = cos(half_euler_x)*cos(half_euler_y)*sin(half_euler_z) - sin(half_euler_x)*sin(half_euler_y)*cos(half_euler_z);
        glm::normalize(_rotation);

		_transform_mat = matrix;
		_transform_did_change = false;
	}
    
    void Transform3D::setPosition(const glm::vec3& position) {
        _transform_did_change = true;
        _position = position;
    }

    void Transform3D::setScale(const glm::vec3& scale) {
        _transform_did_change = true;
        _scale = scale;
    }
    
    void Transform3D::setRotation(const glm::quat& rotation) {
        _transform_did_change = true;
        _rotation = rotation;
    }
    
    const glm::vec3& Transform3D::getPosition() const {
        return _position;
    }
    
    const glm::vec3& Transform3D::getScale() const {
        return _scale;
    }
    
    const glm::quat& Transform3D::getRotation() const {
        return _rotation;
    }
    
    const component_hierarchy Transform3D::_Hierarchy({typeid(Component),typeid(Transform),typeid(Transform3D)});
    
}