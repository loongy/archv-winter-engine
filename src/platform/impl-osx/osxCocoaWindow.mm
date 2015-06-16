#ifdef __APPLE__
#ifdef __OBJC__

#include "osxCocoaWindow.h"
#include "../Event.h"

#include <Carbon/Carbon.h>
#include <iostream>

// custom cocoa window so that we can handle events

// osxCocoaWindow interface
@interface osxCocoaWindow : NSWindow {
@public
    wntr::osxCocoaWindow* _wnd;
}

@end

// osxCocoaWindow implementation
@implementation osxCocoaWindow

- (void)mouseDown:(NSEvent*)evt {
    NSPoint point = [evt locationInWindow];
    int height = [[self contentView] bounds].size.height;
    point.y = height - point.y;
    
    wntr::MouseEvent m_evt(wntr::MOUSE_EVENT_CODE::LEFT_DOWN, point.x, point.y, 0);
    if (_wnd)
        if (_wnd->onMouseEvent)
            _wnd->onMouseEvent(m_evt);
}

- (void)mouseUp:(NSEvent*)evt {
    NSPoint point = [evt locationInWindow];
    int height = [[self contentView] bounds].size.height;
    point.y = height - point.y;
    
    wntr::MouseEvent m_evt(wntr::MOUSE_EVENT_CODE::LEFT_UP, point.x, point.y, 0);
    if (_wnd)
        if (_wnd->onMouseEvent)
            _wnd->onMouseEvent(m_evt);
}

- (void)rightMouseDown:(NSEvent*)evt {
    NSPoint point = [evt locationInWindow];
    int height = [[self contentView] bounds].size.height;
    point.y = height - point.y;
    
    wntr::MouseEvent m_evt(wntr::MOUSE_EVENT_CODE::RIGHT_DOWN, point.x, point.y, 0);
    if (_wnd)
        if (_wnd->onMouseEvent)
            _wnd->onMouseEvent(m_evt);
}

- (void)rightMouseUp:(NSEvent*)evt {
    NSPoint point = [evt locationInWindow];
    int height = [[self contentView] bounds].size.height;
    point.y = height - point.y;
    
    wntr::MouseEvent m_evt(wntr::MOUSE_EVENT_CODE::RIGHT_UP, point.x, point.y, 0);
    if (_wnd)
        if (_wnd->onMouseEvent)
            _wnd->onMouseEvent(m_evt);
}

- (void)mouseMoved:(NSEvent*)evt {
    NSPoint point = [evt locationInWindow];
    int height = [[self contentView] bounds].size.height;
    point.y = height - point.y;
    
    wntr::MouseEvent m_evt(wntr::MOUSE_EVENT_CODE::MOVE, point.x, point.y, 0);
    if (_wnd)
        if (_wnd->onMouseEvent)
            _wnd->onMouseEvent(m_evt);
}

- (void)mouseDragged:(NSEvent*)evt {
    NSPoint point = [evt locationInWindow];
    int height = [[self contentView] bounds].size.height;
    point.y = height - point.y;
    
    wntr::MouseEvent m_evt(wntr::MOUSE_EVENT_CODE::MOVE, point.x, point.y, 0);
    if (_wnd)
        if (_wnd->onMouseEvent)
            _wnd->onMouseEvent(m_evt);
}

- (void)scrollWheel:(NSEvent*)evt {
    NSPoint point = [evt locationInWindow];
    int height = [[self contentView] bounds].size.height;
    point.y = height - point.y;
    
    wntr::MouseEvent m_evt(wntr::MOUSE_EVENT_CODE::SCROLL, point.x, point.y, (int)[evt scrollingDeltaY]);
    if (_wnd)
        if (_wnd->onMouseEvent)
            _wnd->onMouseEvent(m_evt);
}

wntr::KEY_CODE convertOSXKeyCode (unsigned short _code);

- (void)keyDown:(NSEvent*)evt {
    unsigned short kc = [evt keyCode];
    wntr::KeyboardEvent k_evt(convertOSXKeyCode(kc), wntr::KEY_EVENT_CODE::DOWN);
    if (_wnd)
        if (_wnd->onKeyboardEvent)
            _wnd->onKeyboardEvent(k_evt);
}

- (void)keyUp:(NSEvent*)evt {
    unsigned short kc = [evt keyCode];
    wntr::KeyboardEvent k_evt(convertOSXKeyCode(kc), wntr::KEY_EVENT_CODE::UP);
    if (_wnd)
        if (_wnd->onKeyboardEvent)
            _wnd->onKeyboardEvent(k_evt);
}

