/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_RENDERCONTEXT_H_
#define _WNTR_RENDERCONTEXT_H_

#include "CommonGraphics.h"

#include <memory>

namespace wntr {

	class AttributeArray;
	class BlendState;
	class DepthStencilState;
	class Framebuffer;
	class Program;
	class RasterizerState;
	class Window;
    
    class RenderContext {
    public:
		static std::shared_ptr<RenderContext> Create(const std::shared_ptr<Window>& window, unsigned width, unsigned height, bool vsync, bool fullscreen, API api);

#ifndef _MSC_VER
		RenderContext(RenderContext&&) = default;
		RenderContext& operator= (RenderContext&&) = default;
#endif
		RenderContext(const RenderContext&) = delete;
		RenderContext& operator= (const RenderContext&) = delete;

		explicit RenderContext(unsigned width, unsigned height, bool vsync, bool fullscreen, API api);
		virtual ~RenderContext() = default;

		virtual void clear(float red, float green, float blue, float alpha, float depth, unsigned stencil) = 0;
		virtual void clearColor(float red, float green, float blue, float alpha) = 0;
		virtual void clearDepth(float depth) = 0;
		virtual void clearStencil(unsigned stencil) = 0;

		virtual void draw() = 0;
		virtual void drawInstances(unsigned count) = 0;

		virtual void swapBuffers() = 0;

		virtual void lock() = 0;
		virtual void unlock() = 0;

		virtual void setFramebuffer(const std::shared_ptr<Framebuffer>& framebuffer) = 0;
		virtual void setProgram(const std::shared_ptr<Program>& program) = 0;
		virtual void setAttributeArray(const std::shared_ptr<AttributeArray>& attribute_array) = 0;
		virtual void setBlendState(const std::shared_ptr<BlendState>& state) = 0;
		virtual void setDepthStencilState(const std::shared_ptr<DepthStencilState>& state) = 0;
		virtual void setRasterizerState(const std::shared_ptr<RasterizerState>& state) = 0;

		virtual std::shared_ptr<Framebuffer> getFramebuffer() const = 0;
		virtual std::shared_ptr<Program> getProgram() const = 0;
		virtual std::shared_ptr<AttributeArray> getAttributeArray() const = 0;
		virtual std::shared_ptr<BlendState> getBlendState() const = 0;
		virtual std::shared_ptr<DepthStencilState> getDepthStencilState() const = 0;
		virtual std::shared_ptr<RasterizerState> getRasterizerState() const = 0;

		virtual void setVsync(bool vsync);
		virtual void setFullscreen(bool fullscreen);

		unsigned getWidth() const;
		unsigned getHeight() const;
		bool isVsyncing() const;
		bool isFullscreen() const;
		API_VERSION getVersion() const;
		API getApi() const;

    protected:
		unsigned _width;
		unsigned _height;
		bool _vsync;
		bool _fullscreen;

		API_VERSION _version;
		const API _api;
		
    private:

    };
    
}

#endif
