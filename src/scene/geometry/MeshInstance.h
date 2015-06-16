//
//  InstanceMesh.h
//  winter-project
//
//  Created by Benjamin Wang on 6/06/2014.
//  Copyright (c) 2014 loongy. All rights reserved.
//

#ifndef _WNTR_MESHINSTANCE_H_
#define _WNTR_MESHINSTANCE_H_

#include <common/Component.h>

namespace wntr {
    
    class Mesh;
    
    class MeshInstance : public Component {
    public:
        static std::shared_ptr<MeshInstance> Create(const std::shared_ptr<Mesh>& mesh);
        
#ifndef _MSC_VER
		MeshInstance(MeshInstance&&) = default;
		MeshInstance& operator=(MeshInstance&&) = default;
#endif
		MeshInstance(const MeshInstance&) = delete;
		MeshInstance& operator=(const MeshInstance&) = delete;
        
		explicit MeshInstance(const std::shared_ptr<Mesh>& mesh);
		virtual ~MeshInstance() = default;
        
        virtual const component_hierarchy& hierarchy() const override;
        
        const std::shared_ptr<Mesh>& getMesh() const;
        
    protected:
        
    private:
        static const component_hierarchy _Hierarchy;
        
        std::shared_ptr<Mesh> _mesh;
        
    };
    
}

#endif
