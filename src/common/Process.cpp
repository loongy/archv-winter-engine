/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include "Process.h"

namespace wntr {

	void Process::process(const shared_vector<Entity>& entities) {
		for (const auto& entity : entities)
			if (entity)
                if (onFilter(entity))
                    onProcess(entity);
	}
    
    bool Process::onFilter(const std::shared_ptr<Entity>& entity) const {
        return true;
    }

};