/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_COMPONENTT_H_
#define _WNTR_COMPONENTT_H_

#include "Component.h"

namespace wntr {
    
    template<typename T>
    class ComponentT : public Component {
    public:
		template <class... Args>
        static std::shared_ptr<ComponentT<T>> Create(Args&& ...args);
        static std::shared_ptr<ComponentT<T>> Create(const std::shared_ptr<T>& value);
        
#ifndef _MSC_VER
		ComponentT(ComponentT&&) = default;
		ComponentT& operator=(ComponentT&&) = default;
#endif
		ComponentT(const ComponentT&) = delete;
		ComponentT& operator=(const ComponentT&) = delete;
        
		explicit ComponentT(const std::shared_ptr<T>& value);
		virtual ~ComponentT() = default;
        
        virtual const component_hierarchy& hierarchy() const override;
        
		void setValue(const std::shared_ptr<T>& value);
        
		const std::shared_ptr<T>& value();
		const std::shared_ptr<T>& operator->();
        
    protected:
		std::shared_ptr<T> _value;
        
    private:
        static const component_hierarchy _Hierarchy;
        
    };
    
    template<typename T>
	template <class... Args>
	std::shared_ptr<ComponentT<T>> ComponentT<T>::Create(Args&& ...args) {
        return std::make_shared<ComponentT<T>>(std::make_shared<T>(std::forward<Args>(args)...));
    }
    
    template<typename T>
	std::shared_ptr<ComponentT<T>> ComponentT<T>::Create(const std::shared_ptr<T>& value) {
        return std::make_shared<ComponentT<T>>(value);
    }
    
    template<typename T>
	ComponentT<T>::ComponentT(const std::shared_ptr<T>& value)
    : Component(), _value(value) {
    }
    
    template<typename T>
    const component_hierarchy& ComponentT<T>::hierarchy() const {
        return _Hierarchy;
    }
    
    template<typename T>
	void ComponentT<T>::setValue(const std::shared_ptr<T>& value) {
        _value = value;
    }
    
    template<typename T>
	const std::shared_ptr<T>& ComponentT<T>::value() {
        return _value;
    }

	template<typename T>
	const std::shared_ptr<T>& ComponentT<T>::operator->() {
		return _value;
	}
    
    template<typename T>
    const component_hierarchy ComponentT<T>::_Hierarchy(std::vector<std::type_index>{typeid(Component),typeid(ComponentT<T>)});
    
}

#endif