- (void)flagsChanged:(NSEvent*)evt {
    unsigned short osx_key_code = [evt keyCode];
    wntr::KEY_CODE key_code = convertOSXKeyCode(osx_key_code);
    wntr::KEY_EVENT_CODE event_code = wntr::KEY_EVENT_CODE::DOWN;
    
    if (key_code == wntr::KEY_CODE::SHIFT && ([evt modifierFlags] & NSShiftKeyMask) == 0)
        event_code = wntr::KEY_EVENT_CODE::UP;
    else if (key_code == wntr::KEY_CODE::CONTROL && ([evt modifierFlags] & NSControlKeyMask) == 0)
        event_code = wntr::KEY_EVENT_CODE::UP;
    else if (key_code == wntr::KEY_CODE::SUPER && ([evt modifierFlags] & NSCommandKeyMask) == 0)
        event_code = wntr::KEY_EVENT_CODE::UP;
    else if (key_code == wntr::KEY_CODE::ALT && ([evt modifierFlags] & NSAlternateKeyMask) == 0)
        event_code = wntr::KEY_EVENT_CODE::UP;
    else if (key_code == wntr::KEY_CODE::CAPS_LOCK && ([evt modifierFlags] & NSAlphaShiftKeyMask) == 0)
        event_code = wntr::KEY_EVENT_CODE::UP;
    
    wntr::KeyboardEvent k_evt(key_code, event_code);
    if (_wnd)
        if (_wnd->onKeyboardEvent)
            _wnd->onKeyboardEvent(k_evt);
}

wntr::KEY_CODE convertOSXKeyCode (unsigned short code) {
    switch (code) {
        case kVK_ANSI_A:
            return wntr::KEY_CODE::KEY_A;
        case kVK_ANSI_S:
            return wntr::KEY_CODE::KEY_S;
        case kVK_ANSI_D:
            return wntr::KEY_CODE::KEY_D;
        case kVK_ANSI_F:
            return wntr::KEY_CODE::KEY_F;
        case kVK_ANSI_H:
            return wntr::KEY_CODE::KEY_H;
        case kVK_ANSI_G:
            return wntr::KEY_CODE::KEY_G;
        case kVK_ANSI_Z:
            return wntr::KEY_CODE::KEY_Z;
        case kVK_ANSI_X:
            return wntr::KEY_CODE::KEY_X;
        case kVK_ANSI_C:
            return wntr::KEY_CODE::KEY_C;
        case kVK_ANSI_V:
            return wntr::KEY_CODE::KEY_V;
        case kVK_ANSI_B:
            return wntr::KEY_CODE::KEY_B;
        case kVK_ANSI_Q:
            return wntr::KEY_CODE::KEY_Q;
        case kVK_ANSI_W:
            return wntr::KEY_CODE::KEY_W;
        case kVK_ANSI_E:
            return wntr::KEY_CODE::KEY_E;
        case kVK_ANSI_R:
            return wntr::KEY_CODE::KEY_R;
        case kVK_ANSI_Y:
            return wntr::KEY_CODE::KEY_Y;
        case kVK_ANSI_T:
            return wntr::KEY_CODE::KEY_T;
        case kVK_ANSI_1:
            return wntr::KEY_CODE::KEY_1;
        case kVK_ANSI_2:
            return wntr::KEY_CODE::KEY_2;
        case kVK_ANSI_3:
            return wntr::KEY_CODE::KEY_3;
        case kVK_ANSI_4:
            return wntr::KEY_CODE::KEY_4;
        case kVK_ANSI_6:
            return wntr::KEY_CODE::KEY_6;
        case kVK_ANSI_5:
            return wntr::KEY_CODE::KEY_5;
        case kVK_ANSI_9:
            return wntr::KEY_CODE::KEY_9;
        case kVK_ANSI_7:
            return wntr::KEY_CODE::KEY_7;
        case kVK_ANSI_8:
            return wntr::KEY_CODE::KEY_8;
        case kVK_ANSI_0:
            return wntr::KEY_CODE::KEY_0;
        case kVK_ANSI_O:
            return wntr::KEY_CODE::KEY_O;
        case kVK_ANSI_U:
            return wntr::KEY_CODE::KEY_U;
        case kVK_ANSI_I:
            return wntr::KEY_CODE::KEY_I;
        case kVK_ANSI_P:
            return wntr::KEY_CODE::KEY_P;
        case kVK_ANSI_L:
            return wntr::KEY_CODE::KEY_L;
        case kVK_ANSI_J:
            return wntr::KEY_CODE::KEY_J;
        case kVK_ANSI_K:
            return wntr::KEY_CODE::KEY_K;
        case kVK_ANSI_N:
            return wntr::KEY_CODE::KEY_N;
        case kVK_ANSI_M:
            return wntr::KEY_CODE::KEY_M;
        case kVK_Return:
            return wntr::KEY_CODE::ENTER;
        case kVK_Tab:
            return wntr::KEY_CODE::TAB;
        case kVK_Space:
            return wntr::KEY_CODE::SPACE;
        case kVK_Delete:
            return wntr::KEY_CODE::BACKSPACE;
        case kVK_Escape:
            return wntr::KEY_CODE::ESCAPE;
        case kVK_Command:
            return wntr::KEY_CODE::SUPER;
        case kVK_Shift:
            return wntr::KEY_CODE::SHIFT;
        case kVK_CapsLock:
            return wntr::KEY_CODE::CAPS_LOCK;
        case kVK_Option:
            return wntr::KEY_CODE::ALT;
        case kVK_Control:
            return wntr::KEY_CODE::CONTROL;
        case kVK_RightShift:
            return wntr::KEY_CODE::SHIFT;
        case kVK_RightOption:
            return wntr::KEY_CODE::ALT;
        case kVK_RightControl:
            return wntr::KEY_CODE::CONTROL;
        case kVK_F5:
            return wntr::KEY_CODE::F5;
        case kVK_F6:
            return wntr::KEY_CODE::F6;
        case kVK_F7:
            return wntr::KEY_CODE::F7;
        case kVK_F3:
            return wntr::KEY_CODE::F3;
        case kVK_F8:
            return wntr::KEY_CODE::F8;
        case kVK_F9:
            return wntr::KEY_CODE::F9;
        case kVK_F11:
            return wntr::KEY_CODE::F11;
        case kVK_F10:
            return wntr::KEY_CODE::F10;
        case kVK_F12:
            return wntr::KEY_CODE::F12;
        case kVK_Home:
            return wntr::KEY_CODE::HOME;
        case kVK_PageUp:
            return wntr::KEY_CODE::PAGE_UP;
        case kVK_ForwardDelete:
            return wntr::KEY_CODE::DEL;
        case kVK_F4:
            return wntr::KEY_CODE::F4;
        case kVK_End:
            return wntr::KEY_CODE::END;
        case kVK_F2:
            return wntr::KEY_CODE::F2;
        case kVK_PageDown:
            return wntr::KEY_CODE::PAGE_DOWN;
        case kVK_F1:
            return wntr::KEY_CODE::F1;
        case kVK_LeftArrow:
            return wntr::KEY_CODE::LEFT;
        case kVK_RightArrow:
            return wntr::KEY_CODE::RIGHT;
        case kVK_DownArrow:
            return wntr::KEY_CODE::DOWN;
        case kVK_UpArrow:
            return wntr::KEY_CODE::UP;
    }    
    return wntr::KEY_CODE::UNKNOWN;
}

