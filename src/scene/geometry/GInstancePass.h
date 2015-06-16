/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifndef _WNTR_GINSTANCEPASS_H_
#define _WNTR_GINSTANCEPASS_H_

#include "../RenderPass.h"

namespace wntr {
    
    class Mesh;
    class MeshInstance;
    
	class GInstancePass : public RenderPass {
    public:
		static std::shared_ptr<GInstancePass> Create(const RenderContext* context);
        
#ifndef _MSC_VER
		GInstanceRenderPass(GInstanceRenderPass&&) = default;
		GInstanceRenderPass& operator=(GInstanceRenderPass&&) = default;
#endif
		GInstancePass(const GInstancePass&) = delete;
		GInstancePass& operator=(const GInstancePass&) = delete;
        
		explicit GInstancePass(const RenderContext* context);
		virtual ~GInstancePass() = default;
        
		virtual void drawBatch(RenderContext* context) override;
		virtual void addToBatch(const std::shared_ptr<Entity>& entity, const glm::mat4& world, RenderContext* context) override;
        
    protected:
        
    private:
		void createProgram(const RenderContext* context);

        std::unordered_map<std::shared_ptr<Mesh>, std::vector<glm::mat4>> _batch;
        
    };
    
}

#endif
