/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef __APPLE__

#ifndef _WNTR_OSXCOCOAWINDOW_H_
#define _WNTR_OSXCOCOAWINDOW_H_

#include <functional>
#include <string>

namespace wntr {
    
    class KeyboardEvent;
    class MouseEvent;
    class WindowEvent;
    
    class osxCocoaWindow {
    public:
        osxCocoaWindow(osxCocoaWindow&&) = default;
		osxCocoaWindow& operator=(osxCocoaWindow&&) = default;
        
		osxCocoaWindow(const osxCocoaWindow&) = delete;
		osxCocoaWindow& operator=(const osxCocoaWindow&) = delete;
        
		explicit osxCocoaWindow(const std::string& name, int x, int y, unsigned width, unsigned height, bool fullscreen);
		~osxCocoaWindow();
        
        void show();
		void hide();
		void close();
        
		void setX(int x);
		void setY(int y);
        void setXY(int x, int y);
		void setWidth(unsigned width);
		void setHeight(unsigned height);
        void setSize(unsigned width, unsigned height);
		void setFullscreen(bool fullscreen);
        
        void* self();
        
        std::function<void()> windowWillClose;
        std::function<void(const MouseEvent& evt)> onMouseEvent;
        std::function<void(const KeyboardEvent& evt)> onKeyboardEvent;
        std::function<void(const WindowEvent& evt)> onWindowEvent;
        
    protected:
        
    private:
        void* _nswindow;
        void* _nsdelegate;
        
    };
    
}

#endif
#endif
