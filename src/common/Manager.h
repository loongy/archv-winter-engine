/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_MANAGER_H_
#define _WNTR_MANAGER_H_

#include "utility.h"

namespace wntr {
    
    class Entity;
    
    class Manager {
    public:
#ifndef _MSC_VER
		Manager(Manager&&) = default;
		Manager& operator=(Manager&&) = default;
#endif
		Manager(const Manager&) = delete;
		Manager& operator=(const Manager&) = delete;
        
        explicit Manager() = default;
		virtual ~Manager() = default;
        
        void addEntity(const std::shared_ptr<Entity>& entity);
        void removeEntity(const std::shared_ptr<Entity>& entity);
        
        const shared_vector<Entity>& getEntity() const;
        
    protected:
        shared_vector<Entity> _entities;
        
    private:
        
    };
    
}

#endif