#ifdef __APPLE__

#include "openglosxCocoaRenderContext.h"

#include <platform/impl-osx/osxWindow.h>

namespace wntr {
    
    openglosxCocoaRenderContext::openglosxCocoaRenderContext(const std::shared_ptr<osxWindow>& window, unsigned width, unsigned height, bool vsync, bool fullscreen) {
        NSOpenGLPixelFormatAttribute attribs[] = {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFAAccelerated,
            NSOpenGLPFAColorSize, 32,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAStencilSize, 8,
            0,
        };
        NSOpenGLPixelFormat* pixel_format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
        _nsview = (void*)
            CFBridgingRetain([[NSOpenGLView alloc] initWithFrame:NSMakeRect(0.0f, 0.0f, (float)width, (float)height)
                                                     pixelFormat:pixel_format]);
        
        NSWindow* nswindow = (__bridge NSWindow*)window->getNSWindow();
        [nswindow setContentView:(__bridge NSOpenGLView*)(_nsview)];
        
        [[(__bridge NSOpenGLView*)_nsview openGLContext] makeCurrentContext];
    }
    
    openglosxCocoaRenderContext::~openglosxCocoaRenderContext() {
        CFRelease(_nsview);
        _nsview = nullptr;
    }
    
    void openglosxCocoaRenderContext::swapBuffers() {
        [[(__bridge NSOpenGLView*)_nsview openGLContext] flushBuffer];
    }
    
    void openglosxCocoaRenderContext::lock() {
        // TODO: lock the opengl context
        [[(__bridge NSOpenGLView*)_nsview openGLContext] makeCurrentContext];
    }
    
    void openglosxCocoaRenderContext::unlock() {
        // TODO: unlock the opengl context
    }
    
    void openglosxCocoaRenderContext::setVSync(bool vsync) {
        GLint interval = vsync ? 1 : 0;
        [[(__bridge NSOpenGLView*)_nsview openGLContext] setValues:&interval forParameter:NSOpenGLCPSwapInterval];
    }
    
    void* openglosxCocoaRenderContext::self() {
        return _nsview;
    }
    
}

#endif