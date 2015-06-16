/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef __linux

#ifndef _WNTR_OPENGLX11RENDERCONTEXT_H_
#define _WNTR_OPENGLX11RENDERCONTEXT_H_

#include "../openglRenderContext.h"

struct __GLXcontextRec;

namespace wntr {

	class x11Window;

	class openglx11RenderContext : public openglRenderContext {
	public:
#ifndef _MSC_VER
		openglx11RenderContext(openglx11RenderContext&&) = default;
		openglx11RenderContext& operator= (openglx11RenderContext&&) = default;
#endif
		openglx11RenderContext(const openglx11RenderContext&) = delete;
		openglx11RenderContext& operator= (const openglx11RenderContext&) = delete;

		explicit openglx11RenderContext(const std::shared_ptr<x11Window>& window, unsigned width, unsigned height, bool vsync, bool fullscreen);
		virtual ~openglx11RenderContext();

		virtual void swapBuffers() override;

		virtual void lock() override;
		virtual void unlock() override;

		virtual void setVsync(bool vsync) override;

	protected:

	private:
		bool getNewerOpenGLFunctionPointers();

		std::shared_ptr<x11Window> _window;
		__GLXcontextRec* _gl;

		typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

		glXCreateContextAttribsARBProc _glXCreateContextAttribsARB;

	};
    
}

#endif
#endif
