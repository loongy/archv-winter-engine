/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#pragma once
#ifndef _WNTR_UIBITMAPPASS_H_
#define _WNTR_UIBITMAPPASS_H_

#include "../RenderPass.h"

namespace wntr {

	class AttributeArray;

	class UIBitmapPass : public RenderPass {
	public:
		static std::shared_ptr<UIBitmapPass> Create(const RenderContext* context);

#ifndef _MSC_VER
		UIRenderPass(UIRenderPass&&) = default;
		UIRenderPass& operator=(UIRenderPass&&) = default;
#endif
		UIBitmapPass(const UIBitmapPass&) = delete;
		UIBitmapPass& operator=(const UIBitmapPass&) = delete;

		explicit UIBitmapPass(const RenderContext* context);
		virtual ~UIBitmapPass() = default;

		virtual void drawBatch(RenderContext* context) override;
		virtual void addToBatch(const std::shared_ptr<Entity>& entity, const glm::mat4& world, RenderContext* context) override;

	protected:

	private:
		void createAttributeArray(const RenderContext* context);
		void createProgram(const RenderContext* context);

		std::shared_ptr<AttributeArray> _attribute_array;
		std::unordered_map<std::shared_ptr<Texture>, std::vector<glm::mat4>> _batch;

	};

}

#endif
