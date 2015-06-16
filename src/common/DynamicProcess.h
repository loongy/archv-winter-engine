/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#pragma once
#ifndef _WNTR_DYNAMICPROCESS_H_
#define _WNTR_DYNAMICPROCESS_H_

#include "Process.h"

namespace wntr {

	class DynamicProcess : public Process {
	public:
		static std::shared_ptr<DynamicProcess> Create();

#ifndef _MSC_VER
		DynamicProcess(DynamicProcess&&) = default;
		DynamicProcess& operator=(DynamicProcess&&) = default;
#endif
		DynamicProcess(const DynamicProcess&) = delete;
		DynamicProcess& operator=(const DynamicProcess&) = delete;

		explicit DynamicProcess();
		virtual ~DynamicProcess() = default;

		virtual void onProcess(const std::shared_ptr<Entity>& entity) override;
        virtual bool onFilter(const std::shared_ptr<Entity>& entity) const override;

		std::function<void(const std::shared_ptr<Entity>&)> onDynamicProcess;
        std::function<bool(const std::shared_ptr<Entity>&)> onDynamicFilter;

	protected:

	private:

	};

};

#endif