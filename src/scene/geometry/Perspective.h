/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_PERSPECTIVE_H_
#define _WNTR_PERSPECTIVE_H_

#include "Projection.h"

namespace wntr {
    
    class Perspective : public Projection {
    public:
		static std::shared_ptr<Perspective> Create(float aspect, float fov = 1.047197551f, float near_clip = 0.1f, float far_clip = 1000.0f);

#ifndef _MSC_VER
		Perspective(Perspective&&) = default;
		Perspective& operator=(Perspective&&) = default;
#endif
		Perspective(const Perspective&) = delete;
		const Perspective& operator=(const Perspective&) = delete;

		explicit Perspective(float near_clip, float far_clip, float fov, float aspect);
		virtual ~Perspective() = default;

		virtual const component_hierarchy& hierarchy() const override;
		virtual const glm::mat4& projection() const override;

		void setNearClip(float near_clip);
		void setFarClip(float far_clip);
		void setFieldOfView(float fov);
		void setAspectRatio(float aspect);

		float getNearClip() const;
		float getFarClip() const;
		float getFieldOfView() const;
		float getAspectRatio() const;
        
    protected:
        
    private:
        static const component_hierarchy _Hierarchy;

		float _near_clip;
		float _far_clip;
		float _fov;
		float _aspect;

		mutable bool _proj_did_changed;
		mutable glm::mat4 _proj_mat;

    };
    
}

#endif
