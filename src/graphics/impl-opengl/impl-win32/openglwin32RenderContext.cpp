/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef _WIN32

#include "openglwin32RenderContext.h"

#include <platform/impl-win32/win32Window.h>

#include <cassert>
#include <iostream>

#define WGL_DRAW_TO_WINDOW_ARB         0x2001
#define WGL_ACCELERATION_ARB           0x2003
#define WGL_SWAP_METHOD_ARB            0x2007
#define WGL_SUPPORT_OPENGL_ARB         0x2010
#define WGL_DOUBLE_BUFFER_ARB          0x2011
#define WGL_PIXEL_TYPE_ARB             0x2013
#define WGL_COLOR_BITS_ARB             0x2014
#define WGL_DEPTH_BITS_ARB             0x2022
#define WGL_STENCIL_BITS_ARB           0x2023
#define WGL_SAMPLE_BUFFERS_ARB 		   0x2041
#define WGL_SAMPLES_ARB				   0x2042
#define WGL_FULL_ACCELERATION_ARB      0x2027
#define WGL_SWAP_EXCHANGE_ARB          0x2028
#define WGL_TYPE_RGBA_ARB              0x202B
#define WGL_CONTEXT_MAJOR_VERSION_ARB  0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB  0x2092

namespace wntr {

	PFNGLDELETEBUFFERSPROC       glDeleteBuffers = nullptr;
	PFNGLDELETEFRAMEBUFFERSPROC  glDeleteFramebuffers = nullptr;
	PFNGLDELETEPROGRAMPROC       glDeleteProgram = nullptr;
	PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = nullptr;
	PFNGLDELETESAMPLERSPROC      glDeleteSamplers = nullptr;
	PFNGLDELETESHADERPROC        glDeleteShader = nullptr;
	PFNGLDELETEVERTEXARRAYSPROC  glDeleteVertexArrays = nullptr;
	
	openglwin32RenderContext::openglwin32RenderContext(const std::shared_ptr<win32Window>& window, unsigned width, unsigned height, bool vsync, bool fullscreen)
	: openglRenderContext(width, height, vsync, fullscreen), _window(window) {
		assert(window);
		assert(width);
		assert(height);

		_hdc = GetDC(_window->getHWND().get());
		if (!_hdc)
			throw std::runtime_error("error: openglwin32RenderContext::openglwin32RenderContext: failed to create device context");

		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,
			8,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0,
		};

		int old_pixel_format = ChoosePixelFormat(_hdc, &pfd);
		if (!old_pixel_format)
			throw std::runtime_error("error: openglwin32RenderContext::openglwin32RenderContext: failed to choose pixel format");
		if (!SetPixelFormat(_hdc, old_pixel_format, &pfd))
			throw std::runtime_error("error: openglwin32RenderContext::openglwin32RenderContext: failed to set pixel format");

		HGLRC old_gl_context = wglCreateContext(_hdc);
		if (!old_gl_context)
			throw std::runtime_error("error: openglwin32RenderContext::openglwin32RenderContext: failed to create old OpenGL context");
		if (!wglMakeCurrent(_hdc, old_gl_context))
			throw std::runtime_error("error: openglwin32RenderContext::openglwin32RenderContext: failed to make old OpenGL context current");

		_hglrc = nullptr;
		if (!getNewerOpenGLFunctionPointers())
			throw std::runtime_error("error: openglwin32RenderContext::openglwin32RenderContext: failed to get newer OpenGL functions");

		const int pixel_format_attributes[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0,
		};

		int pixel_format;
		unsigned int num_format;

		assert(_wglChoosePixelFormatARB);
		if (!_wglChoosePixelFormatARB(_hdc, pixel_format_attributes, nullptr, 1, &pixel_format, &num_format))
			throw std::runtime_error("error: openglwin32RenderContext::openglwin32RenderContext: failed to choose ARB pixel format");

