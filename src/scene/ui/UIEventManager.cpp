/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "UIEventManager.h"

#include <common/ComponentT.h>
#include <common/Entity.h>
#include <common/Size2D.h>
#include <common/Transform2D.h>

#include <platform/Event.h>
#include <platform/EventHandler.h>
#include <platform/Window.h>

namespace wntr {
    
    std::shared_ptr<UIEventManager> UIEventManager::Create(const std::shared_ptr<Window>& window) {
        return std::make_shared<UIEventManager>(window);
    }
    
    UIEventManager::UIEventManager(const std::shared_ptr<Window>& window)
    : Manager(), _window(window) {
        
        _mouse_handler = MouseHandler::Create();
        assert(_mouse_handler);
        _keyboard_handler = KeyboardHandler::Create();
        assert(_keyboard_handler);
        
        _mouse_handler->onEvent = [this](const MouseEvent& event) {
            processMouseEvent(event);
        };
        _keyboard_handler->onEvent = [this](const KeyboardEvent& event) {
            processKeyboardEvent(event);
        };
        
        auto window_ptr = _window.lock();
        if (window_ptr) {
            window_ptr->registerEventHandler(_mouse_handler);
            window_ptr->registerEventHandler(_keyboard_handler);
        }
    }
    
    UIEventManager::~UIEventManager() {
        auto window_ptr = _window.lock();
        if (window_ptr) {
            window_ptr->unregisterEventHandler(_mouse_handler);
            window_ptr->unregisterEventHandler(_keyboard_handler);
        }
    }
    
    void UIEventManager::dispatchEvents() {
        while (!_message_queue.empty()) {
            auto message = _message_queue.front();
            _message_queue.pop();
            
            assert(message);
            message();
        }
    }
    
    void UIEventManager::runOnEventDispatch (const std::function<void()>& message) {
        assert(message);
        _message_queue.push(message);
    }
    
    void UIEventManager::processMouseEvent (const MouseEvent& event) {
        bool mouse_event_handled = false;
        
        std::function<void(const MouseEvent&, const std::shared_ptr<Entity>&, glm::vec2)>
        process = nullptr; // xcode warning fix
        process =
        [&](const MouseEvent& event, const std::shared_ptr<Entity>& entity, glm::vec2 point) {
            assert(entity);
            
            auto transform = entity->get<Transform2D>();
            if (transform) {
                point -= transform->getPosition();
                point = glm::rotate(point, -transform->getRotation());
                point /= transform->getScale();
            }
            
            auto& children = entity->getChildren();
            for (auto it = children.rbegin(), end = children.rend(); it != end; it++)
                process(event, *it, point);
            
            auto listener = entity->get<MouseListener>();
            auto size = entity->get<Size2D>();
            if (!transform || !listener || !size)
                return;
            
            if (point.x >= 0.0f && point.y >= 0.0f && point.x <= size->getWidth() && point.y <= size->getHeight()) {
                if (!mouse_event_handled) {
                    if (!(*listener)->isMouseOver()) {
                        MouseEvent new_event(MOUSE_EVENT_CODE::MOVE_ON, event.getX(), event.getY(), event.getScrollDelta());
                        std::function<void()> message = [listener, new_event] () {
							(*listener)->sendEvent(new_event);
                        };
                        runOnEventDispatch(message);
                    }
                    std::function<void()> message = [listener, event] () {
						(*listener)->sendEvent(event);
                    };
                    runOnEventDispatch(message);
                    mouse_event_handled = true;
                }
                else {
					if ((*listener)->isMouseOver()) {
                        MouseEvent new_event(MOUSE_EVENT_CODE::MOVE_OFF, event.getX(), event.getY(), event.getScrollDelta());
                        std::function<void()> message = [listener, new_event] () {
							(*listener)->sendEvent(new_event);
                        };
                        runOnEventDispatch(message);
                    }
                }
            }
            else {
				if ((*listener)->isMouseOver()) {
                    MouseEvent new_event(MOUSE_EVENT_CODE::MOVE_OFF, event.getX(), event.getY(), event.getScrollDelta());
                    std::function<void()> message = [listener, new_event] () {
						(*listener)->sendEvent(new_event);
                    };
                    runOnEventDispatch(message);
                }
            }
        };
        
        glm::vec2 point(event.getX(), event.getY());
        for (auto it = _entities.rbegin(), end = _entities.rend(); it != end; it++) {
            auto entity = *it;
            if (!entity->getParent()) {
                process(event, entity, point);
            }
            else {
                auto parent = entity->getParent();
                glm::vec2 local_point = point;
                while (parent) {
                    auto transform = entity->get<Transform2D>();
                    if (transform) {
                        local_point -= transform->getPosition();
                        local_point = glm::rotate(local_point, -transform->getRotation());
                        local_point /= transform->getScale();
                    }
                    parent = parent->getParent();
                }
                process(event, entity, local_point);
            }
        }
    }
    
    void UIEventManager::processKeyboardEvent (const KeyboardEvent& event) {
        for (auto& entity : _entities) {
            assert(entity);
            
            auto listener = entity->get<KeyboardListener>();
            if (listener) {
                std::function<void()> message = [listener, event] () {
					(*listener)->sendEvent(event);
                };
                runOnEventDispatch(message);
            }
        }
    }
}