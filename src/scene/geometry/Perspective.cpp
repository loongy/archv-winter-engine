/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Perspective.h"

namespace wntr {

	std::shared_ptr<Perspective> Perspective::Create(float aspect, float fov, float near_clip, float far_clip) {
		return std::make_shared<Perspective>(near_clip, far_clip, fov, aspect);
	}

	Perspective::Perspective(float near_clip, float far_clip, float fov, float aspect)
	: Projection(), _near_clip(near_clip), _far_clip(far_clip), _fov(fov), _aspect(aspect), _proj_did_changed(false) {
		float y_scale = 1.0f/glm::tan(fov*0.5f);
		float x_scale = y_scale/aspect;
		float e_33 = far_clip/(far_clip-near_clip);
		float e_43 = -near_clip*far_clip/(far_clip-near_clip);
        
		_proj_mat[0][0] = x_scale;
		_proj_mat[1][0] = 0.0f;
		_proj_mat[2][0] = 0.0f;
		_proj_mat[3][0] = 0.0f;

		_proj_mat[0][1] = 0.0f;
        _proj_mat[1][1] = y_scale;
		_proj_mat[2][1] = 0.0f;
		_proj_mat[3][1] = 0.0f;

		_proj_mat[0][2] = 0.0f;
		_proj_mat[1][2] = 0.0f;
        _proj_mat[2][2] = e_33;
		_proj_mat[3][2] = e_43;

		_proj_mat[0][3] = 0.0f;
		_proj_mat[1][3] = 0.0f;
		_proj_mat[2][3] = 1.0f;
		_proj_mat[3][3] = 0.0f;
        
	}
    
	const component_hierarchy& Perspective::hierarchy() const {
        return _Hierarchy;
    }
    
	const glm::mat4& Perspective::projection() const {
		if (_proj_did_changed) {
			float y_scale = 1.0f / glm::tan(_fov*0.5f);
			float x_scale = y_scale / _aspect;
			float e_33 = _far_clip / (_far_clip - _near_clip);
			float e_43 = -_near_clip*_far_clip / (_far_clip - _near_clip);

			_proj_mat[0][0] = x_scale;
			_proj_mat[1][1] = y_scale;
			_proj_mat[2][2] = e_33;
			_proj_mat[3][2] = e_43;

			_proj_did_changed = false;
		}
		return _proj_mat;
	}

	void Perspective::setNearClip(float near_clip) {
		_near_clip = near_clip;
		_proj_did_changed = true;
	}

	void Perspective::setFarClip(float far_clip) {
		_far_clip = far_clip;
		_proj_did_changed = true;
	}

	void Perspective::setFieldOfView(float fov) {
		_fov = fov;
		_proj_did_changed = true;
	}

	void Perspective::setAspectRatio(float aspect) {
		_aspect = aspect;
		_proj_did_changed = true;
	}

	float Perspective::getNearClip() const {
		return _near_clip;
	}

	float Perspective::getFarClip() const {
		return _far_clip;
	}

	float Perspective::getFieldOfView() const {
		return _fov;
	}

	float Perspective::getAspectRatio() const {
		return _aspect;
	}

	const component_hierarchy Perspective::_Hierarchy({ typeid(Component), typeid(Projection), typeid(Perspective) });
    
}