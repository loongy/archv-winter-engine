/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef _WIN32

#include "win32Window.h"
#include "../Event.h"
#include "../EventHandler.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <windowsx.h>

#include <cassert>
#include <functional>

namespace wntr {

	win32Window::win32Window(const std::string& name, int x, int y, unsigned width, unsigned height, bool fullscreen) 
	: Window(name, x, y, width, height, fullscreen) {

		registerWndClassEx();
		createHWND();

		assert(_hwnd);
		_HWNDMap[_hwnd.get()] = this;
	}

	win32Window::~win32Window() {
		close();
	}

	void win32Window::show() {
		if (_is_showing)
			return;

		assert(_hwnd);
		ShowWindow(_hwnd.get(), SW_SHOW);

		Window::show();
	}

	void win32Window::hide() {
		if (!_is_showing)
			return;

		assert(_hwnd);
		ShowWindow(_hwnd.get(), SW_HIDE);

		Window::hide();
	}

	void win32Window::close() {
		if (_is_closed)
			return;
		Window::close();
		
		assert(_hwnd);
		CloseWindow(_hwnd.get());
		DestroyWindow(_hwnd.get());
		_HWNDMap.erase(_hwnd.get());
		_hwnd.reset();

		WindowEvent evt(WINDOW_EVENT_CODE::CLOSE, getX(), getY(), getWidth(), getHeight());
		for (auto handler : _window_handlers)
			handler->sendEvent(evt);
	}

	void win32Window::setX(int x) {
		assert(_hwnd);
		SetWindowPos(_hwnd.get(), HWND_TOP, x, _y, _width, _height, SWP_ASYNCWINDOWPOS | SWP_NOSIZE);
	}

	void win32Window::setY(int y) {
		assert(_hwnd);
		SetWindowPos(_hwnd.get(), HWND_TOP, _x, y, _width, _height, SWP_ASYNCWINDOWPOS | SWP_NOSIZE);
	}

	void win32Window::setXY(int x, int y) {
		assert(_hwnd);
		SetWindowPos(_hwnd.get(), HWND_TOP, x, y, _width, _height, SWP_ASYNCWINDOWPOS | SWP_NOSIZE);
	}

	void win32Window::setWidth(unsigned width) {
		assert(_hwnd);
		SetWindowPos(_hwnd.get(), HWND_TOP, _x, _y, width, _height, SWP_ASYNCWINDOWPOS | SWP_NOMOVE);
	}

	void win32Window::setHeight(unsigned height) {
		assert(_hwnd);
		SetWindowPos(_hwnd.get(), HWND_TOP, _x, _y, _width, height, SWP_ASYNCWINDOWPOS | SWP_NOMOVE);
	}

	void win32Window::setSize(unsigned width, unsigned height) {
		assert(_hwnd);
		SetWindowPos(_hwnd.get(), HWND_TOP, _x, _y, width, height, SWP_ASYNCWINDOWPOS | SWP_NOMOVE);
	}

	void win32Window::setFullscreen(bool fullscreen) {

	}

	const std::shared_ptr<HWND__>& win32Window::getHWND() const {
		return _hwnd;
	}


