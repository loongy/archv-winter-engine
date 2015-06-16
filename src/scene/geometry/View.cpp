/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "View.h"

namespace wntr {

	std::shared_ptr<View> View::Create(const glm::vec3& position, const glm::quat& rotation) {
		return Create(position, glm::vec3(1.0f, 1.0f, 1.0f), rotation);
	}

	std::shared_ptr<View> View::Create(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation) {
		return std::make_shared<View>(position, scale, rotation);
	}

	View::View(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation) 
	: Transform3D(position, scale, rotation), _forward(0.0f, 0.0f, 1.0f), _right(1.0f, 0.0f, 0.0f), _up(0.0f, 1.0f, 0.0f) {
		_view_mat[0][0] = _right.x;
		_view_mat[1][0] = _right.y;
		_view_mat[2][0] = _right.z;
		_view_mat[3][0] = -glm::dot(_right, _position);
        
		_view_mat[0][1] = _up.x;
		_view_mat[1][1] = _up.y;
		_view_mat[2][1] = _up.z;
		_view_mat[3][1] = -glm::dot(_up, _position);
        
		_view_mat[0][2] = _forward.x;
		_view_mat[1][2] = _forward.y;
		_view_mat[2][2] = _forward.z;
		_view_mat[3][2] = -glm::dot(_forward, _position);

		_view_mat[0][3] = 0.0f;
		_view_mat[1][3] = 0.0f;
		_view_mat[2][3] = 0.0f;
		_view_mat[3][3] = 1.0f;
 
		_view_did_change = false;
	}
    
    const component_hierarchy& View::hierarchy() const {
        return _Hierarchy;
    }

	void View::translate(const glm::vec3& translation) {
		Transform3D::translate(translation);
		_view_did_change = true;
	}

	void View::scale(const glm::vec3& scale) {
		Transform3D::scale(scale);
		_view_did_change = true;
	}

	void View::rotate(const glm::quat& rotation) {
		Transform3D::rotate(rotation);
		_forward = glm::normalize(glm::rotate(glm::conjugate(_rotation), glm::vec3(0.0f, 0.0f, 1.0f)));
		_right = glm::normalize(glm::rotate(glm::conjugate(_rotation), glm::vec3(1.0f, 0.0f, 0.0f)));
		_up = glm::normalize(glm::rotate(glm::conjugate(_rotation), glm::vec3(0.0f, 1.0f, 0.0f)));
		_view_did_change = true;
	}
    
    void View::rotateInParentSpace (const glm::quat& rotation) {
        Transform3D::rotateInParentSpace(rotation);
		_view_did_change = true;
    }

	void View::setPosition(const glm::vec3& position) {
		Transform3D::setPosition(position);
		_view_did_change = true;
	}

	void View::setScale(const glm::vec3& scale) {
		Transform3D::setScale(scale);
		_view_did_change = true;
	}

	void View::setRotation(const glm::quat& rotation) {
		Transform3D::setRotation(rotation);
		_forward = glm::normalize(glm::rotate(glm::conjugate(_rotation), glm::vec3(0.0f, 0.0f, 1.0f)));
		_right = glm::normalize(glm::rotate(glm::conjugate(_rotation), glm::vec3(1.0f, 0.0f, 0.0f)));
		_up = glm::normalize(glm::rotate(glm::conjugate(_rotation), glm::vec3(0.0f, 1.0f, 0.0f)));
		_view_did_change = true;
	}

	void View::setForward(const glm::vec3& forward) {
		_forward = glm::normalize(forward);
	}

	void View::setRight(const glm::vec3& right) {
		_right = glm::normalize(right);
	}

	void View::setUp(const glm::vec3& up) {
		_up = glm::normalize(up);
	}

	const glm::mat4& View::view() const {
		if (_view_did_change) {            
			_view_mat[0][0] = _right.x;
			_view_mat[1][0] = _right.y;
			_view_mat[2][0] = _right.z;
			_view_mat[3][0] = -glm::dot(_right, _position);
            
            _view_mat[0][1] = _up.x;
			_view_mat[1][1] = _up.y;
			_view_mat[2][1] = _up.z;
			_view_mat[3][1] = -glm::dot(_up, _position);
            
			_view_mat[0][2] = _forward.x;
			_view_mat[1][2] = _forward.y;
			_view_mat[2][2] = _forward.z;
			_view_mat[3][2] = -glm::dot(_forward, _position);
            
            _view_did_change = false;
		}
		return _view_mat;
	}

	const glm::vec3& View::getForward() const {
		return _forward;
	}

	const glm::vec3& View::getRight() const {
		return _right;
	}

	const glm::vec3& View::getUp() const {
		return _up;
	}

	glm::vec3 View::getBack() const {
		return -_forward;
	}

	glm::vec3 View::getLeft() const {
		return -_right;
	}

	glm::vec3 View::getDown() const {
		return -_up;
	}
    
    const component_hierarchy View::_Hierarchy({typeid(Component),typeid(Transform),typeid(Transform3D),typeid(View)});
    
}