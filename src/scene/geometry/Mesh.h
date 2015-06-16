/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_MESH_H_
#define _WNTR_MESH_H_

#include <common/Component.h>

namespace wntr {
    
    class AttributeArray;
    class Material;
	class RenderContext;
    
    class Mesh : public Component {
    public:
		static std::shared_ptr<Mesh> Create(const RenderContext* context);
		static std::shared_ptr<Mesh> CreateCube(float size, const RenderContext* context);
		static std::shared_ptr<Mesh> CreatePrism(float size_x, float size_y, float size_z, const RenderContext* context);
        static std::shared_ptr<Mesh> CreateUVSphere(float radius, unsigned div_lon, unsigned div_lat, const RenderContext* context);

#ifndef _MSC_VER
		Mesh(Mesh&&) = default;
		Mesh& operator=(Mesh&&) = default;
#endif
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
        
		explicit Mesh(const RenderContext* context);
		virtual ~Mesh() = default;
        
        virtual const component_hierarchy& hierarchy() const override;
        
        void setMaterial(const std::shared_ptr<Material>& material);
        
        const std::shared_ptr<AttributeArray>& getAttributeArray() const;
        const std::shared_ptr<Material>& getMaterial() const;
        
    protected:
        
    private:
        static const component_hierarchy _Hierarchy;
        
        std::shared_ptr<AttributeArray> _attribute_array;
        std::shared_ptr<Material> _material;
        
    };
    
}

#endif
