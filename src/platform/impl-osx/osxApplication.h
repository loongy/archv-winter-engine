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
#ifndef _WNTR_OSXAPPLICATION_H_
#define _WNTR_OSXAPPLICATION_H_

#include "../Application.h"

namespace wntr {
    
    class osxCocoaApplication;
    
    class osxApplication : public Application {
    public:
        osxApplication(osxApplication&&) = default;
		osxApplication& operator=(osxApplication&&) = default;

		osxApplication(const osxApplication&) = delete;
		osxApplication& operator=(const osxApplication&) = delete;
        
		explicit osxApplication();
		~osxApplication();
        
        virtual void update();
		virtual void kill();
        
    protected:
        
    private:
        osxCocoaApplication* _app;
        
    };
    
}

#endif
#endif