/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifndef _WNTR_LORTHOPASS_H_
#define _WNTR_LORTHOPASS_H_

#include "../RenderPass.h"

namespace wntr {
    
    class AttributeArray;
    
    class LOrthoPass : public RenderPass {
    public:
		static std::shared_ptr<LOrthoPass> Create(const RenderContext* context);
        
#ifndef _MSC_VER
		LOrthoPass(LOrthoPass&&) = default;
		LOrthoPass& operator=(LOrthoPass&&) = default;
#endif
		LOrthoPass(const LOrthoPass&) = delete;
		LOrthoPass& operator=(const LOrthoPass&) = delete;
        
		explicit LOrthoPass(const RenderContext* context);
		virtual ~LOrthoPass() = default;
        
        virtual void bind(RenderContext* context) override;
		virtual void drawBatch(RenderContext* context) override;
		virtual void addToBatch(const std::shared_ptr<Entity>& entity, const glm::mat4& world, RenderContext* context) override;
        
    protected:
        
    private:
		void createAttributeArray(const RenderContext* context);
		void createProgram(const RenderContext* context);

        std::shared_ptr<AttributeArray> _attribute_array;
		std::vector<glm::vec4> _batch_direction;
		std::vector<glm::vec4> _batch_color;
        
    };
    
}

#endif
