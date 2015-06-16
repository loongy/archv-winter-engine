/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include <algorithm>
#include <cassert>
#include <functional>
#include <unordered_map>
#include <utility>

#include <common/Color.h>
#include <common/Direction.h>
#include <common/Entity.h>
#include <common/SceneProcess.h>
#include <common/Transform3D.h>

#include <graphics/Depthbuffer.h>
#include <graphics/Framebuffer.h>
#include <graphics/Program.h>
#include <graphics/RenderContext.h>
#include <graphics/Renderbuffer.h>

#include "DeferredRenderer.h"

#include "GInstancePass.h"
#include "GPass.h"
#include "GSPass.h"
#include "LCompositionEffect.h"
#include "LOrthoPass.h"
#include "LSOrthoEffect.h"
#include "LPointPass.h"
#include "LSPointEffect.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshInstance.h"
#include "Perspective.h"
#include "ShadowMap.h"
#include "View.h"

namespace wntr {
    
    std::shared_ptr<DeferredRenderer> DeferredRenderer::Create(const std::shared_ptr<Entity>& camera, const std::shared_ptr<RenderContext>& context) {
        return std::make_shared<DeferredRenderer>(camera, context);
    }
    
    DeferredRenderer::DeferredRenderer(const std::shared_ptr<Entity>& camera, const std::shared_ptr<RenderContext>& context)
    : Renderer(context), _camera(camera) {

		_scene_process = SceneProcess::Create(); assert(_scene_process);
		_scene_shadow_process = SceneProcess::Create(); assert(_scene_process);
        _ambient[0] = _ambient[1] = _ambient[2] = 0.0f;

		createFramebuffers();
		createPassesAndEffects();
    }
    
    void DeferredRenderer::draw() {
		drawGeometry();
		drawLights();
		drawLightsWithShadows();
		drawEffects();
		drawPostProcessEffects();
    }
    
    void DeferredRenderer::addLight(const std::shared_ptr<Entity>& light) {
        assert(light);
		_lights_point.add(light);
    }
    
    void DeferredRenderer::addDirectionalLight(const std::shared_ptr<Entity>& light) {
        assert(light);
		_lights_directional.add(light);
    }
    
    void DeferredRenderer::removeLight(const std::shared_ptr<Entity>& light) {
        assert(light);
		_lights_point.remove(light);
    }
    
    void DeferredRenderer::removeDirectionalLight(const std::shared_ptr<Entity>& light) {
        assert(light);
		_lights_directional.remove(light);
    }
    
    void DeferredRenderer::setCamera(const std::shared_ptr<Entity>& camera) {
        assert(camera);
        assert(camera->get<View>());
        assert(camera->get<Projection>());
        _camera = camera;
    }
    
    void DeferredRenderer::setAmbient(float red, float green, float blue) {
        _ambient[0] = red;
        _ambient[1] = green;
        _ambient[2] = blue;
    }
    
    const std::shared_ptr<Entity>& DeferredRenderer::getCamera() const {
        return _camera;
    }
    
