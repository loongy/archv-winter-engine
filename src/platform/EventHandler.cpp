/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Event.h"
#include "EventHandler.h"
#include "Window.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#endif

namespace wntr {

	std::shared_ptr<KeyboardHandler> KeyboardHandler::Create(const std::shared_ptr<Window>& window) {
		auto handler = std::make_shared<KeyboardHandler>();
		if (window)
			window->registerEventHandler(handler); 
		return handler;
	}
    
	KeyboardHandler::KeyboardHandler()
	: EventHandler<KeyboardEvent>() {
		for (unsigned i = 0; i < 512; i++)
			_key_map[i] = false;
	}

	void KeyboardHandler::sendEvent(const KeyboardEvent& evt) {
		if (evt.getEventCode() == KEY_EVENT_CODE::DOWN)
			_key_map[(unsigned)evt.getKeyCode()] = true;
		else if (evt.getEventCode() == KEY_EVENT_CODE::UP)
			_key_map[(unsigned)evt.getKeyCode()] = false;

		if (onEvent)
			onEvent(evt);
	}
    
    bool KeyboardHandler::isKeyDown(KEY_CODE code) const {
        return _key_map[(unsigned)code];
    }

	std::shared_ptr<MouseHandler> MouseHandler::Create(const std::shared_ptr<Window>& window) {
		auto handler = std::make_shared<MouseHandler>();
		if (window)
			window->registerEventHandler(handler);
		return handler;
	}

	MouseHandler::MouseHandler()
	: EventHandler<MouseEvent>(), _mouse_left(false), _mouse_right(false), _mouse_middle(false), _mouse_over(false) {
#ifdef _WIN32
		POINT point;
		GetCursorPos(&point);
		_mouse_x = point.x;
		_mouse_y = point.y;
#elif __APPLE__
		CGEventRef evt_ref = CGEventCreate(nullptr);
		CGPoint point = CGEventGetLocation(evt_ref);
		_mouse_x = point.x;
		_mouse_y = point.y;
		CFRelease(evt_ref);
#endif
	}

	void MouseHandler::sendEvent(const MouseEvent& evt) {
#ifdef _WIN32
		POINT point;
		GetCursorPos(&point);
		_mouse_x = point.x;
		_mouse_y = point.y;
#elif __APPLE__
		CGEventRef evt_ref = CGEventCreate(nullptr);
		CGPoint point = CGEventGetLocation(evt_ref);
		_mouse_x = point.x;
		_mouse_y = point.y;
		CFRelease(evt_ref);
#endif
        switch (evt.getEventCode()) {
            case MOUSE_EVENT_CODE::LEFT_DOWN:
                _mouse_left = true;
                break;
            case MOUSE_EVENT_CODE::LEFT_UP:
                _mouse_left = false;
                break;
            case MOUSE_EVENT_CODE::RIGHT_DOWN:
                _mouse_right = true;
                break;
            case MOUSE_EVENT_CODE::RIGHT_UP:
                _mouse_right = false;
                break;
            case MOUSE_EVENT_CODE::MIDDLE_DOWN:
                _mouse_middle = true;
                break;
            case MOUSE_EVENT_CODE::MIDDLE_UP:
                _mouse_middle = false;
                break;
            case MOUSE_EVENT_CODE::MOVE_ON:
                _mouse_over = true;
                break;
            case MOUSE_EVENT_CODE::MOVE_OFF:
                _mouse_over = false;
                break;
            default:
                break;
        }
		if (onEvent)
			onEvent(evt);
	}

	void MouseHandler::setMouseXY(int x, int y) {
#ifdef _WIN32
		SetCursorPos(x, y);
#elif __APPLE__        
        CGPoint point;
        point.x = (float)x;
        point.y = (float)y;
        CGWarpMouseCursorPosition(point);
#endif
		_mouse_x = x;
		_mouse_y = y;
	}

	int MouseHandler::getMouseX() const {
		return _mouse_x;
	}

	int MouseHandler::getMouseY() const {
		return _mouse_y;
	}

	bool MouseHandler::isMouseLeftDown() const {
		return _mouse_left;
	}

	bool MouseHandler::isMouseRightDown() const {
		return _mouse_right;
	}

	bool MouseHandler::isMouseMiddleDown() const {
		return _mouse_middle;
	}
    
    bool MouseHandler::isMouseOver() const {
        return _mouse_over;
    }

	std::shared_ptr<WindowHandler> WindowHandler::Create(const std::shared_ptr<Window>& window) {
		auto handler = std::make_shared<WindowHandler>();
		if (window)
			window->registerEventHandler(handler);
		return handler;
	}

	WindowHandler::WindowHandler()
	: EventHandler<WindowEvent>() {

	}

	void WindowHandler::sendEvent(const WindowEvent& evt) {
		if (onEvent)
			onEvent(evt);
	}
    
}