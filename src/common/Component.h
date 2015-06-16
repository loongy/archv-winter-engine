/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_COMPONENT_H_
#define _WNTR_COMPONENT_H_

#include "utility.h"

namespace wntr {
    
    class Component {
    public:
#ifndef _MSC_VER
		Component(Component&&) = default;
		Component& operator=(Component&&) = default;
#endif
		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
        
		explicit Component() = default;
		virtual ~Component() = default;
        
        virtual const component_hierarchy& hierarchy() const;
        
    protected:
        
    private:
        static const component_hierarchy _Hierarchy;
        
    };

}

#endif