	void DeferredRenderer::createFramebuffers() {
		assert(_context);

		// create geometry renderbuffers
		std::shared_ptr<Renderbuffer> gbuffers[] = {
			Renderbuffer::Create(TEXTURE_DIFFUSE,  _context->getWidth(), _context->getHeight(), TEXTURE_FORMAT::RGBA_FLOAT16, _context.get()),
			Renderbuffer::Create(TEXTURE_POSITION, _context->getWidth(), _context->getHeight(), TEXTURE_FORMAT::RGBA_FLOAT16,  _context.get()),
			Renderbuffer::Create(TEXTURE_NORMAL,   _context->getWidth(), _context->getHeight(), TEXTURE_FORMAT::RGBA_FLOAT16,  _context.get()),
			Renderbuffer::Create(TEXTURE_SPECULAR, _context->getWidth(), _context->getHeight(), TEXTURE_FORMAT::R_FLOAT16,    _context.get()),
			Renderbuffer::Create(TEXTURE_EMISSIVE, _context->getWidth(), _context->getHeight(), TEXTURE_FORMAT::R_FLOAT16,    _context.get()),
		}; assert(gbuffers[0] && gbuffers[1] && gbuffers[2] && gbuffers[3] && gbuffers[4]);

		// create depthbuffer
		auto depthbuffer = Depthbuffer::Create(_context->getWidth(), _context->getHeight(), TEXTURE_FORMAT::DEPTH24_STENCIL8, _context.get()); assert(depthbuffer);
		
		// create geometry framebuffer
		_g_framebuffer = Framebuffer::Create(5, gbuffers, depthbuffer, _context.get()); assert(_g_framebuffer);

		// create light renderbuffers
		std::shared_ptr<Renderbuffer> lbuffers[] = {
			Renderbuffer::Create(TEXTURE_LIGHT,    _context->getWidth(), _context->getHeight(), TEXTURE_FORMAT::RGBA_FLOAT16, _context.get()),
			Renderbuffer::Create(TEXTURE_SPECULAR, _context->getWidth(), _context->getHeight(), TEXTURE_FORMAT::RGBA_FLOAT16, _context.get()),
		}; assert(lbuffers[0] && lbuffers[1]);

		// create light framebuffer
		_l_framebuffer = Framebuffer::Create(2, lbuffers, nullptr, _context.get()); assert(_l_framebuffer);
	}

	void DeferredRenderer::createPassesAndEffects() {
		// create geometry render passes
		_g_pass = GPass::Create(_context.get()); assert(_g_pass);
        _g_instance_pass = GInstancePass::Create(_context.get()); assert(_g_instance_pass);
		// create lighting render passes (no shadows)
		_l_ortho_pass = LOrthoPass::Create(_context.get()); assert(_l_ortho_pass);
		_l_point_pass = LPointPass::Create(_context.get()); assert(_l_point_pass);
		// create lighting render passes (with shadows)
		_gs_pass = GSPass::Create(_context.get()); assert(_gs_pass);
		_ls_ortho_effect = LSOrthoEffect::Create(_context.get()); assert(_ls_ortho_effect);
		_ls_point_effect = LSPointEffect::Create(_context.get()); assert(_ls_point_effect);
 		// create lighting render effect
		_l_composition_effect = LCompositionEffect::Create(_context.get()); assert(_l_composition_effect);
	}

	void DeferredRenderer::drawGeometry() {
		assert(_context);

		// set and clear the geometry framebuffer
		_context->setFramebuffer(_g_framebuffer);
		_context->clear(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0);

		{ // draw geometry
			assert(_g_pass);
			_g_pass->bind(_context.get());

			// bind camera data
			auto view       = _camera->get<View>()->view();
			auto projection = _camera->get<Projection>()->projection();
			_g_pass->setUniform(UNIFORM_VIEW, view, _context.get());
			_g_pass->setUniform(UNIFORM_PROJECTION, projection, _context.get());

			// use the scene process to batch all entities
			_scene_process->onNodeProcess = [this](const std::shared_ptr<Entity>& entity, const glm::mat4& transform) {
				_g_pass->addToBatch(entity, transform, _context.get());
			};
			_scene_process->process(_entities);

			// render the batch
			_g_pass->drawBatch(_context.get());
		}

		{ // draw instance geometry
			assert(_g_instance_pass);
			_g_instance_pass->bind(_context.get());

			// bind camera data
			auto view       = _camera->get<View>()->view();
			auto projection = _camera->get<Projection>()->projection();
			_g_instance_pass->setUniform(UNIFORM_VIEW, view, _context.get());
			_g_instance_pass->setUniform(UNIFORM_PROJECTION, projection, _context.get());

			// use the scene process to collect all instances
			_scene_process->onNodeProcess = [this](const std::shared_ptr<Entity>& entity, const glm::mat4& transform) {
				_g_instance_pass->addToBatch(entity, transform, _context.get());
			};
			_scene_process->process(_entities);

			// draw all the collected instances
			_g_instance_pass->drawBatch(_context.get());
		}
	}

