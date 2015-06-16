/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifndef _WNTR_LPOINTPASS_H_
#define _WNTR_LPOINTPASS_H_

#include "../RenderPass.h"

namespace wntr {
    
    class AttributeArray;
    
    class LPointPass : public RenderPass {
    public:
		static std::shared_ptr<LPointPass> Create(const RenderContext* context);
        
#ifndef _MSC_VER
		LPointPass(LPointPass&&) = default;
		LPointPass& operator=(LPointPass&&) = default;
#endif
		LPointPass(const LPointPass&) = delete;
		LPointPass& operator=(const LPointPass&) = delete;
        
		explicit LPointPass(const RenderContext* context);
		virtual ~LPointPass() = default;
        
        virtual void bind(RenderContext* context) override;
		virtual void drawBatch(RenderContext* context) override;
		virtual void addToBatch(const std::shared_ptr<Entity>& entity, const glm::mat4& world, RenderContext* context) override;

	protected:

	private:
		void createAttributeArray(const RenderContext* context);
		void createProgram(const RenderContext* context);

		std::shared_ptr<AttributeArray> _attribute_array;
		std::vector<glm::vec4> _batch_position;
		std::vector<glm::vec4> _batch_color;
        
    };
    
}

#endif