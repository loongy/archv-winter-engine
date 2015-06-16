/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include <cassert>

#include "Component.h"
#include "Entity.h"

namespace wntr {
    
	std::shared_ptr<Entity> Entity::Create(const std::vector<std::shared_ptr<Component>>& components) {
		return std::make_shared<Entity>("", components);
	}

	std::shared_ptr<Entity> Entity::Create(const std::string& name, const std::vector<std::shared_ptr<Component>>& components) {
		return std::make_shared<Entity>(name, components);
	}
    
	Entity::Entity(const std::string& name, const std::vector<std::shared_ptr<Component>>& components)
    : std::enable_shared_from_this<Entity>(), _name(name) {
        for (const auto& com : components) {
            assert(com);
            attach(com);
        }
    }

    void Entity::attach(const std::shared_ptr<Component>& component) {
        assert(component);
        const auto& hierarchy = component->hierarchy().types();
        for (const auto& type : hierarchy)
            if (type != std::type_index(typeid(Component)))
                _components[type] = component;
    }
    
    void Entity::detach(const std::shared_ptr<Component>& component) {
        assert(component);
        const auto& hierarchy = component->hierarchy().types();
        for (const auto& type : hierarchy)
            if (_components[type].get() == component.get())
                _components[type] = nullptr;
    }
    
    void Entity::addChild(const std::shared_ptr<Entity>& child) {
        assert(child);
        
        if (child->_parent.lock())
            throw std::invalid_argument("error: Entity::addChild: child already has a parent");
        
        _children.add(child);
        child->_parent = shared_from_this();
    }
    
    void Entity::removeChild(const std::shared_ptr<Entity>& child) {
        assert(child);
        
        if (auto parent = child->_parent.lock())
            if (parent.get() != this)
                return;
        
        _children.remove(child);
        child->_parent.reset();
    }

	void Entity::setName(const std::string& name) {
		_name = name;
	}

	const std::string& Entity::getName() const {
		return _name;
	}
    
    std::shared_ptr<Entity> Entity::getParent() const {
        return _parent.lock();
    }
    
    const shared_vector<Entity>& Entity::getChildren() const {
        return _children;
    }
}