	void DeferredRenderer::drawLights() {
		// get the required geometry renderbuffers
		auto gbuffer_position = _g_framebuffer->getRenderbuffer(TEXTURE_POSITION);
		auto gbuffer_normal   = _g_framebuffer->getRenderbuffer(TEXTURE_NORMAL);
		auto gbuffer_specular = _g_framebuffer->getRenderbuffer(TEXTURE_SPECULAR);

		assert(_context);
		// set and clear the lighting framebuffer
		_context->setFramebuffer(_l_framebuffer);
		_context->clear(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0);

		{ // draw ortho lights
			assert(_l_ortho_pass);
			_l_ortho_pass->bind(_context.get());
			
			// bind camera data
			auto cam_pos = _camera->get<View>()->getPosition();
			_l_ortho_pass->setUniform(UNIFORM_CAMERA_POSITION, glm::vec4(cam_pos.x, cam_pos.y, cam_pos.z, 0.0f), _context.get());

			// bind the geometry renderbuffers
			_l_ortho_pass->setTexture(TEXTURE_POSITION, gbuffer_position, _context.get());
			_l_ortho_pass->setTexture(TEXTURE_NORMAL,   gbuffer_normal,   _context.get());
			_l_ortho_pass->setTexture(TEXTURE_SPECULAR, gbuffer_specular, _context.get());

			// use the scene process to batch all directional lights
			_scene_process->onNodeProcess = [this](const std::shared_ptr<Entity>& entity, const glm::mat4& transform) {
				if (!entity->get<ShadowMap>())
					_l_ortho_pass->addToBatch(entity, transform, _context.get());
			};
			_scene_process->process(_lights_directional);

			// render the batch
			_l_ortho_pass->drawBatch(_context.get());
		}

		{ // draw point lights
			assert(_l_point_pass);
			_l_point_pass->bind(_context.get());

			// bind camera data
			auto cam_pos = _camera->get<View>()->getPosition();
			_l_point_pass->setUniform(UNIFORM_CAMERA_POSITION, glm::vec4(cam_pos.x, cam_pos.y, cam_pos.z, 0.0f), _context.get());

			// bind the geometry renderbuffers
			_l_point_pass->setTexture(TEXTURE_POSITION, gbuffer_position, _context.get());
			_l_point_pass->setTexture(TEXTURE_NORMAL, gbuffer_normal, _context.get());
			_l_point_pass->setTexture(TEXTURE_SPECULAR, gbuffer_specular, _context.get());

			// use the scene process to draw all point lights
			_scene_process->onNodeProcess = [this](const std::shared_ptr<Entity>& entity, const glm::mat4& transform) {
				if (!entity->get<ShadowMap>())
					_l_point_pass->addToBatch(entity, transform, _context.get());
			};
			_scene_process->process(_lights_point);

			// render the batch
			_l_point_pass->drawBatch(_context.get());
		}
	}

