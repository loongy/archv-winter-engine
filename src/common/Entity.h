/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_ENTITY_H_
#define _WNTR_ENTITY_H_

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "utility.h"

namespace wntr {
    
    class Component;
    
    class Entity : public std::enable_shared_from_this<Entity> {
    public:
		static std::shared_ptr<Entity> Create(const std::vector<std::shared_ptr<Component>>& components);
		static std::shared_ptr<Entity> Create(const std::string& name = "", const std::vector<std::shared_ptr<Component>>& components = {});
        
#ifndef _MSC_VER
		Entity(Entity&&) = default;
		Entity& operator=(Entity&&) = default;
#endif
		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;
        
		explicit Entity(const std::string& name, const std::vector<std::shared_ptr<Component>>& components);
		virtual ~Entity() = default;
        
        void attach(const std::shared_ptr<Component>& component);
        void detach(const std::shared_ptr<Component>& component);
        
        template<typename T>
        std::shared_ptr<T> get() const;
        
        void addChild(const std::shared_ptr<Entity>& child);
        void removeChild(const std::shared_ptr<Entity>& child);

		void setName(const std::string& name);
        
		const std::string&           getName() const;
        std::shared_ptr<Entity>      getParent() const;
        const shared_vector<Entity>& getChildren() const;
        
    protected:
    private:
        std::unordered_map<std::type_index, std::shared_ptr<Component>> _components;
        
		std::string _name;
        std::weak_ptr<Entity> _parent;
        shared_vector<Entity> _children;
        
    };
    
    template<typename T>
    std::shared_ptr<T> Entity::get() const {
        auto it = _components.find(std::type_index(typeid(T)));
        if (it != _components.end())
            return std::static_pointer_cast<T>(it->second);
        else
            return nullptr;
    }
    
}

#endif