/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_MESHCOLLECTION_H_
#define _WNTR_MESHCOLLECTION_H_

#include <common/Component.h>

namespace wntr {
    
    class Mesh;
    
    class MeshCollection : public Component {
    public:
        static std::shared_ptr<MeshCollection> Create();
        
#ifndef _MSC_VER
		MeshCollection(MeshCollection&&) = default;
		MeshCollection& operator=(MeshCollection&&) = default;
#endif
		MeshCollection(const MeshCollection&) = delete;
		MeshCollection& operator=(const MeshCollection&) = delete;
        
		explicit MeshCollection() = default;
		virtual ~MeshCollection() = default;
        
        virtual const component_hierarchy& hierarchy() const override;
        
        void addMesh(const std::shared_ptr<Mesh>& mesh);
        void removeMesh(const std::shared_ptr<Mesh>& mesh);
        
        const shared_vector<Mesh>& getMeshes() const;
        
    protected:
        shared_vector<Mesh> _meshes;
        
    private:
        static const component_hierarchy _Hierarchy;
        
    };
    
}

#endif
