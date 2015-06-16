//
//  InstanceMesh.cpp
//  winter-project
//
//  Created by Benjamin Wang on 6/06/2014.
//  Copyright (c) 2014 loongy. All rights reserved.
//

#include "MeshInstance.h"

namespace wntr {
    
    std::shared_ptr<MeshInstance> MeshInstance::Create(const std::shared_ptr<Mesh>& mesh) {
        return std::make_shared<MeshInstance>(mesh);
    }
    
    MeshInstance::MeshInstance(const std::shared_ptr<Mesh>& mesh)
    : Component(), _mesh(mesh) {
    }
    
    const component_hierarchy& MeshInstance::hierarchy() const {
        return _Hierarchy;
    }
    
    const std::shared_ptr<Mesh>& MeshInstance::getMesh() const {
        return _mesh;
    }
    
    const component_hierarchy MeshInstance::_Hierarchy({typeid(Component),typeid(MeshInstance)});
    
}