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
#ifndef _WNTR_OSXCOCOAAPPLICATION_H_
#define _WNTR_OSXCOCOAAPPLICATION_H_

namespace wntr {
    
    class osxCocoaApplication {
    public:
		osxCocoaApplication(osxCocoaApplication&&) = default;
		osxCocoaApplication& operator=(osxCocoaApplication&&) = default;

		osxCocoaApplication(const osxCocoaApplication&) = delete;
		osxCocoaApplication& operator=(const osxCocoaApplication&) = delete;
        
		explicit osxCocoaApplication();
		virtual ~osxCocoaApplication();
        
        void update();
        void kill();
        
        void* self();
        
    protected:
        
    private:
        void* _nsapplication;
        
    };
    
}

#endif
#endif
