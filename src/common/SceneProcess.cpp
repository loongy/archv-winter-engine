/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include "SceneProcess.h"

#include "Entity.h"
#include "Transform.h"

#include <cassert>

namespace wntr {

	std::shared_ptr<SceneProcess> SceneProcess::Create() {
		return std::make_shared<SceneProcess>();
	}

	SceneProcess::SceneProcess()
	: DynamicProcess(), onNodeProcess(nullptr) {

	}

	void SceneProcess::onProcess(const std::shared_ptr<Entity>& entity) {
		assert(entity);
		if (entity->getParent())
			processEntity(entity, getParentTransform(entity));
		else
			processEntity(entity);
	}

	void SceneProcess::processEntity(const std::shared_ptr<Entity>& entity) {
		assert(entity);

		// process the entity
		auto transform = entity->get<Transform>();
		if (transform) {
			// get transformation for this entity
			auto world_transform = transform->matrix();
			// filter out entities
            if (onFilter(entity)) {
                DynamicProcess::onProcess(entity);
                if (onNodeProcess)
                    onNodeProcess(entity, world_transform);
            }
			// process all children
			for (const auto& child : entity->getChildren())
				processEntity(child, world_transform);
		}
		else {
			// filter out entities
			if (onFilter(entity)) {
                DynamicProcess::onProcess(entity);
                if (onNodeProcess)
                    onNodeProcess(entity, glm::mat4());
            }
			// process all children
			for (const auto& child : entity->getChildren())
				processEntity(child);
		}
	}

	void SceneProcess::processEntity(const std::shared_ptr<Entity>& entity, const glm::mat4& parent_transform) {
		assert(entity);

		// process the entity
		auto transform = entity->get<Transform>();
		if (transform) {
			// get transformation for this entity
			auto world_transform = parent_transform * transform->matrix();
			// filter out entities
			if (onFilter(entity)) {
                DynamicProcess::onProcess(entity);
                if (onNodeProcess)
                    onNodeProcess(entity, world_transform);
            }
			// process all children
			for (const auto& child : entity->getChildren())
				processEntity(child, world_transform);
		}
		else {
			// filter out entities
            if (onFilter(entity)) {
                DynamicProcess::onProcess(entity);
                if (onNodeProcess)
                    onNodeProcess(entity, parent_transform);
            }
			// process all children
			for (const auto& child : entity->getChildren())
				processEntity(child, parent_transform);
		}
	}

	glm::mat4 SceneProcess::getParentTransform(const std::shared_ptr<Entity>& entity) const {
		assert(entity);

		//get parent information
		auto parent = entity->getParent();
		auto parent_transform = glm::mat4();

		// while there are parents to traverse
		while (parent) {
			// concatenate any transformation
			auto transform = parent->get<Transform>();
			if (transform)
				parent_transform = parent_transform * transform->matrix();
			// traverse up to the next parent
			parent = parent->getParent();
		}

		return parent_transform;
	}

};