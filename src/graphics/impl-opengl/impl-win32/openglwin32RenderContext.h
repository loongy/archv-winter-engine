/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef _WIN32

#pragma once
#ifndef _WNTR_OPENGLWIN32RENDERCONTEXT_H_
#define _WNTR_OPENGLWIN32RENDERCONTEXT_H_

#include "../openglRenderContext.h"

namespace wntr {

	class win32Window;
    
	class openglwin32RenderContext : public openglRenderContext {
    public:
#ifndef _MSC_VER
		openglwin32RenderContext(openglwin32RenderContext&&) = default;
		openglwin32RenderContext& operator= (openglwin32RenderContext&&) = default;
#endif
		openglwin32RenderContext(const openglwin32RenderContext&) = delete;
		openglwin32RenderContext& operator= (const openglwin32RenderContext&) = delete;

		explicit openglwin32RenderContext(const std::shared_ptr<win32Window>& window, unsigned width, unsigned height, bool vsync, bool fullscreen);
		virtual ~openglwin32RenderContext();

		virtual void swapBuffers() override;

		virtual void lock() override;
		virtual void unlock() override;

		virtual void setVsync(bool vsync) override;

		const std::shared_ptr<win32Window>& getWindow() const;

    protected:

    private:
		bool getNewerOpenGLFunctionPointers();
		bool getNewerOpenGL();

		HDC _hdc;
		HGLRC _hglrc;
		std::shared_ptr<win32Window> _window;

		typedef int(__stdcall * wglChoosePixelFormatARB_PROC) (HDC, const int*, const float*, unsigned int, int*, unsigned int*);
		typedef HGLRC(__stdcall * wglCreateContextAttribsARB_PROC) (HDC, HGLRC, const int*);
		typedef int(__stdcall * wglSwapIntervalEXT_PROC) (int);
		wglChoosePixelFormatARB_PROC _wglChoosePixelFormatARB;
		wglCreateContextAttribsARB_PROC _wglCreateContextAttribsARB;
		wglSwapIntervalEXT_PROC _wglSwapIntervalEXT;
    };
    
}

#endif
#endif