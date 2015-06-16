/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Event.h"

namespace wntr {

	KeyboardEvent::KeyboardEvent(KEY_CODE key_code, KEY_EVENT_CODE event_code) 
	: _key_code(key_code), _event_code(event_code) {
	}

	KEY_CODE KeyboardEvent::getKeyCode() const {
		return _key_code;
	}

	KEY_EVENT_CODE KeyboardEvent::getEventCode() const {
		return _event_code;
	}

	MouseEvent::MouseEvent(MOUSE_EVENT_CODE event_code, int x, int y, int scroll_delta) 
	: _event_code(event_code), _x(x), _y(y), _scroll_delta(scroll_delta) {
	}

	MOUSE_EVENT_CODE MouseEvent::getEventCode() const {
		return _event_code;
	}

	int MouseEvent::getX() const {
		return _x;
	}

	int MouseEvent::getY() const {
		return _y;
	}

	int MouseEvent::getScrollDelta() const {
		return _scroll_delta;
	}
    
    WindowEvent::WindowEvent(WINDOW_EVENT_CODE event_code, int x, int y, unsigned width, unsigned height)
    : _event_code(event_code), _x(x), _y(y), _width(width), _height(height) {
    }
    
    WINDOW_EVENT_CODE WindowEvent::getEventCode() const {
        return _event_code;
    }
    
    int WindowEvent::getX() const {
        return _x;
    }
    
    int WindowEvent::getY() const {
        return _y;
    }
    
    unsigned WindowEvent::getWidth() const {
        return _width;
    }
    
    unsigned WindowEvent::getHeight() const {
        return _height;
    }

}