/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef __APPLE__

#include "openglosxCocoaRenderContext.h"
#include "openglosxRenderContext.h"

namespace wntr {
    
    openglosxRenderContext::openglosxRenderContext(const std::shared_ptr<osxWindow>& window, unsigned width, unsigned height, bool vsync, bool fullscreen)
    : openglRenderContext(width, height, vsync, fullscreen), _window(window) {
        _gl = new openglosxCocoaRenderContext(window, width, height, vsync, fullscreen);
        getOpenGLFunctionPointers();
        setVsync(vsync);
    }
    
    openglosxRenderContext::~openglosxRenderContext() {
        delete _gl;
        _gl = nullptr;
    }
    
    void openglosxRenderContext::swapBuffers() {
        _gl->swapBuffers();
    }
    
    void openglosxRenderContext::lock() {
        _gl->lock();
    }
    
    void openglosxRenderContext::unlock() {
        _gl->unlock();
    }
    
    void openglosxRenderContext::setVsync(bool vsync) {
        _gl->setVSync(vsync);
    }
    
    const std::shared_ptr<osxWindow>& openglosxRenderContext::getWindow() const {
        return _window;
    }
    
    void openglosxRenderContext::getOpenGLFunctionPointers() {
        glActiveTexture = ::glActiveTexture;
        glAttachShader = ::glAttachShader;
        glBindBuffer = ::glBindBuffer;
        glBindBufferBase = ::glBindBufferBase;
        glBindFramebuffer = ::glBindFramebuffer;
        glBindRenderbuffer = ::glBindRenderbuffer;
        glBindSampler = ::glBindSampler;
        glBindVertexArray = ::glBindVertexArray;
        glBlendEquationSeparate = ::glBlendEquationSeparate;
        glBlendEquationSeparatei = ::glBlendEquationSeparatei;
        glBlendFuncSeparate = ::glBlendFuncSeparate;
        glBlendFuncSeparatei = ::glBlendFuncSeparatei;
        glBufferData = ::glBufferData;
        glBufferSubData = ::glBufferSubData;
        glCheckFramebufferStatus = ::glCheckFramebufferStatus;
        glCompileShader = ::glCompileShader;
        glCreateProgram = ::glCreateProgram;
        glCreateShader = ::glCreateShader;
        glDisablei = ::glDisablei;
        glDisableVertexAttribArray = ::glDisableVertexAttribArray;
        glDrawArraysInstanced = ::glDrawArraysInstanced;
        glDrawBuffers = ::glDrawBuffers;
        glDrawElementsInstanced = ::glDrawElementsInstanced;
        glEnablei = ::glEnablei;
        glEnableVertexAttribArray = ::glEnableVertexAttribArray;
        glFramebufferRenderbuffer = ::glFramebufferRenderbuffer;
        glFramebufferTexture2D = ::glFramebufferTexture2D;
        glFramebufferTexture = ::glFramebufferTexture;
        glGenBuffers = ::glGenBuffers;
        glGenerateMipmap = ::glGenerateMipmap;
        glGenFramebuffers = ::glGenFramebuffers;
        glGenRenderbuffers = ::glGenRenderbuffers;
        glGenSamplers = ::glGenSamplers;
        glGenVertexArrays = ::glGenVertexArrays;
        glGetActiveAttrib = ::glGetActiveAttrib;
        glGetActiveUniform = ::glGetActiveUniform;
        glGetActiveUniformBlockiv = ::glGetActiveUniformBlockiv;
        glGetActiveUniformBlockName = ::glGetActiveUniformBlockName;
        glGetAttribLocation = ::glGetAttribLocation;
        glGetProgramiv = ::glGetProgramiv;
        glGetShaderInfoLog = ::glGetShaderInfoLog;
        glGetShaderiv = ::glGetShaderiv;
        glGetUniformBlockIndex = ::glGetUniformBlockIndex;
        glGetUniformLocation = ::glGetUniformLocation;
        glLinkProgram = ::glLinkProgram;
        glRenderbufferStorage = ::glRenderbufferStorage;
        glSampleCoverage = ::glSampleCoverage;
        glSamplerParameterf = ::glSamplerParameterf;
        glSamplerParameterfv = ::glSamplerParameterfv;
        glSamplerParameteri = ::glSamplerParameteri;
        glShaderSource = ::glShaderSource;
        glStencilFuncSeparate = ::glStencilFuncSeparate;
        glStencilOpSeparate = ::glStencilOpSeparate;
        glUniform1i = ::glUniform1i;
        glUniformBlockBinding = ::glUniformBlockBinding;
        glUseProgram = ::glUseProgram;
        glVertexAttribIPointer = ::glVertexAttribIPointer;
        glVertexAttribPointer = ::glVertexAttribPointer;
    }
    
}

#endif
