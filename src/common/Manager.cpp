/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include <cassert>

#include "Manager.h"

namespace wntr {
    
    void Manager::addEntity(const std::shared_ptr<Entity>& entity) {
        assert(entity);
        _entities.add(entity);
    }
    
    void Manager::removeEntity(const std::shared_ptr<Entity>& entity) {
        assert(entity);
        _entities.remove(entity);
    }
    
    const shared_vector<Entity>& Manager::getEntity() const {
        return _entities;
    }
    
}