/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include <cassert>

#include <common/Entity.h>
#include <common/SceneProcess.h>
#include <common/Transform2D.h>

#include "UIRenderer.h"

#include "Bitmap.h"
#include "UIBitmapPass.h"

namespace wntr {

	std::shared_ptr<UIRenderer> UIRenderer::Create(unsigned width, unsigned height, const std::shared_ptr<RenderContext>& context) {
		return std::make_shared<UIRenderer>(width, height, context);
	}

	UIRenderer::UIRenderer(unsigned width, unsigned height, const std::shared_ptr<RenderContext>& context) 
	: Renderer(context), _width(width), _height(height) {
		assert(context);

		_bitmap_pass = UIBitmapPass::Create(context.get()); assert(_bitmap_pass);
		_bitmap_pass->setUniform(UNIFORM_PROJECTION, glm::ortho(0.0f, (float)_width, (float)_height, 0.0f), _context.get());
        
        _scene_process = SceneProcess::Create(); assert(_scene_process);
		_scene_process->onNodeProcess = [this](const std::shared_ptr<Entity>& entity, const glm::mat4& world) {
			_bitmap_pass->addToBatch(entity, world, _context.get());
        };
    }

	void UIRenderer::draw() {
        assert(_scene_process);
		_scene_process->process(_entities);
		assert(_bitmap_pass);
		_bitmap_pass->bind(_context.get());
		_bitmap_pass->drawBatch(_context.get());
	}

	void UIRenderer::resize(unsigned width, unsigned height) {
		assert(_bitmap_pass);
		_bitmap_pass->setUniform(UNIFORM_PROJECTION, glm::ortho(0.0f, (float)_width, (float)_height, 0.0f), _context.get());
        _width  = width;
		_height = height;
	}

}