/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifndef _WNTR_GPASS_H_
#define _WNTR_GPASS_H_

#include "../RenderPass.h"

namespace wntr {

	class Material;
	class Mesh;
    
    class GPass : public RenderPass {
    public:
		static std::shared_ptr<GPass> Create(const RenderContext* context);
        
#ifndef _MSC_VER
		GPass(GPass&&) = default;
		GPass& operator=(GPass&&) = default;
#endif
		GPass(const GPass&) = delete;
		GPass& operator=(const GPass&) = delete;
        
		explicit GPass(const RenderContext* context);
		virtual ~GPass() = default;
        
		virtual void drawBatch(RenderContext* context) override;
		virtual void addToBatch(const std::shared_ptr<Entity>& entity, const glm::mat4& world, RenderContext* context) override;
        
    protected:

    private:
		void createProgram(const RenderContext* context);

		std::unordered_map< std::shared_ptr<Material>, 
			                std::vector< std::pair<glm::mat4, std::shared_ptr<Mesh>> 
							           > 
		                  > _batch;

    };
    
}

#endif