	std::unordered_map<HWND, win32Window*> win32Window::_HWNDMap;
	long __stdcall win32Window::WindowProcedure(HWND hwnd, unsigned msg, unsigned wparam, long lparam) {
		auto it = _HWNDMap.find(hwnd);
		if (it == _HWNDMap.end())
			return DefWindowProc(hwnd, msg, wparam, lparam);

		auto window = it->second;
		if (!window)
			return DefWindowProc(hwnd, msg, wparam, lparam);

		switch (msg) {
		
		// closing the window
		case WM_CLOSE:
		case WM_DESTROY:
			window->close();
			break;

		// resizing the window
		case WM_SIZE: 
		{
			unsigned width = (unsigned)LOWORD(lparam);
			unsigned height = (unsigned)HIWORD(lparam);
			switch (wparam) {
			case SIZE_RESTORED:
				if (window->_window_handlers.size() > 0) {
					WindowEvent evt(WINDOW_EVENT_CODE::RESIZE, window->_x, window->_y, width, height);
					for (auto handler : window->_window_handlers)
						handler->sendEvent(evt);
				}
				break;
			case SIZE_MINIMIZED:
				if (window->_window_handlers.size() > 0) {
					WindowEvent evt(WINDOW_EVENT_CODE::MINIMIZE, window->_x, window->_y, width, height);
					for (auto handler : window->_window_handlers)
						handler->sendEvent(evt);
				}
				break;
			case SIZE_MAXIMIZED:
				if (window->_window_handlers.size() > 0) {
					WindowEvent evt(WINDOW_EVENT_CODE::MAXIMIZE, window->_x, window->_y, width, height);
					for (auto handler : window->_window_handlers)
						handler->sendEvent(evt);
				}
				break;
			}
			break;
		}

		case WM_MOVE:
		{
			int x = (int)LOWORD(lparam);
			int y = (int)HIWORD(lparam);
			window->_x = x;
			window->_y = y;

			if (window->_window_handlers.size() > 0) {
				WindowEvent evt(WINDOW_EVENT_CODE::MOVE, x, y, window->_width, window->_height);
				for (auto handler : window->_window_handlers)
					handler->sendEvent(evt);
			}
			break;
		}

		case WM_MOUSEMOVE: 
		{
			int x = (int)GET_X_LPARAM(lparam);
			int y = (int)GET_Y_LPARAM(lparam);

			if (window->_mouse_handlers.size() > 0) {
				MouseEvent evt(MOUSE_EVENT_CODE::MOVE, x, y, 0);
				for (auto handler : window->_mouse_handlers)
					handler->sendEvent(evt);
			}
			break;
		}
		case WM_MOUSEWHEEL: 
		{
			int x = (int)GET_X_LPARAM(lparam);
			int y = (int)GET_Y_LPARAM(lparam);
			int scroll_delta = (int)GET_WHEEL_DELTA_WPARAM(wparam);

			if (window->_mouse_handlers.size() > 0) {
				MouseEvent evt(MOUSE_EVENT_CODE::SCROLL, x, y, scroll_delta);
				for (auto handler : window->_mouse_handlers)
					handler->sendEvent(evt);
			}
			break;
		}
		case WM_LBUTTONDOWN: 
		{
			int x = (int)GET_X_LPARAM(lparam);
			int y = (int)GET_Y_LPARAM(lparam);

			if (window->_mouse_handlers.size() > 0) {
				MouseEvent evt(MOUSE_EVENT_CODE::LEFT_DOWN, x, y, 0);
				for (auto handler : window->_mouse_handlers)
					handler->sendEvent(evt);
			}
			break;
		}
		case WM_LBUTTONUP: 
		{
			int x = (int)GET_X_LPARAM(lparam);
			int y = (int)GET_Y_LPARAM(lparam);

			if (window->_mouse_handlers.size() > 0) {
				MouseEvent evt(MOUSE_EVENT_CODE::LEFT_UP, x, y, 0);
				for (auto handler : window->_mouse_handlers)
					handler->sendEvent(evt);
			}
			break;
		}
		case WM_RBUTTONDOWN:
		{
			int x = (int)GET_X_LPARAM(lparam);
			int y = (int)GET_Y_LPARAM(lparam);

			if (window->_mouse_handlers.size() > 0) {
				MouseEvent evt(MOUSE_EVENT_CODE::RIGHT_DOWN, x, y, 0);
				for (auto handler : window->_mouse_handlers)
					handler->sendEvent(evt);
			}
			break;
		}
		case WM_RBUTTONUP:
		{
			int x = (int)GET_X_LPARAM(lparam);
			int y = (int)GET_Y_LPARAM(lparam);

			if (window->_mouse_handlers.size() > 0) {
				MouseEvent evt(MOUSE_EVENT_CODE::RIGHT_UP, x, y, 0);
				for (auto handler : window->_mouse_handlers)
					handler->sendEvent(evt);
			}
			break;
		}
		case WM_MBUTTONDOWN: 
		{
			int x = (int)GET_X_LPARAM(lparam);
			int y = (int)GET_Y_LPARAM(lparam);

			if (window->_mouse_handlers.size() > 0) {
				MouseEvent evt(MOUSE_EVENT_CODE::MIDDLE_DOWN, x, y, 0);
				for (auto handler : window->_mouse_handlers)
					handler->sendEvent(evt);
			}
			break;
		}
		case WM_MBUTTONUP: 
		{
			int x = (int)GET_X_LPARAM(lparam);
			int y = (int)GET_Y_LPARAM(lparam);

			if (window->_mouse_handlers.size() > 0) {
				MouseEvent evt(MOUSE_EVENT_CODE::MIDDLE_UP, x, y, 0);
				for (auto handler : window->_mouse_handlers)
					handler->sendEvent(evt);
			}
			break;
		}

		case WM_KEYDOWN:
			if (window->_keyboard_handlers.size() > 0) {
				KeyboardEvent evt((KEY_CODE)GetKeyCodeFromWParam(wparam), KEY_EVENT_CODE::DOWN);
				for (auto handler : window->_keyboard_handlers)
					handler->sendEvent(evt);
			}
			break;
		case WM_KEYUP:
			if (window->_keyboard_handlers.size() > 0) {
				KeyboardEvent evt((KEY_CODE)GetKeyCodeFromWParam(wparam), KEY_EVENT_CODE::UP);
				for (auto handler : window->_keyboard_handlers)
					handler->sendEvent(evt);
			}
			break;
		}

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	 unsigned win32Window::GetKeyCodeFromWParam(unsigned wparam) {
		 // handle the alphabet
		 if (wparam >= 0x41 && wparam <= 0x5A)
			 return ((unsigned)KEY_CODE::KEY_A + (wparam - 0x41));
		 // handle numbers
		 if (wparam >= 0x30 && wparam <= 0x39)
			 return ((unsigned)KEY_CODE::KEY_0 + (wparam - 0x30));
		 // handle all other keys
		 switch (wparam) {
		 case VK_LWIN:
			 return (unsigned)KEY_CODE::SUPER;
		 case VK_RWIN:
			 return (unsigned)KEY_CODE::SUPER;
		 case VK_ESCAPE:
			 return (unsigned)KEY_CODE::ESCAPE;
		 case VK_TAB:
			 return (unsigned)KEY_CODE::TAB;
		 case VK_CAPITAL:
			 return (unsigned)KEY_CODE::CAPS_LOCK;
		 case VK_SHIFT:
			 return (unsigned)KEY_CODE::SHIFT;
		 case VK_CONTROL:
			 return (unsigned)KEY_CODE::CONTROL;
		 case VK_MENU:
			 return (unsigned)KEY_CODE::ALT;
		 case VK_SPACE:
			 return (unsigned)KEY_CODE::SPACE;
		 case VK_RETURN:
			 return (unsigned)KEY_CODE::ENTER;
		 case VK_BACK:
			 return (unsigned)KEY_CODE::BACKSPACE;
		 case VK_INSERT:
			 return (unsigned)KEY_CODE::INSERT;
		 case VK_DELETE:
			 return (unsigned)KEY_CODE::DEL;
		 case VK_HOME:
			 return (unsigned)KEY_CODE::HOME;
		 case VK_END:
			 return (unsigned)KEY_CODE::END;
		 case VK_PRIOR:
			 return (unsigned)KEY_CODE::PAGE_UP;
		 case VK_NEXT:
			 return (unsigned)KEY_CODE::PAGE_DOWN;
		 case VK_LEFT:
			 return (unsigned)KEY_CODE::LEFT;
		 case VK_RIGHT:
			 return (unsigned)KEY_CODE::RIGHT;
		 case VK_UP:
			 return (unsigned)KEY_CODE::UP;
		 case VK_DOWN:
			 return (unsigned)KEY_CODE::DOWN;
		 case VK_F1:
			 return (unsigned)KEY_CODE::F1;
		 case VK_F2:
			 return (unsigned)KEY_CODE::F2;
		 case VK_F3:
			 return (unsigned)KEY_CODE::F3;
		 case VK_F4:
			 return (unsigned)KEY_CODE::F4;
		 case VK_F5:
			 return (unsigned)KEY_CODE::F5;
		 case VK_F6:
			 return (unsigned)KEY_CODE::F6;
		 case VK_F7:
			 return (unsigned)KEY_CODE::F7;
		 case VK_F8:
			 return (unsigned)KEY_CODE::F8;
		 case VK_F9:
			 return (unsigned)KEY_CODE::F9;
		 case VK_F10:
			 return (unsigned)KEY_CODE::F10;
		 case VK_F11:
			 return (unsigned)KEY_CODE::F11;
		 case VK_F12:
			 return (unsigned)KEY_CODE::F12;
		 default:
			 return (unsigned)KEY_CODE::UNKNOWN;
		 }
	}

	void win32Window::registerWndClassEx() {
		// randomize the window class name
		_hwnd_name = _name + "_WNDCLASSEX_";
		for (int i = 0; i < 64; ++i)
			_hwnd_name += ((char)std::rand() % 26 + 65);

		// setup wndclassex
		WNDCLASSEX wnd_class_ex;
		ZeroMemory(&wnd_class_ex, sizeof(WNDCLASSEX));
		wnd_class_ex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wnd_class_ex.lpfnWndProc = WindowProcedure;
		wnd_class_ex.cbClsExtra = 0;
		wnd_class_ex.cbWndExtra = 0;
		wnd_class_ex.hInstance = GetModuleHandle(nullptr);
		wnd_class_ex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
		wnd_class_ex.hIconSm = wnd_class_ex.hIcon;
		wnd_class_ex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wnd_class_ex.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wnd_class_ex.lpszMenuName = nullptr;
		wnd_class_ex.lpszClassName = _hwnd_name.c_str();
		wnd_class_ex.cbSize = sizeof(WNDCLASSEX);

		// register wndclassex
		if (!RegisterClassEx(&wnd_class_ex))
			throw std::runtime_error("error: win32Window::win32Window: could not register WNDCLASSEX: " + _name);
	}

	void win32Window::createHWND() {
		// setup window style
		unsigned long style = WS_OVERLAPPEDWINDOW;
		unsigned long style_ex = WS_EX_APPWINDOW;
		if (_fullscreen) {
			style = WS_POPUP | WS_VISIBLE;
			_x = 0;
			_y = 0;
			_width = (unsigned)GetSystemMetrics(SM_CXSCREEN);
			_height = (unsigned)GetSystemMetrics(SM_CYSCREEN);
		}

		// adjust the size of the window
		RECT wnd_size = { (long)_x, (long)_y, (long)_width + (long)_x, (long)_height + (long)_y, };
		if (!AdjustWindowRectEx(&wnd_size, style, false, style_ex))
			throw std::runtime_error("error: win32Window::win32Window: could not adjust RECT: " + _name);

		// create window
		_hwnd.reset(CreateWindowEx(style_ex,
			_hwnd_name.c_str(), _name.c_str(),
			style,
			_x, _y,
			wnd_size.right - wnd_size.left, wnd_size.bottom - wnd_size.top,
			nullptr, nullptr, nullptr, nullptr),
			DestroyWindow);
		if (!_hwnd)
			throw std::runtime_error("error: win32Window::win32Window: could not create HWND: " + _name);
	}
    
}

#endif