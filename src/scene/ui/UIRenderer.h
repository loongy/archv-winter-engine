/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_UIRENDERER_H_
#define _WNTR_UIRENDERER_H_

#include "../Renderer.h"

namespace wntr {

	class RenderContext;
	class SceneProcess;
	class UIBitmapPass;

	class UIRenderer : public Renderer {
	public:
		static std::shared_ptr<UIRenderer> Create(unsigned width, unsigned height, const std::shared_ptr<RenderContext>& context);

#ifndef _MSC_VER
		UIRenderer(UIRenderer&&) = default;
		UIRenderer& operator= (UIRenderer&&) = default;
#endif
		UIRenderer(const UIRenderer&) = delete;
		UIRenderer& operator= (const UIRenderer&) = delete;

		explicit UIRenderer(unsigned width, unsigned height, const std::shared_ptr<RenderContext>& context);
		virtual ~UIRenderer() = default;

		virtual void draw() override;

		void resize(unsigned width, unsigned height);

	protected:

	private:
		unsigned _width;
		unsigned _height;

		std::shared_ptr<SceneProcess> _scene_process;
        std::shared_ptr<UIBitmapPass> _bitmap_pass;
	};

}

#endif