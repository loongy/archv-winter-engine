/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include "DynamicProcess.h"

#include <cassert>

namespace wntr {

	std::shared_ptr<DynamicProcess> DynamicProcess::Create() {
		return std::make_shared<DynamicProcess>();
	}

	DynamicProcess::DynamicProcess()
		: onDynamicProcess(nullptr), onDynamicFilter(nullptr) {

	}

	void DynamicProcess::onProcess(const std::shared_ptr<Entity>& entity) {
        assert(entity);
		if (onDynamicProcess)
            onDynamicProcess(entity);
	}
    
    bool DynamicProcess::onFilter(const std::shared_ptr<Entity>& entity) const {
        assert(entity);
		if (onDynamicFilter)
            return onDynamicFilter(entity);
        return true;
	}

};