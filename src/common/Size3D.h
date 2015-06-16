/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_SIZE3D_H_
#define _WNTR_SIZE3D_H_

#include "Component.h"

namespace wntr {
    
    class Size3D : public Component {
    public:
        static std::shared_ptr<Size3D> Create(float width, float height, float depth);
        
#ifndef _MSC_VER
		Size3D(Size3D&&) = default;
		Size3D& operator=(Size3D&&) = default;
#endif
		Size3D(const Size3D&) = delete;
		Size3D& operator=(const Size3D&) = delete;
        
        explicit Size3D(float width, float height, float depth);
		virtual ~Size3D() = default;
        
        virtual const component_hierarchy& hierarchy() const override;
        
        void setWidth(float width);
        void setHeight(float height);
        void setDepth(float depth);
        
        float getWidth() const;
        float getHeight() const;
        float getDepth() const;
        
    protected:
        float _width;
        float _height;
        float _depth;
        
    private:
        static const component_hierarchy _Hierarchy;
        
    };
    
}

#endif
