/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_EVENT_H_
#define _WNTR_EVENT_H_

namespace wntr {

	enum class KEY_CODE {
		SUPER,
		KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
		KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		ESCAPE,
		TAB,
		CAPS_LOCK,
		SHIFT,
		CONTROL,
		ALT,
		SPACE,
		ENTER,
		BACKSPACE,
		INSERT,
		DEL,
		HOME,
		END,
		PAGE_UP,
		PAGE_DOWN,
		LEFT, RIGHT, UP, DOWN,
		UNKNOWN,
	};

	enum class KEY_EVENT_CODE {
		UP,
		DOWN,
	};

	enum class MOUSE_EVENT_CODE {
		LEFT_UP,
		LEFT_DOWN,
		RIGHT_UP,
		RIGHT_DOWN,
		MIDDLE_UP,
		MIDDLE_DOWN,
		SCROLL,
		MOVE,
		MOVE_ON,
		MOVE_OFF,
	};

	enum class WINDOW_EVENT_CODE {
        MOVE,
        RESIZE,
        MINIMIZE,
        MAXIMIZE,
        CLOSE,
	};

	class KeyboardEvent {
	public:
#ifndef _MSC_VER
		KeyboardEvent(KeyboardEvent&&) = default;
		KeyboardEvent& operator= (KeyboardEvent&&) = default;
#endif
		KeyboardEvent(const KeyboardEvent&) = default;
		KeyboardEvent& operator= (const KeyboardEvent&) = default;

		explicit KeyboardEvent(KEY_CODE key_code, KEY_EVENT_CODE event_code);
		~KeyboardEvent() = default;

		KEY_CODE       getKeyCode() const;
		KEY_EVENT_CODE getEventCode() const;

	protected:

	private:
		KEY_CODE _key_code;
		KEY_EVENT_CODE _event_code;

	};

	class MouseEvent {
	public:
#ifndef _MSC_VER
		MouseEvent(MouseEvent&&) = default;
		MouseEvent& operator= (MouseEvent&&) = default;
#endif
		MouseEvent(const MouseEvent&) = default;
		MouseEvent& operator= (const MouseEvent&) = default;

		explicit MouseEvent(MOUSE_EVENT_CODE event_code, int x, int y, int scroll_delta);
		~MouseEvent() = default;

		MOUSE_EVENT_CODE getEventCode() const;
		int getX() const;
		int getY() const;
		int getScrollDelta() const;

	protected:

	private:
		MOUSE_EVENT_CODE _event_code;
		int _x;
		int _y;
		int _scroll_delta;

	};

	class WindowEvent {
	public:
#ifndef _MSC_VER
		WindowEvent(WindowEvent&&) = default;
		WindowEvent& operator= (WindowEvent&&) = default;
#endif
		WindowEvent(const WindowEvent&) = default;
		WindowEvent& operator= (const WindowEvent&) = default;
        
        explicit WindowEvent(WINDOW_EVENT_CODE event_code, int x, int y, unsigned width, unsigned height);
		~WindowEvent() = default;
        
        WINDOW_EVENT_CODE getEventCode() const;
        int      getX() const;
        int      getY() const;
        unsigned getWidth() const;
        unsigned getHeight() const;
        
	protected:
        
	private:
        WINDOW_EVENT_CODE _event_code;
        int _x;
        int _y;
        unsigned _width;
        unsigned _height;
        
	};

}

#endif