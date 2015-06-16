#ifdef __APPLE__
#ifdef __OBJC__

#include "osxCocoaApplication.h"

namespace wntr {
    
    osxCocoaApplication::osxCocoaApplication() {
        _nsapplication = (void*)CFBridgingRetain([NSApplication sharedApplication]);
    }
    
    osxCocoaApplication::~osxCocoaApplication() {
        CFRelease(_nsapplication);
        _nsapplication = nullptr;
    }
    
    void osxCocoaApplication::update() {
        @autoreleasepool {
            NSApplication* app = (__bridge NSApplication*)_nsapplication;
            NSEvent* event = [app nextEventMatchingMask:NSAnyEventMask
                                              untilDate:nil
                                                 inMode:NSDefaultRunLoopMode
                                                dequeue:YES];
            if (event)
                [app sendEvent:event];
            [app updateWindows];
        }
    }
    
    void osxCocoaApplication::kill() {
        @autoreleasepool {
            NSApplication* app = (__bridge NSApplication*)_nsapplication;
            [app terminate:nil];
        }
    }
    
    void* osxCocoaApplication::self() {
        return _nsapplication;
    }
    
}

#endif
#endif