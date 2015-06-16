/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef __APPLE__

#include "osxApplication.h"
#include "osxCocoaApplication.h"

#include <cassert>

namespace wntr {
    
    osxApplication::osxApplication() {
        _app = new osxCocoaApplication();
    }
    
    osxApplication::~osxApplication() {
        delete _app;
        _app = nullptr;
    }
    
    void osxApplication::update() {
        assert(_app);
        _app->update();
    }
    
    void osxApplication::kill() {
        assert(_app);
        _app->kill();
    }
    
}

#endif