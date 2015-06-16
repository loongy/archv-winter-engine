/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_MATERIAL_H_
#define _WNTR_MATERIAL_H_

#include <common/Component.h>

#include <memory>

namespace wntr {
    
    class Texture;
    
    class Material : public Component {
    public:
        static std::shared_ptr<Material> Create();
        static std::shared_ptr<Material> Create(const std::shared_ptr<Texture>& diffuse, const std::shared_ptr<Texture>& specular=nullptr, const std::shared_ptr<Texture>& emissive=nullptr);
        
#ifndef _MSC_VER
		Material(Material&&) = default;
		Material& operator=(Material&&) = default;
#endif
		Material(const Material&) = delete;
		Material& operator=(const Material&) = delete;
        
		explicit Material() = default;
		virtual ~Material() = default;
        
        virtual const component_hierarchy& hierarchy() const override;
        
        void setTextureDiffuse(const std::shared_ptr<Texture>& texture);
        void setTextureSpecular(const std::shared_ptr<Texture>& texture);
        void setTextureEmissive(const std::shared_ptr<Texture>& texture);
        
        const std::shared_ptr<Texture>& getTextureDiffuse() const;
        const std::shared_ptr<Texture>& getTextureSpecular() const;
        const std::shared_ptr<Texture>& getTextureEmissive() const;
        
    protected:
        
    private:
        static const component_hierarchy _Hierarchy;
        
        std::shared_ptr<Texture> _diffuse;
        std::shared_ptr<Texture> _specular;
        std::shared_ptr<Texture> _emissive;
        
    };
    
}

#endif