		if (getNewerOpenGL()) {
			assert(_hdc);
			assert(_hglrc);

			if (!wglMakeCurrent(nullptr, nullptr))
				throw std::runtime_error("error: openglwin32RenderContext::openglwin32RenderContext: failed to make null OpenGL context current");
			if (!wglDeleteContext(old_gl_context))
				throw std::runtime_error("error: openglwin32RenderContext::openglwin32RenderContext: failed to delete old OpenGL context");
			if (!wglMakeCurrent(_hdc, _hglrc))
				throw std::runtime_error("error: openglwin32RenderContext::openglwin32RenderContext: failed to make newer OpenGL context current");

			setVsync(vsync);

			glFrontFace(GL_CW);
			glViewport(0, 0, _width, _height);

			setBlendState(nullptr);
			setDepthStencilState(nullptr);
			setRasterizerState(nullptr);

			const char* opengl_version = (const char*)glGetString(GL_VERSION);
			const char* opengl_vendor = (const char*)glGetString(GL_VENDOR);
			const char* opengl_renderer = (const char*)glGetString(GL_RENDERER);

			std::cout << "Winter Info: OpenGL Version "  << opengl_version  << std::endl;
			std::cout << "Winter Info: OpenGL Vendor "   << opengl_vendor   << std::endl;
			std::cout << "Winter Info: OpenGL Renderer " << opengl_renderer << std::endl;
		}
		else {
			if (!wglMakeCurrent(nullptr, nullptr))
				throw std::runtime_error("error: openglwin32RenderContext::openglwin32RenderContext: failed to make null OpenGL context current");
			if (!wglDeleteContext(old_gl_context))
				throw std::runtime_error("error: openglwin32RenderContext::openglwin32RenderContext: failed to delete old OpenGL context");
			throw std::runtime_error("error: openglwin32RenderContext::openglwin32RenderContext: failed to create newer OpenGL context");
		}
	}

    openglwin32RenderContext::~openglwin32RenderContext() {
		if (!wglMakeCurrent(nullptr, nullptr)) {
		}
		if (_hglrc) {
			if (!wglDeleteContext(_hglrc)) {
			}
			_hglrc = nullptr;
		}
		if (_window && _hdc) {
			if (!ReleaseDC(_window->getHWND().get(), _hdc)) {
			}
			_hdc = nullptr;
		}
    }

	void openglwin32RenderContext::swapBuffers() {
		assert(_hdc);
		SwapBuffers(_hdc);
	}

	void openglwin32RenderContext::lock() {
        // TODO: lock opengl context
        if (!wglMakeCurrent(_hdc, _hglrc))
            throw std::runtime_error("error: openglwin32RenderContext::lock: failed to make newer OpenGL context current");
	}

	void openglwin32RenderContext::unlock() {
        // TODO: unlock opengl context
	}

	void openglwin32RenderContext::setVsync(bool vsync) {
		openglRenderContext::setVsync(vsync);
		assert(_wglSwapIntervalEXT);
		_wglSwapIntervalEXT(vsync ? 1 : 0);
	}

	const std::shared_ptr<win32Window>& openglwin32RenderContext::getWindow() const {
		return _window;
	}

	bool openglwin32RenderContext::getNewerOpenGLFunctionPointers() {
		// load public global OpenGL functions
		if (!glDeleteBuffers) {
			glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
			if (!glDeleteBuffers)  {
				return false;
			}
		}
		if (!glDeleteFramebuffers) {
			glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
			if (!glDeleteFramebuffers)  {
				return false;
			}
		}
		if (!glDeleteProgram) {
			glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
			if (!glDeleteProgram)  {
				return false;
			}
		}
		if (!glDeleteRenderbuffers) {
			glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
			if (!glDeleteRenderbuffers)  {
				return false;
			}
		}
		if (!glDeleteSamplers) {
			glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)wglGetProcAddress("glDeleteSamplers");
			if (!glDeleteSamplers)  {
				return false;
			}
		}
		if (!glDeleteShader) {
			glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
			if (!glDeleteShader)  {
				return false;
			}
		}
		if (!glDeleteVertexArrays) {
			glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
			if (!glDeleteVertexArrays)  {
				return false;
			}
		}

		// load OpenGL functions
		glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
		if (!glActiveTexture)  {
			return false;
		}
		glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
		if (!glAttachShader)  {
			return false;
		}
		glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
		if (!glBindBuffer)  {
			return false;
		}
		glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)wglGetProcAddress("glBindBufferBase");
		if (!glBindBufferBase)  {
			return false;
		}
		glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
		if (!glBindFramebuffer)  {
			return false;
		}
		glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
		if (!glBindRenderbuffer)  {
			return false;
		}
		glBindSampler = (PFNGLBINDSAMPLERPROC)wglGetProcAddress("glBindSampler");
		if (!glBindSampler)  {
			return false;
		}
		glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
		if (!glBindVertexArray)  {
			return false;
		}
		glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)wglGetProcAddress("glBlendEquationSeparate");
		if (!glBlendEquationSeparate)  {
			return false;
		}
		glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC)wglGetProcAddress("glBlendEquationSeparatei");
		if (!glBlendEquationSeparatei)  {
			return false;
		}
		glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)wglGetProcAddress("glBlendFuncSeparate");
		if (!glBlendFuncSeparate)  {
			return false;
		}
		glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC)wglGetProcAddress("glBlendFuncSeparatei");
		if (!glBlendFuncSeparatei)  {
			return false;
		}
		glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
		if (!glBufferData)  {
			return false;
		}
		glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
		if (!glBufferSubData)  {
			return false;
		}
		glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
		if (!glCheckFramebufferStatus)  {
			return false;
		}
		glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
		if (!glCompileShader)  {
			return false;
		}
		glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
		if (!glCreateProgram)  {
			return false;
		}
		glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
		if (!glCreateShader)  {
			return false;
		}
		glDisablei = (PFNGLDISABLEIPROC)wglGetProcAddress("glDisablei");
		if (!glDisablei)  {
			return false;
		}
		glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
		if (!glDisableVertexAttribArray)  {
			return false;
		}
		glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers");
		if (!glDrawBuffers)  {
			return false;
		}
		glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)wglGetProcAddress("glDrawElementsInstanced");
		if (!glDrawElementsInstanced)  {
			return false;
		}
		glEnablei = (PFNGLENABLEIPROC)wglGetProcAddress("glEnablei");
		if (!glEnablei)  {
			return false;
		}
		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
		if (!glEnableVertexAttribArray)  {
			return false;
		}
		glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
		if (!glFramebufferRenderbuffer)  {
			return false;
		}
		glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
		if (!glFramebufferTexture2D)  {
			return false;
		}
		glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)wglGetProcAddress("glFramebufferTexture");
		if (!glFramebufferTexture)  {
			return false;
		}
		glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
		if (!glGenBuffers)  {
			return false;
		}
		glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
		if (!glGenerateMipmap)  {
			return false;
		}
		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
		if (!glGenFramebuffers)  {
			return false;
		}
		glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
		if (!glGenRenderbuffers)  {
			return false;
		}
		glGenSamplers = (PFNGLGENSAMPLERSPROC)wglGetProcAddress("glGenSamplers");
		if (!glGenSamplers)  {
			return false;
		}
		glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
		if (!glGenVertexArrays)  {
			return false;
		}
		glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)wglGetProcAddress("glGetActiveAttrib");
		if (!glGetActiveAttrib)  {
			return false;
		}
		glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");
		if (!glGetActiveUniform)  {
			return false;
		}
		glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)wglGetProcAddress("glGetActiveUniformBlockiv");
		if (!glGetActiveUniformBlockiv)  {
			return false;
		}
		glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)wglGetProcAddress("glGetActiveUniformBlockName");
		if (!glGetActiveUniformBlockName)  {
			return false;
		}
		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
		if (!glGetAttribLocation)  {
			return false;
		}
		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
		if (!glGetProgramiv)  {
			return false;
		}
		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
		if (!glGetShaderInfoLog)  {
			return false;
		}
		glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
		if (!glGetShaderiv)  {
			return false;
		}
		glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)wglGetProcAddress("glGetUniformBlockIndex");
		if (!glGetUniformBlockIndex)  {
			return false;
		}
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
		if (!glGetUniformLocation)  {
			return false;
		}
		glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
		if (!glLinkProgram)  {
			return false;
		}
		glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
		if (!glRenderbufferStorage)  {
			return false;
		}
		glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)wglGetProcAddress("glSampleCoverage");
		if (!glSampleCoverage)  {
			return false;
		}
		glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)wglGetProcAddress("glSamplerParameterf");
		if (!glSamplerParameterf)  {
			return false;
		}
		glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)wglGetProcAddress("glSamplerParameterfv");
		if (!glSamplerParameterfv)  {
			return false;
		}
		glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)wglGetProcAddress("glSamplerParameteri");
		if (!glSamplerParameteri)  {
			return false;
		}
		glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
		if (!glShaderSource)  {
			return false;
		}
		glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)wglGetProcAddress("glStencilFuncSeparate");
		if (!glStencilFuncSeparate)  {
			return false;
		}
		glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)wglGetProcAddress("glStencilOpSeparate");
		if (!glStencilOpSeparate)  {
			return false;
		}
		glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
		if (!glUniform1i)  {
			return false;
		}
		glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)wglGetProcAddress("glUniformBlockBinding");
		if (!glUniformBlockBinding)  {
			return false;
		}
		glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
		if (!glUseProgram)  {
			return false;
		}
		glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)wglGetProcAddress("glVertexAttribIPointer");
		if (!glVertexAttribIPointer)  {
			return false;
		}
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
		if (!glVertexAttribPointer)  {
			return false;
		}

		// load WGL functions
		_wglChoosePixelFormatARB = (wglChoosePixelFormatARB_PROC)wglGetProcAddress("wglChoosePixelFormatARB");
		if (!_wglChoosePixelFormatARB) {
			return false;
		}

		_wglCreateContextAttribsARB = (wglCreateContextAttribsARB_PROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if (!_wglCreateContextAttribsARB) {
			return false;
		}

		_wglSwapIntervalEXT = (wglSwapIntervalEXT_PROC)wglGetProcAddress("wglSwapIntervalEXT");
		if (!_wglSwapIntervalEXT)  {
			return false;
		}

		return true;
	}

	bool openglwin32RenderContext::getNewerOpenGL() {
		assert(_hdc);
		assert(_wglCreateContextAttribsARB);
		
		const int context_attributes_44[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 4,
			0,
		};
		_hglrc = _wglCreateContextAttribsARB(_hdc, 0, context_attributes_44);
		if (_hglrc) {
			_version = API_VERSION::OPENGL44;
			return true;
		}
			
		const int context_attributes_43[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			0,
		};
		_hglrc = _wglCreateContextAttribsARB(_hdc, 0, context_attributes_43);
		if (_hglrc) {
			_version = API_VERSION::OPENGL43;
			return true;
		}

		const int context_attributes_42[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 2,
			0,
		};
		_hglrc = _wglCreateContextAttribsARB(_hdc, 0, context_attributes_42);
		if (_hglrc) {
			_version = API_VERSION::OPENGL42;
			return true;
		}

		const int context_attributes_41[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 1,
			0,
		};
		_hglrc = _wglCreateContextAttribsARB(_hdc, 0, context_attributes_41);
		if (_hglrc) {
			_version = API_VERSION::OPENGL41;
			return true;
		}

		const int context_attributes_40[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 0,
			0,
		};
		_hglrc = _wglCreateContextAttribsARB(_hdc, 0, context_attributes_40);
		if (_hglrc) {
			_version = API_VERSION::OPENGL40;
			return true;
		}

		const int context_attributes_33[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			0,
		};
		_hglrc = _wglCreateContextAttribsARB(_hdc, 0, context_attributes_33);
		if (_hglrc) {
			_version = API_VERSION::OPENGL33;
			return true;
		}

		return false;
	}

}

#endif
