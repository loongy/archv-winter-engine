/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef __linux

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092

#include "openglx11RenderContext.h"

#include <platform/impl-x11/x11Window.h>

namespace wntr {

	PFNGLDELETEBUFFERSPROC       glDeleteBuffers = nullptr;
	PFNGLDELETEFRAMEBUFFERSPROC  glDeleteFramebuffers = nullptr;
	PFNGLDELETEPROGRAMPROC       glDeleteProgram = nullptr;
	PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = nullptr;
	PFNGLDELETESAMPLERSPROC      glDeleteSamplers = nullptr;
	PFNGLDELETESHADERPROC        glDeleteShader = nullptr;
	PFNGLDELETEVERTEXARRAYSPROC  glDeleteVertexArrays = nullptr;

	openglx11RenderContext::openglx11RenderContext(const std::shared_ptr<x11Window>& window, unsigned width, unsigned height, bool vsync, bool fullscreen)
	: openglRenderContext(width, height, vsync, fullscreen), _window(window) {
		assert(_window);
		assert(_window->getXWindow());
		assert(_window->getXDisplay());
		assert(_window->getXVisualInfo());

		if (!getNewerOpenGLFunctionPointers())
					throw std::runtime_error("error: openglx11RenderContext::openglx11RenderContext: failed to get newer OpenGL functions");

		_gl = nullptr;
		for (int minor = 3; minor >= 0 && !_gl; minor--) {
			int glattribs[] = {
				GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
				GLX_CONTEXT_MINOR_VERSION_ARB, minor,
				None
			};
			_gl = _glXCreateContextAttribsARB(_window->getXDisplay(), _window->getGLXFBConfig(), 0, True, glattribs);
		}
		if (!_gl)
			throw std::runtime_error("error: openglx11RenderContext::openglx11RenderContext: failed to get create OpenGL context");

		glXMakeCurrent(_window->getXDisplay(), _window->getXWindow(), _gl);

		const char* opengl_version = (const char*)glGetString(GL_VERSION);
		const char* opengl_vendor = (const char*)glGetString(GL_VENDOR);
		const char* opengl_renderer = (const char*)glGetString(GL_RENDERER);

		std::cout << "Winter Info: OpenGL Version "  << opengl_version  << std::endl;
		std::cout << "Winter Info: OpenGL Vendor "   << opengl_vendor   << std::endl;
		std::cout << "Winter Info: OpenGL Renderer " << opengl_renderer << std::endl;
	}

	openglx11RenderContext::~openglx11RenderContext() {
		if (_window && _gl) {
			glXMakeCurrent(_window->getXDisplay(), None, NULL);
			glXDestroyContext(_window->getXDisplay(), _gl);
		}
	}

	void openglx11RenderContext::swapBuffers() {
		assert(_window);
		assert(_window->getXWindow());
		assert(_window->getXDisplay());

		glXSwapBuffers(_window->getXDisplay(), _window->getXWindow());
	}

	void openglx11RenderContext::lock() {
		assert(_gl);
		assert(_window);
		assert(_window->getXWindow());
		assert(_window->getXDisplay());

		glXMakeCurrent(_window->getXDisplay(), _window->getXWindow(), _gl);
	}

	void openglx11RenderContext::unlock() {
		assert(_window);
		assert(_window->getXDisplay());

		glXMakeCurrent(_window->getXDisplay(), None, NULL);
	}

	void openglx11RenderContext::setVsync(bool vsync) {

	}

