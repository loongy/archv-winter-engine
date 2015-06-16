/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef __linux

#include<X11/X.h>
#include<X11/Xlib.h>
#include<X11/keysym.h>
#include<GL/gl.h>

#include "x11Application.h"
#include "x11Window.h"
#include "../Event.h"
#include "../EventHandler.h"

namespace wntr {

	x11Window::x11Window(const std::string& name, int x, int y, unsigned width, unsigned height, bool fullscreen)
	: Window(name, x, y, width, height, fullscreen) {

		assert(x11Application::_Display);
		::Window root = DefaultRootWindow(x11Application::_Display);

		GLint glattribs[] = {
			GLX_X_RENDERABLE    , True,
			GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
			GLX_RENDER_TYPE     , GLX_RGBA_BIT,
			GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
			GLX_RED_SIZE        , 8,
			GLX_GREEN_SIZE      , 8,
			GLX_BLUE_SIZE       , 8,
			GLX_ALPHA_SIZE      , 8,
			GLX_DEPTH_SIZE      , 24,
			GLX_STENCIL_SIZE    , 8,
			GLX_DOUBLEBUFFER    , True,
			None
		};

		int fb_count;
		GLXFBConfig* configs = glXChooseFBConfig(x11Application::_Display, DefaultScreen(x11Application::_Display), glattribs, &fb_count);
		if (!configs)
			throw std::runtime_error("error: x11Window::x11Window: could not find a GLXFBConfig: " + _name);

		int optimal_config = -1, best_num_samp = -1;
		for (int i = 0; i < fb_count; ++i) {
			XVisualInfo *vi = glXGetVisualFromFBConfig(x11Application::_Display, configs[i]);
			if (vi) {
				int samp_buf, samples;
				glXGetFBConfigAttrib(x11Application::_Display, configs[i], GLX_SAMPLE_BUFFERS, &samp_buf );
				glXGetFBConfigAttrib(x11Application::_Display, configs[i], GLX_SAMPLES       , &samples  );
				if ((optimal_config < 0 || samp_buf) && samples > best_num_samp)
					optimal_config = i, best_num_samp = samples;
				XFree(vi);
			}
		}

		_config = configs[optimal_config];
		XFree(configs);
		if (!_config)
			throw std::runtime_error("error: x11Window::x11Window: could not choose GLXFBConfig: " + _name);

		_visual_info = glXGetVisualFromFBConfig(x11Application::_Display, _config);
		if (!_visual_info)
			throw std::runtime_error("error: x11Window::x11Window: could not choose XVisualInfo: " + _name);

		Colormap color_map = XCreateColormap(x11Application::_Display, root, _visual_info->visual, AllocNone);
		XSetWindowAttributes xattribs;
		xattribs.colormap = color_map;
		xattribs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;

		_xwnd = XCreateWindow(x11Application::_Display, root,
				x, y,
				width, height,
				0,
				_visual_info->depth, InputOutput, _visual_info->visual,
				CWColormap | CWEventMask, &xattribs);
		if (!_xwnd)
			throw std::runtime_error("error: x11Window::x11Window: could not create Window: " + _name);

		XStoreName(x11Application::_Display, _xwnd, _name.c_str());

		x11Application::_Windows[_xwnd] = this;
	}

	x11Window::~x11Window() {
		assert(_xwnd);
		assert(x11Application::_Display);

		XDestroyWindow(x11Application::_Display, _xwnd);
		x11Application::_Windows.erase(_xwnd);
		_xwnd = 0;
	}

	void x11Window::show() {
		assert(_xwnd);
		assert(x11Application::_Display);
		XMapWindow(x11Application::_Display, _xwnd);
	}

	void x11Window::hide() {
		assert(_xwnd);
		assert(x11Application::_Display);
		XUnmapWindow(x11Application::_Display, _xwnd);
	}

	void x11Window::close() {

	}

	void x11Window::setX(int x) {

	}

	void x11Window::setY(int y) {

	}

	void x11Window::setXY(int x, int y) {

	}

