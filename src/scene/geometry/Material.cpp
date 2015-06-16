/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Material.h"

namespace wntr {
    
    std::shared_ptr<Material> Material::Create() {
        return Create(nullptr, nullptr, nullptr);
    }
    
    std::shared_ptr<Material> Material::Create(const std::shared_ptr<Texture>& diffuse, const std::shared_ptr<Texture>& specular, const std::shared_ptr<Texture>& emissive) {
        auto material = std::make_shared<Material>();
        material->setTextureDiffuse(diffuse);
        material->setTextureSpecular(specular);
        material->setTextureEmissive(emissive);
        return material;
    }
    
    const component_hierarchy& Material::hierarchy() const {
        return _Hierarchy;
    }
    
    void Material::setTextureDiffuse(const std::shared_ptr<Texture>& texture) {
        _diffuse = texture;
    }
    
    void Material::setTextureSpecular(const std::shared_ptr<Texture>& texture) {
        _specular = texture;
    }
    
    void Material::setTextureEmissive(const std::shared_ptr<Texture>& texture) {
        _emissive = texture;
    }
    
    const std::shared_ptr<Texture>& Material::getTextureDiffuse() const {
        return _diffuse;
    }
    
    const std::shared_ptr<Texture>& Material::getTextureSpecular() const {
        return _specular;
    }
    
    const std::shared_ptr<Texture>& Material::getTextureEmissive() const {
        return _emissive;
    }
    
    const component_hierarchy Material::_Hierarchy({typeid(Component),typeid(Material)});
    
}