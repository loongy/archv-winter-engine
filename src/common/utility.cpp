/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include <utility>

#include "utility.h"

namespace wntr {
    
    std::ostream& operator<<(std::ostream& os, const component_hierarchy& hierarchy) {
        for (const auto& t : hierarchy._types)
            os << t.name() << " ";
        return os;
    }
    
    component_hierarchy::component_hierarchy(const std::vector<std::type_index>& types)
    : _types(types) {
    }
    
    const std::vector<std::type_index>& component_hierarchy::types() const {
        return _types;
    }

}