	void x11Window::setWidth(unsigned width) {

	}

	void x11Window::setHeight(unsigned height) {

	}

	void x11Window::setSize(unsigned width, unsigned height) {

	}

	void x11Window::setFullscreen(bool fullscreen) {

	}


	void x11Window::processEvent(const _XEvent& event) {
		switch (event.type) {
		case ButtonPress: {
			MOUSE_EVENT_CODE code = MOUSE_EVENT_CODE::LEFT_DOWN;
			if (event.xbutton.button == Button1)
				code = MOUSE_EVENT_CODE::LEFT_DOWN;
			else if (event.xbutton.button == Button2)
				code = MOUSE_EVENT_CODE::MIDDLE_DOWN;
			else if (event.xbutton.button == Button3)
				code = MOUSE_EVENT_CODE::RIGHT_DOWN;
			MouseEvent e(code, event.xbutton.x, event.xbutton.y, 0);
			for (const auto& mouse_handler : _mouse_handlers)
				mouse_handler->sendEvent(e);
			break;
		}
		case ButtonRelease: {
			MOUSE_EVENT_CODE code = MOUSE_EVENT_CODE::LEFT_UP;
			if (event.xbutton.button == Button1)
				code = MOUSE_EVENT_CODE::LEFT_UP;
			else if (event.xbutton.button == Button2)
				code = MOUSE_EVENT_CODE::MIDDLE_UP;
			else if (event.xbutton.button == Button3)
				code = MOUSE_EVENT_CODE::RIGHT_UP;
			MouseEvent e(code, event.xbutton.x, event.xbutton.y, 0);
			for (const auto& mouse_handler : _mouse_handlers)
				mouse_handler->sendEvent(e);
			break;
		}
		case MotionNotify: {
			MouseEvent e(MOUSE_EVENT_CODE::MOVE, event.xmotion.x, event.xmotion.y, 0);
			for (const auto& mouse_handler : _mouse_handlers)
				mouse_handler->sendEvent(e);
			break;
		}
		case KeyPress: {
			unsigned x11_keycode = event.xkey.keycode;
			auto keysym = ::XKeycodeToKeysym(x11Application::_Display, x11_keycode, 1);

			KEY_CODE code = KEY_CODE::UNKNOWN;

			switch (keysym) {
			case XK_A:
				code = KEY_CODE::KEY_A;
				break;
			case XK_D:
				code = KEY_CODE::KEY_D;
				break;
			case XK_S:
				code = KEY_CODE::KEY_S;
				break;
			case XK_W:
				code = KEY_CODE::KEY_W;
				break;
			}

			KeyboardEvent e(code, KEY_EVENT_CODE::DOWN);
			for (const auto& key_handler : _keyboard_handlers)
				key_handler->sendEvent(e);
			break;
		}
		case KeyRelease: {
			unsigned x11_keycode = event.xkey.keycode;
			auto keysym = ::XKeycodeToKeysym(x11Application::_Display, x11_keycode, 1);

			KEY_CODE code = KEY_CODE::UNKNOWN;

			switch (keysym) {
			case XK_A:
				code = KEY_CODE::KEY_A;
			case XK_D:
				code = KEY_CODE::KEY_D;
				break;
			case XK_S:
				code = KEY_CODE::KEY_S;
				break;
			case XK_W:
				code = KEY_CODE::KEY_W;
				break;
			}

			KeyboardEvent e(code, KEY_EVENT_CODE::UP);
			for (const auto& key_handler : _keyboard_handlers)
				key_handler->sendEvent(e);
			break;
		}
		default:
			break;
		}
	}


	int x11Window::getXWindow() const {
		return _xwnd;
	}

	__GLXFBConfigRec* x11Window::getGLXFBConfig() const {
		return _config;
	}

	XVisualInfo* x11Window::getXVisualInfo() const {
		return _visual_info;
	}

	_XDisplay* x11Window::getXDisplay() const {
		return x11Application::_Display;
	}
}

#endif
