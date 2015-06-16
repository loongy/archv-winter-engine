/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_UIEVENTMANAGER_H_
#define _WNTR_UIEVENTMANAGER_H_

#include <common/Manager.h>

#include <functional>
#include <queue>

namespace wntr {
    
    template<typename T>
    class ComponentT;
    class KeyboardEvent;
    class KeyboardHandler;
    class MouseEvent;
    class MouseHandler;
    class Window;
    
    using MouseListener = ComponentT<MouseHandler>;
    using KeyboardListener = ComponentT<KeyboardHandler>;
    
    class UIEventManager : public Manager {
    public:
        static std::shared_ptr<UIEventManager> Create(const std::shared_ptr<Window>& window);
        
#ifndef _MSC_VER
		UIEventManager(UIEventManager&&) = default;
		UIEventManager& operator=(UIEventManager&&) = default;
#endif
		UIEventManager(const UIEventManager&) = delete;
		UIEventManager& operator=(const UIEventManager&) = delete;
        
        explicit UIEventManager(const std::shared_ptr<Window>& window);
		virtual ~UIEventManager();
        
        void dispatchEvents();
        void runOnEventDispatch(const std::function<void()>& message);
        
    protected:
        
    private:
        void processMouseEvent (const MouseEvent& event);
        void processKeyboardEvent (const KeyboardEvent& event);
        
        std::shared_ptr<MouseHandler> _mouse_handler;
        std::shared_ptr<KeyboardHandler> _keyboard_handler;
        std::weak_ptr<Window> _window;
        std::queue<std::function<void()>> _message_queue;
        
    };
    
}

#endif
