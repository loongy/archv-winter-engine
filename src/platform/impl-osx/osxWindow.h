/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef __APPLE__

#pragma once
#ifndef _WNTR_OSXWINDOW_H_
#define _WNTR_OSXWINDOW_H_

#include "Window.h"

namespace wntr {
    
    class osxCocoaWindow;
    
    class osxWindow : public Window {
    public:
        osxWindow(osxWindow&&) = default;
		osxWindow& operator=(osxWindow&&) = default;

		osxWindow(const osxWindow&) = delete;
		osxWindow& operator=(const osxWindow&) = delete;
        
		explicit osxWindow(const std::string& name, int x, int y, unsigned width, unsigned height, bool fullscreen);
		virtual ~osxWindow();
        
		virtual void show() override;
		virtual void hide() override;
		virtual void close() override;
        
		virtual void setX(int x) override;
		virtual void setY(int y) override;
		virtual void setXY(int x, int y) override;
		virtual void setWidth(unsigned width) override;
		virtual void setHeight(unsigned height) override;
		virtual void setSize(unsigned width, unsigned height) override;
		virtual void setFullscreen(bool fullscreen) override;
        
        void* getNSWindow() const;
        
    protected:
        
    private:
        osxCocoaWindow* _window;
        
    };
    
}

#endif
#endif