/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifndef _WNTR_TRANSFORM_H_
#define _WNTR_TRANSFORM_H_

#include "Component.h"

namespace wntr {
    
    class Transform : public Component {
    public:
#ifndef _MSC_VER
		Transform(Transform&&) = default;
		Transform& operator=(Transform&&) = default;
#endif
		Transform(const Transform&) = delete;
		Transform& operator=(const Transform&) = delete;
        
        explicit Transform() = default;
        virtual ~Transform() = default;
        
        virtual const component_hierarchy& hierarchy() const override;
        virtual const glm::mat4& matrix() const = 0;
        
    protected:
        
    private:
        static const component_hierarchy _Hierarchy;
        
    };
    
}
#endif
