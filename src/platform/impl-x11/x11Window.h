/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef __linux

#ifndef _WNTR_X11WINDOW_H_
#define _WNTR_X11WINDOW_H_

#include<GL/glx.h>

#include "../Window.h"

struct _XDisplay;
struct __GLXFBConfigRec;
union _XEvent;

namespace wntr {
    
	class x11Window : public Window {
	public:
#ifndef _MSC_VER
		x11Window(x11Window&&) = default;
		x11Window& operator=(x11Window&&) = default;
#endif
		x11Window(const x11Window&) = delete;
		x11Window& operator=(const x11Window&) = delete;

		explicit x11Window(const std::string& name, int x, int y, unsigned width, unsigned height, bool fullscreen);
		virtual ~x11Window();

		virtual void show() override;
		virtual void hide() override;
		virtual void close() override;

		virtual void setX(int x) override;
		virtual void setY(int y) override;
		virtual void setXY(int x, int y) override;
		virtual void setWidth(unsigned width) override;
		virtual void setHeight(unsigned height) override;
		virtual void setSize(unsigned width, unsigned height) override;
		virtual void setFullscreen(bool fullscreen) override;

		void processEvent(const _XEvent& event);

		int getXWindow() const;
		__GLXFBConfigRec* getGLXFBConfig() const;
		XVisualInfo* getXVisualInfo() const;
		_XDisplay* getXDisplay() const;

	protected:

	private:
		unsigned long _xwnd;
		__GLXFBConfigRec* _config;
		XVisualInfo* _visual_info;

	};

}

#endif
#endif
