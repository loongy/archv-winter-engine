/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_TRANSFORM3D_H_
#define _WNTR_TRANSFORM3D_H_

#include "Transform.h"

namespace wntr {
    
    class Transform3D : public Transform {
    public:
        static std::shared_ptr<Transform3D> Create(const glm::vec3& position=glm::vec3(0.0f,0.0f,0.0f), const glm::quat& rotation=glm::quat(1.0f,0.0f,0.0f,0.0f));
        static std::shared_ptr<Transform3D> Create(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation=glm::quat(1.0f,0.0f,0.0f,0.0f));
        
#ifndef _MSC_VER
		Transform3D(Transform3D&&) = default;
		Transform3D& operator=(Transform3D&&) = default;
#endif
		Transform3D(const Transform3D&) = delete;
		Transform3D& operator=(const Transform3D&) = delete;
        
        explicit Transform3D(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);
		virtual ~Transform3D() = default;
        
        virtual const component_hierarchy& hierarchy() const override;
        virtual const glm::mat4& matrix() const override;
        
		virtual void translate(const glm::vec3& translation);
		virtual void scale(const glm::vec3& scale);
		virtual void rotate(const glm::quat& rotation);
        virtual void rotateInParentSpace (const glm::quat& rotation);
        
		virtual void setMatrix(const glm::mat4& matrix);
		virtual void setPosition(const glm::vec3& position);
		virtual void setScale(const glm::vec3& scale);
		virtual void setRotation(const glm::quat& rotation);
        
		virtual const glm::vec3& getPosition() const;
		virtual const glm::vec3& getScale() const;
		virtual const glm::quat& getRotation() const;
        
    protected:
        mutable bool _transform_did_change;
        mutable glm::mat4 _transform_mat;
        
        glm::vec3 _position;
        glm::vec3 _scale;
        glm::quat _rotation;
        
    private:
        static const component_hierarchy _Hierarchy;
        
    };
    
}

#endif