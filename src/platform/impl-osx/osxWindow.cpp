/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef __APPLE__

#include "osxCocoaWindow.h"
#include "osxWindow.h"
#include "../Event.h"
#include "../EventHandler.h"

#include <cassert>
#include <iostream>

namespace wntr {
    
    osxWindow::osxWindow(const std::string& name, int x, int y, unsigned width, unsigned height, bool fullscreen)
    : Window(name, x, y, width, height, fullscreen) {
        _window = new osxCocoaWindow(name, x, y, width, height, fullscreen);
        _window->windowWillClose = [this](void) {
            _is_closed = true;
            close();
        };
        _window->onMouseEvent = [this](const MouseEvent& evt) {
            for (auto it = _mouse_handlers.begin(), end = _mouse_handlers.end(); it != end; it++)
                (*it)->sendEvent(evt);
        };
        _window->onKeyboardEvent = [this](const KeyboardEvent& evt) {
            for (auto it = _keyboard_handlers.begin(), end = _keyboard_handlers.end(); it != end; it++)
                (*it)->sendEvent(evt);
        };
        _window->onWindowEvent = [this](const WindowEvent& evt) {
            for (auto it = _window_handlers.begin(), end = _window_handlers.end(); it != end; it++)
                (*it)->sendEvent(evt);
        };
    }
    
    osxWindow::~osxWindow() {
        close();
        delete _window;
        _window = nullptr;
    }
    
    void osxWindow::show() {
        assert(_window);
        _window->show();
        
        Window::show();
    }
    
    void osxWindow::hide() {
        assert(_window);
        _window->hide();
        
        Window::hide();
    }
    
    void osxWindow::close() {
        if (_is_closed)
            return;
        
        assert(_window);
        _window->close();
        
        Window::close();
    }
    
    void osxWindow::setX(int x) {
        
    }
    
    void osxWindow::setY(int y) {
        
    }
    
    void osxWindow::setXY(int x, int y) {
        
    }
    
    void osxWindow::setWidth(unsigned width) {
        
    }
    
    void osxWindow::setHeight(unsigned height) {
        
    }
    
    void osxWindow::setSize(unsigned width, unsigned height) {
        
    }
    
    void osxWindow::setFullscreen(bool fullscreen) {
        
    }
    
    void* osxWindow::getNSWindow() const {
        return _window->self();
    }
    
}

#endif