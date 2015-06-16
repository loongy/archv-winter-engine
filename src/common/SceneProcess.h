/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#pragma once
#ifndef _WNTR_SCENEPROCESS_H_
#define _WNTR_SCENEPROCESS_H_

#include "DynamicProcess.h"

namespace wntr {

	class SceneProcess : public DynamicProcess {
	public:
		static std::shared_ptr<SceneProcess> Create();

#ifndef _MSC_VER
		SceneProcess(SceneProcess&&) = default;
		SceneProcess& operator=(SceneProcess&&) = default;
#endif
		SceneProcess(const SceneProcess&) = delete;
		SceneProcess& operator=(const SceneProcess&) = delete;

		explicit SceneProcess();
		virtual ~SceneProcess() = default;

		virtual void onProcess(const std::shared_ptr<Entity>& entity) override;

		std::function<void(const std::shared_ptr<Entity>&, const glm::mat4&)> onNodeProcess;

	protected:

		void processEntity(const std::shared_ptr<Entity>& entity);
		void processEntity(const std::shared_ptr<Entity>& entity, const glm::mat4& parent_transform);

		glm::mat4 getParentTransform(const std::shared_ptr<Entity>& entity) const;

	private:

	};

};

#endif