	void DeferredRenderer::drawLightsWithShadows() {
		// get the required geometry renderbuffers
		auto gbuffer_position = _g_framebuffer->getRenderbuffer(TEXTURE_POSITION);
		auto gbuffer_normal   = _g_framebuffer->getRenderbuffer(TEXTURE_NORMAL);
		auto gbuffer_specular = _g_framebuffer->getRenderbuffer(TEXTURE_SPECULAR);

		assert(_context);
		// set and clear the lighting framebuffer (commented out, because this is done in the DeferredRenderer::drawLights() function)
		//_context->setFramebuffer(_framebuffer_lbuffer);
		//_context->clear(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0);

		{ // directional lighting with shadows
			_scene_shadow_process->onNodeProcess = [this](const std::shared_ptr<Entity>& entity, const glm::mat4& transform) {
				_gs_pass->addToBatch(entity, transform, _context.get());
			};
			_scene_process->onNodeProcess = [&,this](const std::shared_ptr<Entity>& entity, const glm::mat4& transform) {
				// make sure the entity has a view and a shadow map
				auto view = entity->get<View>();
				auto projection = entity->get<Projection>();
				auto shadow_map = entity->get<ShadowMap>();
				if (view && projection && shadow_map) {
					{ // generate a shadow map using all geometry
						// draw the shadow map
						assert(_gs_pass);
						_gs_pass->bind(_context.get());
						_context->setFramebuffer(shadow_map->getFramebuffer());
						_context->clear(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0);

						// bind light transformation data
						_gs_pass->setUniform(UNIFORM_LIGHT_VIEW, view->view(), _context.get());
						_gs_pass->setUniform(UNIFORM_LIGHT_PROJECTION, projection->projection(), _context.get());

						// use the shadow process to batch all entities
						_scene_shadow_process->process(_entities);

						// render the batch
						_gs_pass->drawBatch(_context.get());
					}					
					{ // draw the light entity using the generated shadow map
						// draw the light
						assert(_ls_ortho_effect);
						_ls_ortho_effect->bind(_context.get());
						_context->setFramebuffer(_l_framebuffer);

						// bind the geometry renderbuffers
						_ls_ortho_effect->setTexture(TEXTURE_SHADOW, shadow_map->getFramebuffer()->getRenderbuffer(TEXTURE_SHADOW), _context.get());
						_ls_ortho_effect->setTexture(TEXTURE_POSITION, gbuffer_position, _context.get());
						_ls_ortho_effect->setTexture(TEXTURE_NORMAL, gbuffer_normal, _context.get());
						_ls_ortho_effect->setTexture(TEXTURE_SPECULAR, gbuffer_specular, _context.get());

						// bind all the data
						auto cam_pos = _camera->get<View>()->getPosition();
						_ls_ortho_effect->setUniform(UNIFORM_CAMERA_POSITION, glm::vec4(cam_pos.x, cam_pos.y, cam_pos.z, 0.0f), _context.get());
						_ls_ortho_effect->setUniform(UNIFORM_LIGHT_DIRECTION, glm::vec4(view->getForward(), 0.0f), _context.get());
						_ls_ortho_effect->setUniform(UNIFORM_LIGHT_VIEW, view->view(), _context.get());
						_ls_ortho_effect->setUniform(UNIFORM_LIGHT_PROJECTION, projection->projection(), _context.get());
						auto color = entity->get<Color>();
						if (color)
							_ls_ortho_effect->setUniform(UNIFORM_LIGHT_COLOR, color->getColor(), _context.get());
						else
							_ls_ortho_effect->setUniform(UNIFORM_LIGHT_COLOR, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), _context.get());

						// draw
						_ls_ortho_effect->draw(_context.get());
					}
				}
			};
			_scene_process->process(_lights_directional);
		}
        
