/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_COLLECTION_H_
#define _WNTR_COLLECTION_H_

#include <cassert>
#include <memory>

#include "Component.h"

namespace wntr {
    
    template<typename T>
    class Collection : public Component {
    public:
		static std::shared_ptr<Collection<T>> Create();

#ifndef _MSC_VER
		Collection(Collection&&) = default;
		Collection& operator=(Collection&&) = default;
#endif
		Collection(const Collection&) = delete;
		Collection& operator=(const Collection&) = delete;
        
		explicit Collection() = default;
		virtual ~Collection() = default;
        
        virtual const component_hierarchy& hierarchy() const override;
        
        void addComponent(const std::shared_ptr<T>& com);
        void removeComponent(const std::shared_ptr<T>& com);
        
        const shared_vector<T>& getComponents() const;
        
    protected:
        shared_vector<T> _components;
        
    private:
        static const component_hierarchy _Hierarchy;
        
    };

	template<typename T>
	std::shared_ptr<Collection<T>> Collection<T>::Create() {
		return std::make_shared<Collection<T>>();
	}

    template<typename T>
    const component_hierarchy& Collection<T>::hierarchy() const {
        return _Hierarchy;
    }
    
    template<typename T>
    void Collection<T>::addComponent(const std::shared_ptr<T>& com) {
        assert(com);
        _components.add(com);
    }
    
    template<typename T>
    void Collection<T>::removeComponent(const std::shared_ptr<T>& com) {
        assert(com);
        _components.remove(com);
    }
    
    template<typename T>
    const shared_vector<T>& Collection<T>::getComponents() const {
        return _components;
    }
    
    template<typename T>
	const component_hierarchy Collection<T>::_Hierarchy(std::vector<std::type_index>{typeid(Component), typeid(Collection<T>)});
    
}

#endif