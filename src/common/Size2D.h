/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_SIZE2D_H_
#define _WNTR_SIZE2D_H_

#include "Component.h"

namespace wntr {
    
    class Size2D : public Component {
    public:
        static std::shared_ptr<Size2D> Create(float width, float height);
        
#ifndef _MSC_VER
		Size2D(Size2D&&) = default;
		Size2D& operator=(Size2D&&) = default;
#endif
		Size2D(const Size2D&) = delete;
		Size2D& operator=(const Size2D&) = delete;
        
        explicit Size2D(float width, float height);
		virtual ~Size2D() = default;
        
        virtual const component_hierarchy& hierarchy() const override;
        
        void setWidth(float width);
        void setHeight(float height);
        
        float getWidth() const;
        float getHeight() const;
        
    protected:
        float _width;
        float _height;
        
    private:
        static const component_hierarchy _Hierarchy;
        
    };
    
}

#endif
