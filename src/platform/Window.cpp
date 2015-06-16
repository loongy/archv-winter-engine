/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Window.h"

#ifdef _WIN32
#include "impl-win32/win32Window.h"
#elif __APPLE__
#include "impl-osx/osxWindow.h"
#elif __linux
#include "impl-x11/x11Window.h"
#endif

#include <cassert>

namespace wntr {

	std::shared_ptr<Window> Window::Create(const std::string& name, int x, int y, unsigned width, unsigned height, bool fullscreen) {
#ifdef _WIN32
		return std::make_shared<win32Window>(name, x, y, width, height, fullscreen);
#elif __APPLE__
		return std::make_shared<osxWindow>(name, x, y, width, height, fullscreen);
#elif __linux
		return std::make_shared<x11Window>(name, x, y, width, height, fullscreen);
#endif
	}

	Window::Window(const std::string& name, int x, int y, unsigned width, unsigned height, bool fullscreen) 
	: _name(name), _x(x), _y(y), _width(width), _height(height), _fullscreen(fullscreen), _is_showing(false), _is_closed(false) {

	}

	void Window::show() {
		_is_showing = true;
	}

	void Window::hide() {
		_is_showing = false;
	}

	void Window::close() {
		_is_closed = true;
	}

	void Window::setX(int x) {
		_x = x;
	}

	void Window::setY(int y) {
		_y = y;
	}

	void Window::setXY(int x, int y) {
		_x = x;
		_y = y;
	}

	void Window::setWidth(unsigned width) {
		_width = width;
	}

	void Window::setHeight(unsigned height) {
		_height = height;
	}

	void Window::setSize(unsigned width, unsigned height) {
		_width = width;
		_height = height;
	}

	void Window::setFullscreen(bool fullscreen) {
		_fullscreen = fullscreen;
	}
    
    void Window::registerEventHandler(const std::shared_ptr<EventHandler<KeyboardEvent>>& handler) {
        assert(handler);
		_keyboard_handlers.add(handler);
    }
    
    void Window::registerEventHandler(const std::shared_ptr<EventHandler<MouseEvent>>& handler) {
        assert(handler);
		_mouse_handlers.add(handler);
    }
    
    void Window::registerEventHandler(const std::shared_ptr<EventHandler<WindowEvent>>& handler) {
        assert(handler);
		_window_handlers.add(handler);
    }
    
    void Window::unregisterEventHandler(const std::shared_ptr<EventHandler<KeyboardEvent>>& handler) {
        assert(handler);
		_keyboard_handlers.remove(handler);
    }
    
    void Window::unregisterEventHandler(const std::shared_ptr<EventHandler<MouseEvent>>& handler) {
        assert(handler);
		_mouse_handlers.remove(handler);
    }
    
    void Window::unregisterEventHandler(const std::shared_ptr<EventHandler<WindowEvent>>& handler) {
        assert(handler);
		_window_handlers.remove(handler);
    }

	const std::string& Window::getName() const {
		return _name;
	}

	int Window::getX() const {
		return _x;
	}

	int Window::getY() const {
		return _y;
	}

	unsigned Window::getWidth() const {
		return _width;
	}

	unsigned Window::getHeight() const {
		return _height;
	}
    
    float Window::getAspectRatio() const {
        return (float)getWidth()/(float)getHeight();
    }

	bool Window::isFullscreen() const {
		return _fullscreen;
	}

	bool Window::isShowing() const {
		return _is_showing;
	}

	bool Window::isHiding() const {
		return !_is_showing;
	}

	bool Window::isOpen() const {
		return !_is_closed;
	}

	bool Window::isClosed() const {
		return _is_closed;
	}

};