@end

// custom cocoa delegate so that we can handle closing correctly

// osxCocoaDelegate interface
@interface osxCocoaDelegate : NSObject<NSWindowDelegate> {
@public
    wntr::osxCocoaWindow* _wnd;
}
- (id)initFromWindow:(wntr::osxCocoaWindow*)wnd;

@end

// osxCocoaDelegate implementation
@implementation osxCocoaDelegate

- (id)initFromWindow:(wntr::osxCocoaWindow*)wnd {
    self = [super init];
    if (self)
        _wnd = wnd;
    return (self);
}

- (void)windowDidMove:(NSNotification*)notification {
    NSWindow* nswindow = (__bridge ::osxCocoaWindow*)_wnd->self();
    
    NSRect main_screen_frame = [[NSScreen mainScreen] frame];
    NSRect frame = [nswindow frame];
    
    unsigned display_height = (unsigned)main_screen_frame.size.height;
    int inverse_y = display_height-(frame.origin.y+frame.size.height);
    
    wntr::WindowEvent evt(wntr::WINDOW_EVENT_CODE::MOVE, frame.origin.x, inverse_y, frame.size.width, frame.size.height);
    if (_wnd)
        if (_wnd->onWindowEvent)
            _wnd->onWindowEvent(evt);
}

- (void)windowDidResize:(NSNotification*)notification {
    NSWindow* nswindow = (__bridge ::osxCocoaWindow*)_wnd->self();
    
    NSRect main_screen_frame = [[NSScreen mainScreen] frame];
    NSRect frame = [nswindow frame];
    
    unsigned display_height = (unsigned)main_screen_frame.size.height;
    int inverse_y = display_height-(frame.origin.y+frame.size.height);
    
    wntr::WindowEvent evt(wntr::WINDOW_EVENT_CODE::RESIZE, frame.origin.x, inverse_y, frame.size.width, frame.size.height);
    if (_wnd)
        if (_wnd->onWindowEvent)
            _wnd->onWindowEvent(evt);
}

