/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Transform.h"

namespace wntr {
    
    const component_hierarchy& Transform::hierarchy() const {
        return _Hierarchy;
    }
    
    const component_hierarchy Transform::_Hierarchy({typeid(Component),typeid(Transform)});
    
}