	bool openglx11RenderContext::getNewerOpenGLFunctionPointers() {
		// load public global OpenGL functions
		if (!glDeleteBuffers) {
			glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glXGetProcAddress((const unsigned char*)"glDeleteBuffers");
			if (!glDeleteBuffers)  {
				return false;
			}
		}
		if (!glDeleteFramebuffers) {
			glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)glXGetProcAddress((const unsigned char*)"glDeleteFramebuffers");
			if (!glDeleteFramebuffers)  {
				return false;
			}
		}
		if (!glDeleteProgram) {
			glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glXGetProcAddress((const unsigned char*)"glDeleteProgram");
			if (!glDeleteProgram)  {
				return false;
			}
		}
		if (!glDeleteRenderbuffers) {
			glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)glXGetProcAddress((const unsigned char*)"glDeleteRenderbuffers");
			if (!glDeleteRenderbuffers)  {
				return false;
			}
		}
		if (!glDeleteSamplers) {
			glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)glXGetProcAddress((const unsigned char*)"glDeleteSamplers");
			if (!glDeleteSamplers)  {
				return false;
			}
		}
		if (!glDeleteShader) {
			glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddress((const unsigned char*)"glDeleteShader");
			if (!glDeleteShader)  {
				return false;
			}
		}
		if (!glDeleteVertexArrays) {
			glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glXGetProcAddress((const unsigned char*)"glDeleteVertexArrays");
			if (!glDeleteVertexArrays)  {
				return false;
			}
		}

		// load OpenGL functions
		glActiveTexture = (PFNGLACTIVETEXTUREPROC)glXGetProcAddress((const unsigned char*)"glActiveTexture");
		if (!glActiveTexture)  {
			return false;
		}
		glAttachShader = (PFNGLATTACHSHADERPROC)glXGetProcAddress((const unsigned char*)"glAttachShader");
		if (!glAttachShader)  {
			return false;
		}
		glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress((const unsigned char*)"glBindBuffer");
		if (!glBindBuffer)  {
			return false;
		}
		glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)glXGetProcAddress((const unsigned char*)"glBindBufferBase");
		if (!glBindBufferBase)  {
			return false;
		}
		glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glXGetProcAddress((const unsigned char*)"glBindFramebuffer");
		if (!glBindFramebuffer)  {
			return false;
		}
		glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)glXGetProcAddress((const unsigned char*)"glBindRenderbuffer");
		if (!glBindRenderbuffer)  {
			return false;
		}
		glBindSampler = (PFNGLBINDSAMPLERPROC)glXGetProcAddress((const unsigned char*)"glBindSampler");
		if (!glBindSampler)  {
			return false;
		}
		glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress((const unsigned char*)"glBindVertexArray");
		if (!glBindVertexArray)  {
			return false;
		}
		glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)glXGetProcAddress((const unsigned char*)"glBlendEquationSeparate");
		if (!glBlendEquationSeparate)  {
			return false;
		}
		glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC)glXGetProcAddress((const unsigned char*)"glBlendEquationSeparatei");
		if (!glBlendEquationSeparatei)  {
			return false;
		}
		glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)glXGetProcAddress((const unsigned char*)"glBlendFuncSeparate");
		if (!glBlendFuncSeparate)  {
			return false;
		}
		glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC)glXGetProcAddress((const unsigned char*)"glBlendFuncSeparatei");
		if (!glBlendFuncSeparatei)  {
			return false;
		}
		glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress((const unsigned char*)"glBufferData");
		if (!glBufferData)  {
			return false;
		}
		glBufferSubData = (PFNGLBUFFERSUBDATAPROC)glXGetProcAddress((const unsigned char*)"glBufferSubData");
		if (!glBufferSubData)  {
			return false;
		}
		glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)glXGetProcAddress((const unsigned char*)"glCheckFramebufferStatus");
		if (!glCheckFramebufferStatus)  {
			return false;
		}
		glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddress((const unsigned char*)"glCompileShader");
		if (!glCompileShader)  {
			return false;
		}
		glCreateProgram = (PFNGLCREATEPROGRAMPROC)glXGetProcAddress((const unsigned char*)"glCreateProgram");
		if (!glCreateProgram)  {
			return false;
		}
		glCreateShader = (PFNGLCREATESHADERPROC)glXGetProcAddress((const unsigned char*)"glCreateShader");
		if (!glCreateShader)  {
			return false;
		}
		glDisablei = (PFNGLDISABLEIPROC)glXGetProcAddress((const unsigned char*)"glDisablei");
		if (!glDisablei)  {
			return false;
		}
		glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((const unsigned char*)"glDisableVertexAttribArray");
		if (!glDisableVertexAttribArray)  {
			return false;
		}
		glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)glXGetProcAddress((const unsigned char*)"glDrawArraysInstanced");
		if (!glDrawArraysInstanced)  {
			return false;
		}
		glDrawBuffers = (PFNGLDRAWBUFFERSPROC)glXGetProcAddress((const unsigned char*)"glDrawBuffers");
		if (!glDrawBuffers)  {
			return false;
		}
		glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)glXGetProcAddress((const unsigned char*)"glDrawElementsInstanced");
		if (!glDrawElementsInstanced)  {
			return false;
		}
		glEnablei = (PFNGLENABLEIPROC)glXGetProcAddress((const unsigned char*)"glEnablei");
		if (!glEnablei)  {
			return false;
		}
		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((const unsigned char*)"glEnableVertexAttribArray");
		if (!glEnableVertexAttribArray)  {
			return false;
		}
		glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)glXGetProcAddress((const unsigned char*)"glFramebufferRenderbuffer");
		if (!glFramebufferRenderbuffer)  {
			return false;
		}
		glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)glXGetProcAddress((const unsigned char*)"glFramebufferTexture2D");
		if (!glFramebufferTexture2D)  {
			return false;
		}
		glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)glXGetProcAddress((const unsigned char*)"glFramebufferTexture");
		if (!glFramebufferTexture)  {
			return false;
		}
		glGenBuffers = (PFNGLGENBUFFERSPROC)glXGetProcAddress((const unsigned char*)"glGenBuffers");
		if (!glGenBuffers)  {
			return false;
		}
		glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glXGetProcAddress((const unsigned char*)"glGenerateMipmap");
		if (!glGenerateMipmap)  {
			return false;
		}
		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)glXGetProcAddress((const unsigned char*)"glGenFramebuffers");
		if (!glGenFramebuffers)  {
			return false;
		}
		glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)glXGetProcAddress((const unsigned char*)"glGenRenderbuffers");
		if (!glGenRenderbuffers)  {
			return false;
		}
		glGenSamplers = (PFNGLGENSAMPLERSPROC)glXGetProcAddress((const unsigned char*)"glGenSamplers");
		if (!glGenSamplers)  {
			return false;
		}
		glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress((const unsigned char*)"glGenVertexArrays");
		if (!glGenVertexArrays)  {
			return false;
		}
		glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)glXGetProcAddress((const unsigned char*)"glGetActiveAttrib");
		if (!glGetActiveAttrib)  {
			return false;
		}
		glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)glXGetProcAddress((const unsigned char*)"glGetActiveUniform");
		if (!glGetActiveUniform)  {
			return false;
		}
		glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)glXGetProcAddress((const unsigned char*)"glGetActiveUniformBlockiv");
		if (!glGetActiveUniformBlockiv)  {
			return false;
		}
		glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)glXGetProcAddress((const unsigned char*)"glGetActiveUniformBlockName");
		if (!glGetActiveUniformBlockName)  {
			return false;
		}
		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)glXGetProcAddress((const unsigned char*)"glGetAttribLocation");
		if (!glGetAttribLocation)  {
			return false;
		}
		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glXGetProcAddress((const unsigned char*)"glGetProgramiv");
		if (!glGetProgramiv)  {
			return false;
		}
		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress((const unsigned char*)"glGetShaderInfoLog");
		if (!glGetShaderInfoLog)  {
			return false;
		}
		glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddress((const unsigned char*)"glGetShaderiv");
		if (!glGetShaderiv)  {
			return false;
		}
		glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)glXGetProcAddress((const unsigned char*)"glGetUniformBlockIndex");
		if (!glGetUniformBlockIndex)  {
			return false;
		}
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress((const unsigned char*)"glGetUniformLocation");
		if (!glGetUniformLocation)  {
			return false;
		}
		glLinkProgram = (PFNGLLINKPROGRAMPROC)glXGetProcAddress((const unsigned char*)"glLinkProgram");
		if (!glLinkProgram)  {
			return false;
		}
		glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)glXGetProcAddress((const unsigned char*)"glRenderbufferStorage");
		if (!glRenderbufferStorage)  {
			return false;
		}
		glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)glXGetProcAddress((const unsigned char*)"glSampleCoverage");
		if (!glSampleCoverage)  {
			return false;
		}
		glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)glXGetProcAddress((const unsigned char*)"glSamplerParameterf");
		if (!glSamplerParameterf)  {
			return false;
		}
		glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)glXGetProcAddress((const unsigned char*)"glSamplerParameterfv");
		if (!glSamplerParameterfv)  {
			return false;
		}
		glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)glXGetProcAddress((const unsigned char*)"glSamplerParameteri");
		if (!glSamplerParameteri)  {
			return false;
		}
		glShaderSource = (PFNGLSHADERSOURCEPROC)glXGetProcAddress((const unsigned char*)"glShaderSource");
		if (!glShaderSource)  {
			return false;
		}
		glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)glXGetProcAddress((const unsigned char*)"glStencilFuncSeparate");
		if (!glStencilFuncSeparate)  {
			return false;
		}
		glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)glXGetProcAddress((const unsigned char*)"glStencilOpSeparate");
		if (!glStencilOpSeparate)  {
			return false;
		}
		glUniform1i = (PFNGLUNIFORM1IPROC)glXGetProcAddress((const unsigned char*)"glUniform1i");
		if (!glUniform1i)  {
			return false;
		}
		glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)glXGetProcAddress((const unsigned char*)"glUniformBlockBinding");
		if (!glUniformBlockBinding)  {
			return false;
		}
		glUseProgram = (PFNGLUSEPROGRAMPROC)glXGetProcAddress((const unsigned char*)"glUseProgram");
		if (!glUseProgram)  {
			return false;
		}
		glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)glXGetProcAddress((const unsigned char*)"glVertexAttribIPointer");
		if (!glVertexAttribIPointer)  {
			return false;
		}
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress((const unsigned char*)"glVertexAttribPointer");
		if (!glVertexAttribPointer)  {
			return false;
		}

		// load GLX functions

		_glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const unsigned char*)"glXCreateContextAttribsARB");
		if (!_glXCreateContextAttribsARB)  {
			return false;
		}

		return true;
	}
    
}

#endif