- (void)windowDidMiniaturize:(NSNotification*)notification {
    NSWindow* nswindow = (__bridge ::osxCocoaWindow*)_wnd->self();
    
    NSRect main_screen_frame = [[NSScreen mainScreen] frame];
    NSRect frame = [nswindow frame];
    
    unsigned display_height = (unsigned)main_screen_frame.size.height;
    int inverse_y = display_height-(frame.origin.y+frame.size.height);
    
    wntr::WindowEvent evt(wntr::WINDOW_EVENT_CODE::MINIMIZE, frame.origin.x, inverse_y, frame.size.width, frame.size.height);
    if (_wnd)
        if (_wnd->onWindowEvent)
            _wnd->onWindowEvent(evt);
}

- (void)windowDidDeminiaturize:(NSNotification*)notification {
    NSWindow* nswindow = (__bridge ::osxCocoaWindow*)_wnd->self();
    
    NSRect main_screen_frame = [[NSScreen mainScreen] frame];
    NSRect frame = [nswindow frame];
    
    unsigned display_height = (unsigned)main_screen_frame.size.height;
    int inverse_y = display_height-(frame.origin.y+frame.size.height);
    
    wntr::WindowEvent evt(wntr::WINDOW_EVENT_CODE::MAXIMIZE, frame.origin.x, inverse_y, frame.size.width, frame.size.height);
    if (_wnd)
        if (_wnd->onWindowEvent)
            _wnd->onWindowEvent(evt);
}

- (void)windowWillClose:(NSNotification*)notification {
    if (_wnd)
        if (_wnd->windowWillClose)
            _wnd->windowWillClose();
}

@end

namespace wntr {
    
    osxCocoaWindow::osxCocoaWindow(const std::string& name, int x, int y, unsigned width, unsigned height, bool fullscreen) {
        NSRect main_screen_frame = [[NSScreen mainScreen] frame];
        unsigned display_height = (unsigned)main_screen_frame.size.height;
        int inverse_y = display_height-(y+height);
        
        _nswindow = (void*)
        CFBridgingRetain([[::osxCocoaWindow alloc] initWithContentRect:NSMakeRect(x, inverse_y, width, height)
                                                             styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask
                                                               backing:NSBackingStoreBuffered
                                                                 defer:NO]);
        _nsdelegate = (void*)CFBridgingRetain([[osxCocoaDelegate alloc] initFromWindow:this]);
        
        ::osxCocoaWindow* wnd = (__bridge ::osxCocoaWindow*)_nswindow;
        wnd->_wnd = this;
        
        [wnd setAcceptsMouseMovedEvents:YES];
        [wnd setReleasedWhenClosed:NO];
        [wnd setDelegate:(__bridge osxCocoaDelegate*)_nsdelegate];
        [wnd setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
        [wnd setTitle:[NSString stringWithCString:name.c_str()
                                         encoding:[NSString defaultCStringEncoding]]];
        if (fullscreen)
            [wnd toggleFullScreen:nil];
        
        windowWillClose = nullptr;
        onMouseEvent = nullptr;
        onKeyboardEvent = nullptr;
        onWindowEvent = nullptr;
    }
    
    osxCocoaWindow::~osxCocoaWindow() {
        CFRelease(_nswindow);
        CFRelease(_nsdelegate);
        _nswindow = nullptr;
        _nsdelegate = nullptr;
    }
    
    void osxCocoaWindow::show() {
        NSWindow* wnd =(__bridge NSWindow*)_nswindow;
        [wnd orderFront:wnd];
    }
    
    void osxCocoaWindow::hide() {
        NSWindow* wnd =(__bridge NSWindow*)_nswindow;
        [wnd orderOut:wnd];
    }
    
    void osxCocoaWindow::close() {
        NSWindow* wnd =(__bridge NSWindow*)_nswindow;
        [wnd close];
    }
    
    void osxCocoaWindow::setX(int x) {
        
    }
    
    void osxCocoaWindow::setY(int y) {
        
    }
    
    void osxCocoaWindow::setXY(int x, int y) {
        
    }
    
    void osxCocoaWindow::setWidth(unsigned width) {
        
    }
    
    void osxCocoaWindow::setHeight(unsigned height) {
        
    }
    
    void osxCocoaWindow::setSize(unsigned width, unsigned height) {
        
    }
    
    void osxCocoaWindow::setFullscreen(bool fullscreen) {
        
    }
    
    void* osxCocoaWindow::self() {
        return _nswindow;
    }
    
}

#endif
#endif