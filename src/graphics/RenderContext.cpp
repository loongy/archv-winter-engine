/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "RenderContext.h"

#ifdef _WIN32
#include "impl-opengl/impl-win32/openglwin32RenderContext.h"
#include <platform/impl-win32/win32Window.h>
#elif __APPLE__
#include "impl-opengl/impl-osx/openglosxRenderContext.h"
#include <platform/impl-osx/osxWindow.h>
#elif __linux
#include "impl-opengl/impl-x11/openglx11RenderContext.h"
#include <platform/impl-x11/x11Window.h>
#endif

namespace wntr {

	std::shared_ptr<RenderContext> RenderContext::Create(const std::shared_ptr<Window>& window, unsigned width, unsigned height, bool vsync, bool fullscreen, API api) {
#ifdef _WIN32
		switch (api) {
		case API::OPENGL:
			return std::make_shared<openglwin32RenderContext>(std::static_pointer_cast<win32Window>(window), width, height, vsync, fullscreen);
		default:
			return nullptr;
		}
#elif __APPLE__
		switch (api) {
		case API::OPENGL:
			return std::make_shared<openglosxRenderContext>(std::static_pointer_cast<osxWindow>(window), width, height, vsync, fullscreen);
		default:
			return nullptr;
		}
#elif __linux
		switch (api) {
		case API::OPENGL:
			return std::make_shared<openglx11RenderContext>(std::static_pointer_cast<x11Window>(window), width, height, vsync, fullscreen);
		default:
			return nullptr;
		}
#endif
	}
    
	RenderContext::RenderContext(unsigned width, unsigned height, bool vsync, bool fullscreen, API api)
	: _width(width), _height(height), _vsync(vsync), _fullscreen(fullscreen), _version(API_VERSION::UNKNOWN), _api(api) {

	}

	void RenderContext::setVsync(bool vsync) {
		_vsync = vsync;
	}

	void RenderContext::setFullscreen(bool fullscreen) {
		_fullscreen = fullscreen;
	}

	unsigned RenderContext::getWidth() const {
		return _width;
	}

	unsigned RenderContext::getHeight() const {
		return _height;
	}

	bool RenderContext::isVsyncing() const {
		return _vsync;
	}

	bool RenderContext::isFullscreen() const {
		return _fullscreen;
	}

	API_VERSION RenderContext::getVersion() const {
		return _version;
	}

	API RenderContext::getApi() const {
		return _api;
	}

}
