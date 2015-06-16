/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Mesh.h"
#include "MeshCollection.h"

#include <cassert>

namespace wntr {
    
    std::shared_ptr<MeshCollection> MeshCollection::Create() {
        return std::make_shared<MeshCollection>();
    }
    
    const component_hierarchy& MeshCollection::hierarchy() const {
        return _Hierarchy;
    }
    
    void MeshCollection::addMesh(const std::shared_ptr<Mesh>& mesh) {
        assert(mesh);
        _meshes.add(mesh);
    }
    
    void MeshCollection::removeMesh(const std::shared_ptr<Mesh>& mesh) {
        assert(mesh);
        _meshes.remove(mesh);
    }
    
    const shared_vector<Mesh>& MeshCollection::getMeshes() const {
        return _meshes;
    }
    
    const component_hierarchy MeshCollection::_Hierarchy({typeid(Component),typeid(MeshCollection)});
    
}
