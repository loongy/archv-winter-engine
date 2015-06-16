/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_BITMAP_H_
#define _WNTR_BITMAP_H_

#include <common/Component.h>
#include <common/utility.h>

namespace wntr {
    
    class AttributeArray;
	class RenderContext;
    class Texture;
    
    class Bitmap : public Component {
    public:
        static std::shared_ptr<Bitmap> Create(float width, float height, const RenderContext* context);
		static std::shared_ptr<Bitmap> Create(float width, float height, const std::shared_ptr<Texture>& texture, const RenderContext* context);
        
#ifndef _MSC_VER
		Bitmap(Bitmap&&) = default;
		Bitmap& operator=(Bitmap&&) = default;
#endif
		Bitmap(const Bitmap&) = delete;
		Bitmap& operator=(const Bitmap&) = delete;
        
		explicit Bitmap(float width, float height, const std::shared_ptr<Texture>& texture, const RenderContext* context);
		virtual ~Bitmap() = default;
        
        virtual const component_hierarchy& hierarchy() const override;
        
		void setWidth(float width);
		void setHeight(float height);
        void setTexture(const std::shared_ptr<Texture>& texture);
        
        float getWidth() const;
        float getHeight() const;
        const std::shared_ptr<Texture>& getTexture() const;
        
    protected:
        
    private:
        static const component_hierarchy _Hierarchy;
        
        float _width;
        float _height;
        
        std::shared_ptr<Texture> _texture;
        
    };
    
}

#endif
