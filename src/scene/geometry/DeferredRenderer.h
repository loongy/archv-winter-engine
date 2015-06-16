/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_DEFERREDRENDERER_H_
#define _WNTR_DEFERREDRENDERER_H_

#include "../Renderer.h"

namespace wntr {
    
    class Framebuffer;
    class GInstancePass;
    class GPass;
	class GSPass;
    class LCompositionEffect;
    class LOrthoPass;
	class LSOrthoEffect;
    class LPointPass;
    class LSPointEffect;
    class SceneProcess;
	
    class DeferredRenderer : public Renderer {
    public:
        static std::shared_ptr<DeferredRenderer> Create(const std::shared_ptr<Entity>& camera, const std::shared_ptr<RenderContext>& context);
        
#ifndef _MSC_VER
		DeferredRenderer(DeferredRenderer&&) = default;
		DeferredRenderer& operator=(DeferredRenderer&&) = default;
#endif
		DeferredRenderer(const DeferredRenderer&) = delete;
		DeferredRenderer& operator=(const DeferredRenderer&) = delete;
        
		explicit DeferredRenderer(const std::shared_ptr<Entity>& camera, const std::shared_ptr<RenderContext>& context);
		virtual ~DeferredRenderer() = default;
        
        virtual void draw() override;
        
        void addLight (const std::shared_ptr<Entity>& light);
        void addDirectionalLight (const std::shared_ptr<Entity>& light);
        
        void removeLight (const std::shared_ptr<Entity>& light);
        void removeDirectionalLight (const std::shared_ptr<Entity>& light);
        
        void setCamera(const std::shared_ptr<Entity>& camera);
        void setAmbient(float red, float green, float blue);
        
        const std::shared_ptr<Entity>& getCamera() const;
        
    protected:
        
    private:
        void createFramebuffers();
        void createPassesAndEffects();

		void drawGeometry();
		void drawLights();
		void drawLightsWithShadows();
		void drawEffects();
		void drawPostProcessEffects();

        std::shared_ptr<Entity> _camera;
		std::shared_ptr<SceneProcess> _scene_process;
		std::shared_ptr<SceneProcess> _scene_shadow_process;
        
        float _ambient[3];
		shared_vector<Entity> _lights_directional;
		shared_vector<Entity> _lights_point;
        
		std::shared_ptr<Framebuffer> _g_framebuffer;
		std::shared_ptr<Framebuffer> _l_framebuffer;
        
		std::shared_ptr<GPass> _g_pass;
		std::shared_ptr<GInstancePass> _g_instance_pass;

		std::shared_ptr<LOrthoPass> _l_ortho_pass;
		std::shared_ptr<LPointPass> _l_point_pass;

		std::shared_ptr<GSPass> _gs_pass;
		std::shared_ptr<LSOrthoEffect> _ls_ortho_effect;
        std::shared_ptr<LSPointEffect> _ls_point_effect;

		std::shared_ptr<LCompositionEffect> _l_composition_effect;
        
    };
    
}

#endif