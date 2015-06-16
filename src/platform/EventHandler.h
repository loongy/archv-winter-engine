/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_EVENTHANDLER_H_
#define _WNTR_EVENTHANDLER_H_

#include <functional>
#include <memory>

namespace wntr { 

	class Window;

    template<typename T>
	class EventHandler {
	public:
#ifndef _MSC_VER
		EventHandler(EventHandler&&) = default;
		EventHandler& operator=(EventHandler&&) = default;
#endif
		EventHandler(const EventHandler&) = delete;
		EventHandler& operator=(const EventHandler&) = delete;

		explicit EventHandler() = default;
		virtual ~EventHandler() = default;

		virtual void sendEvent(const T& evt) = 0;
        
        std::function<void(const T& evt)> onEvent;

	protected:

	private:

	};

	class KeyboardEvent;
	class KeyboardHandler : public EventHandler<KeyboardEvent> {
	public:
		static std::shared_ptr<KeyboardHandler> Create(const std::shared_ptr<Window>& window=nullptr);

#ifndef _MSC_VER
		KeyboardHandler(KeyboardHandler&&) = default;
		KeyboardHandler& operator=(KeyboardHandler&&) = default;
#endif
		KeyboardHandler(const KeyboardHandler&) = delete;
		KeyboardHandler& operator=(const KeyboardHandler&) = delete;

		explicit KeyboardHandler();
		virtual ~KeyboardHandler() = default;

		virtual void sendEvent(const KeyboardEvent& evt) override;
        
        bool isKeyDown(KEY_CODE code) const;

		std::function<void(const KeyboardEvent& evt)> onEvent;

	protected:

	private:
		bool _key_map[512];

	};

	class MouseEvent;
	class MouseHandler : public EventHandler<MouseEvent> {
	public:
		static std::shared_ptr<MouseHandler> Create(const std::shared_ptr<Window>& window = nullptr);

#ifndef _MSC_VER
		MouseHandler(MouseHandler&&) = default;
		MouseHandler& operator=(MouseHandler&&) = default;
#endif
		MouseHandler(const MouseHandler&) = delete;
		MouseHandler& operator=(const MouseHandler&) = delete;

		explicit MouseHandler();
		virtual ~MouseHandler() = default;

		virtual void sendEvent(const MouseEvent& evt) override;

		virtual void setMouseXY(int x, int y);

		virtual int getMouseX() const;
		virtual int getMouseY() const;

		virtual bool isMouseLeftDown() const;
		virtual bool isMouseRightDown() const;
		virtual bool isMouseMiddleDown() const;
        
        virtual bool isMouseOver() const;

		std::function<void(const MouseEvent& evt)> onEvent;

	protected:

	private:
		int _mouse_x;
		int _mouse_y;
		bool _mouse_left;
		bool _mouse_right;
		bool _mouse_middle;
        bool _mouse_over;

	};

	class WindowEvent;
	class WindowHandler : public EventHandler<WindowEvent> {
	public:
		static std::shared_ptr<WindowHandler> Create(const std::shared_ptr<Window>& window = nullptr);

#ifndef _MSC_VER
		WindowHandler(WindowHandler&&) = default;
		WindowHandler& operator=(WindowHandler&&) = default;
#endif
		WindowHandler(const WindowHandler&) = delete;
		WindowHandler& operator=(const WindowHandler&) = delete;

		explicit WindowHandler();
		virtual ~WindowHandler() = default;

		virtual void sendEvent(const WindowEvent& evt) override;

		std::function<void(const WindowEvent& evt)> onEvent;

	protected:

	private:

	};

}

#endif