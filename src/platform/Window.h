/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_WINDOW_H_
#define _WNTR_WINDOW_H_

#include <common/utility.h>

#include <memory>
#include <string>

namespace wntr {

    template<typename T>
	class EventHandler;
    
    class KeyboardEvent;
    class MouseEvent;
    class WindowEvent;

	class Window {
	public:
		static std::shared_ptr<Window> Create(const std::string& name, int x, int y, unsigned width, unsigned height, bool fullscreen = false);

#ifndef _MSC_VER
		Window(Window&&) = default;
		Window& operator=(Window&&) = default;
#endif
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		explicit Window(const std::string& name, int x, int y, unsigned width, unsigned height, bool fullscreen);
		virtual ~Window() = default;

		virtual void show();
		virtual void hide();
		virtual void close();

		virtual void setX(int x);
		virtual void setY(int y);
		virtual void setXY(int x, int y);
		virtual void setWidth(unsigned width);
		virtual void setHeight(unsigned height);
		virtual void setSize(unsigned width, unsigned height);
		virtual void setFullscreen(bool fullscreen);

		void registerEventHandler(const std::shared_ptr<EventHandler<KeyboardEvent>>& handler);
        void registerEventHandler(const std::shared_ptr<EventHandler<MouseEvent>>& handler);
        void registerEventHandler(const std::shared_ptr<EventHandler<WindowEvent>>& handler);
		void unregisterEventHandler(const std::shared_ptr<EventHandler<KeyboardEvent>>& handler);
		void unregisterEventHandler(const std::shared_ptr<EventHandler<MouseEvent>>& handler);
		void unregisterEventHandler(const std::shared_ptr<EventHandler<WindowEvent>>& handler);

		const std::string& getName() const;
		int      getX() const;
		int      getY() const;
		unsigned getWidth() const;
		unsigned getHeight() const;
        float    getAspectRatio() const;
		bool     isFullscreen() const;
		bool     isShowing() const;
		bool     isHiding() const;
		bool     isOpen() const;
		bool     isClosed() const;

	protected:
		std::string _name;
		int _x;
		int _y;
		unsigned _width;
		unsigned _height;
		bool _fullscreen;

		bool _is_showing;
		bool _is_closed;

		shared_vector<EventHandler<KeyboardEvent>> _keyboard_handlers;
        shared_vector<EventHandler<MouseEvent>> _mouse_handlers;
        shared_vector<EventHandler<WindowEvent>> _window_handlers;

	private:

	};

}

#endif