        { // point lighting with shadows
			const static auto perspective_ptr = Perspective::Create(1.0f, glm::pi<float>()*0.5f);
			const static auto view_ptr = View::Create();
            const static glm::vec3 view_map[] = {
				glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
				glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
            };

			_scene_shadow_process->onNodeProcess = [this](const std::shared_ptr<Entity>& entity, const glm::mat4& transform) {
				_gs_pass->addToBatch(entity, transform, _context.get());
			};
            
			_scene_process->onNodeProcess = [&,this](const std::shared_ptr<Entity>& entity, const glm::mat4& world) {
				// make sure the entity has a view and a shadow map
				auto transform = entity->get<Transform3D>();
				auto shadow_map = entity->get<ShadowMap>();
				if (transform && shadow_map) {
                    // loop over the six directions
                    for (unsigned i = 0; i < 6; i++) {
						{ // generate a shadow map using all geometry
							// draw the shadow map
							assert(_gs_pass);
							_gs_pass->bind(_context.get());
							_context->setFramebuffer(shadow_map->getFramebuffer());
							_context->clear(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0);

							// calculate the current light view
							view_ptr->setPosition(transform->getPosition());
							view_ptr->setForward(view_map[i*3 + 0]);
							view_ptr->setRight(view_map[i*3 + 1]);
							view_ptr->setUp(view_map[i*3 + 2]);

							// bind light transformation data
							_gs_pass->setUniform(UNIFORM_LIGHT_VIEW, view_ptr->view(), _context.get());
							_gs_pass->setUniform(UNIFORM_LIGHT_PROJECTION, perspective_ptr->projection(), _context.get());

							// use the shadow process to batch all entities
							_scene_shadow_process->process(_entities);

							// render batch
							_gs_pass->drawBatch(_context.get());
						}
						{ // draw the light entity using the generated shadow map
							// draw the light
							assert(_ls_point_effect);
							_ls_point_effect->bind(_context.get());
							_context->setFramebuffer(_l_framebuffer);

							// bind the geometry renderbuffers
							_ls_point_effect->setTexture(TEXTURE_SHADOW, shadow_map->getFramebuffer()->getRenderbuffer(TEXTURE_SHADOW), _context.get());
							_ls_point_effect->setTexture(TEXTURE_POSITION, gbuffer_position, _context.get());
							_ls_point_effect->setTexture(TEXTURE_NORMAL, gbuffer_normal, _context.get());
							_ls_point_effect->setTexture(TEXTURE_SPECULAR, gbuffer_specular, _context.get());

							// bind data
							auto cam_pos = _camera->get<View>()->getPosition();
							_ls_point_effect->setUniform(UNIFORM_CAMERA_POSITION, glm::vec4(cam_pos.x, cam_pos.y, cam_pos.z, 0.0f), _context.get());
							_ls_point_effect->setUniform(UNIFORM_LIGHT_VIEW, view_ptr->view(), _context.get());
							_ls_point_effect->setUniform(UNIFORM_LIGHT_POSITION, glm::vec4(world[3][0], world[3][1], world[3][2], 0.0f), _context.get());
							_ls_point_effect->setUniform(UNIFORM_LIGHT_PROJECTION, perspective_ptr->projection(), _context.get());
							auto color = entity->get<Color>();
							if (color)
								_ls_point_effect->setUniform(UNIFORM_LIGHT_COLOR, color->getColor(), _context.get());
							else
								_ls_point_effect->setUniform(UNIFORM_LIGHT_COLOR, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), _context.get());

							// draw
							_ls_point_effect->draw(_context.get());
						}
                    }
				}
			};
            
			_scene_process->process(_lights_point);
		}
	}
    
	void DeferredRenderer::drawEffects() {
		// get the required geometry renderbuffers
		auto gbuffer_diffuse  = _g_framebuffer->getRenderbuffer(TEXTURE_DIFFUSE);
		auto gbuffer_emissive = _g_framebuffer->getRenderbuffer(TEXTURE_EMISSIVE);

		// get the required lighting renderbuffers
		auto lbuffer_light    = _l_framebuffer->getRenderbuffer(TEXTURE_LIGHT);
		auto lbuffer_specular = _l_framebuffer->getRenderbuffer(TEXTURE_SPECULAR);

		assert(_context);
		// set and clear the default framebuffer
		_context->setFramebuffer(nullptr);
		_context->clear(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0);

		{ // compose all of the lighting and geometry renderbuffers
			assert(_l_composition_effect);
			_l_composition_effect->bind(_context.get());

			// set ambient lighting data
			_l_composition_effect->setUniform(UNIFORM_LIGHT_AMBIENT, glm::vec4(_ambient[0], _ambient[1], _ambient[2], 1.0f), _context.get());

			// bind geometry and lighting renderbuffers
			_l_composition_effect->setTexture(TEXTURE_DIFFUSE, gbuffer_diffuse, _context.get());
			_l_composition_effect->setTexture(TEXTURE_LIGHT, lbuffer_light, _context.get());
			_l_composition_effect->setTexture(TEXTURE_SPECULAR, lbuffer_specular, _context.get());
			_l_composition_effect->setTexture(TEXTURE_EMISSIVE, gbuffer_emissive, _context.get());

			// draw the effect
			_l_composition_effect->draw(_context.get());
		}
	}

	void DeferredRenderer::drawPostProcessEffects() {
		
	}

}
