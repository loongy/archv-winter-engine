/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_TRANSFORM2D_H_
#define _WNTR_TRANSFORM2D_H_

#include "Transform.h"

namespace wntr {
    
    class Transform2D : public Transform {
    public:
        static std::shared_ptr<Transform2D> Create(const glm::vec2& position=glm::vec2(1.0f,1.0f), float rotation=0.0f);
        static std::shared_ptr<Transform2D> Create(const glm::vec2& position, const glm::vec2& scale, float rotation=0.0f);
        
#ifndef _MSC_VER
		Transform2D(Transform2D&&) = default;
		Transform2D& operator=(Transform2D&&) = default;
#endif
		Transform2D(const Transform2D&) = delete;
		Transform2D& operator=(const Transform2D&) = delete;
        
        explicit Transform2D(const glm::vec2& position, const glm::vec2& scale, float rotation);
		virtual ~Transform2D() = default;
        
        virtual const component_hierarchy& hierarchy() const override;
        virtual const glm::mat4& matrix() const override;
        
		virtual void translate(const glm::vec2& translation);
		virtual void scale(const glm::vec2& scale);
		virtual void rotate(float rotation);
        
		virtual void setPosition(const glm::vec2& position);
		virtual void setScale(const glm::vec2& scale);
		virtual void setRotation(float rotation);
        
		virtual const glm::vec2& getPosition() const;
		virtual const glm::vec2& getScale() const;
		virtual float getRotation() const;
        
    protected:
        mutable bool _transform_did_change;
        mutable glm::mat4 _transform_mat;
        
        glm::vec2 _position;
        glm::vec2 _scale;
        float _rotation;
        
    private:
        static const component_hierarchy _Hierarchy;
        
    };
    
}

#endif