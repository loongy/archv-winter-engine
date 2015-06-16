/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLRENDERCONTEXT_H_
#define _WNTR_OPENGLRENDERCONTEXT_H_

#include "../RenderContext.h"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <gl/gl.h>
#include "impl-win32/glext.h"
#elif __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#elif __linux
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>
#endif

namespace wntr {

#if defined( _WIN32) || defined(__linux)
	extern PFNGLDELETEBUFFERSPROC       glDeleteBuffers;
	extern PFNGLDELETEFRAMEBUFFERSPROC  glDeleteFramebuffers;
	extern PFNGLDELETEPROGRAMPROC       glDeleteProgram;
	extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
	extern PFNGLDELETESAMPLERSPROC      glDeleteSamplers;
	extern PFNGLDELETESHADERPROC        glDeleteShader;
	extern PFNGLDELETEVERTEXARRAYSPROC  glDeleteVertexArrays;
#endif
    
	class openglAttributeArray;
	class openglBlendState;
	class openglDepthStencilState;
	class openglFramebuffer;
	class openglProgram;
	class openglRasterizerState;

	class openglRenderContext : public RenderContext {
	public:
#ifndef _MSC_VER
		openglRenderContext(openglRenderContext&&) = default;
		openglRenderContext& operator= (openglRenderContext&&) = default;
#endif
		openglRenderContext(const openglRenderContext&) = delete;
		openglRenderContext& operator= (const openglRenderContext&) = delete;

		explicit openglRenderContext(unsigned width, unsigned height, bool vsync, bool fullscreen);
		virtual ~openglRenderContext() = default;

		virtual void clear(float red, float green, float blue, float alpha, float depth, unsigned stencil) override;
		virtual void clearColor(float red, float green, float blue, float alpha) override;
		virtual void clearDepth(float depth) override;
		virtual void clearStencil(unsigned stencil) override;

		virtual void draw() override;
		virtual void drawInstances(unsigned count) override;

		virtual void setFramebuffer(const std::shared_ptr<Framebuffer>& framebuffer) override;
		virtual void setProgram(const std::shared_ptr<Program>& program) override;
		virtual void setAttributeArray(const std::shared_ptr<AttributeArray>& attribute_array) override;
		virtual void setBlendState(const std::shared_ptr<BlendState>& state) override;
		virtual void setDepthStencilState(const std::shared_ptr<DepthStencilState>& state) override;
		virtual void setRasterizerState(const std::shared_ptr<RasterizerState>& state) override;

		virtual std::shared_ptr<Framebuffer>       getFramebuffer() const override;
		virtual std::shared_ptr<Program>           getProgram() const override;
		virtual std::shared_ptr<AttributeArray>    getAttributeArray() const override;
		virtual std::shared_ptr<BlendState>        getBlendState() const override;
		virtual std::shared_ptr<DepthStencilState> getDepthStencilState() const override;
		virtual std::shared_ptr<RasterizerState>   getRasterizerState() const override;

		PFNGLACTIVETEXTUREPROC 				glActiveTexture;
		PFNGLATTACHSHADERPROC 				glAttachShader;
		PFNGLBINDBUFFERPROC 				glBindBuffer;
		PFNGLBINDBUFFERBASEPROC 			glBindBufferBase;
		PFNGLBINDFRAMEBUFFERPROC			glBindFramebuffer;
		PFNGLBINDRENDERBUFFERPROC           glBindRenderbuffer;
		PFNGLBINDSAMPLERPROC 				glBindSampler;
		PFNGLBINDVERTEXARRAYPROC			glBindVertexArray;
		PFNGLBLENDEQUATIONSEPARATEPROC		glBlendEquationSeparate;
		PFNGLBLENDEQUATIONSEPARATEIPROC		glBlendEquationSeparatei;
		PFNGLBLENDFUNCSEPARATEPROC			glBlendFuncSeparate;
		PFNGLBLENDFUNCSEPARATEIPROC			glBlendFuncSeparatei;
		PFNGLBUFFERDATAPROC 				glBufferData;
		PFNGLBUFFERSUBDATAPROC 				glBufferSubData;
		PFNGLCHECKFRAMEBUFFERSTATUSPROC		glCheckFramebufferStatus;
		PFNGLCOMPILESHADERPROC				glCompileShader;
		PFNGLCREATEPROGRAMPROC				glCreateProgram;
		PFNGLCREATESHADERPROC				glCreateShader;
		PFNGLDISABLEIPROC					glDisablei;
		PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray;
		PFNGLDRAWARRAYSINSTANCEDPROC        glDrawArraysInstanced;
		PFNGLDRAWBUFFERSPROC				glDrawBuffers;
		PFNGLDRAWELEMENTSINSTANCEDPROC      glDrawElementsInstanced;
		PFNGLENABLEIPROC					glEnablei;
		PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray;
		PFNGLFRAMEBUFFERRENDERBUFFERPROC    glFramebufferRenderbuffer;
		PFNGLFRAMEBUFFERTEXTURE2DPROC 		glFramebufferTexture2D;
		PFNGLFRAMEBUFFERTEXTUREPROC			glFramebufferTexture;
		PFNGLGENBUFFERSPROC 				glGenBuffers;
		PFNGLGENERATEMIPMAPPROC				glGenerateMipmap;
		PFNGLGENFRAMEBUFFERSPROC			glGenFramebuffers;
		PFNGLGENRENDERBUFFERSPROC           glGenRenderbuffers;
		PFNGLGENSAMPLERSPROC				glGenSamplers;
		PFNGLGENVERTEXARRAYSPROC			glGenVertexArrays;
		PFNGLGETACTIVEATTRIBPROC			glGetActiveAttrib;
		PFNGLGETACTIVEUNIFORMPROC			glGetActiveUniform;
		PFNGLGETACTIVEUNIFORMBLOCKIVPROC	glGetActiveUniformBlockiv;
		PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC	glGetActiveUniformBlockName;
		PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation;
		PFNGLGETPROGRAMIVPROC				glGetProgramiv;
		PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog;
		PFNGLGETSHADERIVPROC				glGetShaderiv;
		PFNGLGETUNIFORMBLOCKINDEXPROC		glGetUniformBlockIndex;
		PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation;
		PFNGLLINKPROGRAMPROC				glLinkProgram;
		PFNGLRENDERBUFFERSTORAGEPROC        glRenderbufferStorage;
		PFNGLSAMPLECOVERAGEPROC             glSampleCoverage;
		PFNGLSAMPLERPARAMETERFPROC			glSamplerParameterf;
		PFNGLSAMPLERPARAMETERFVPROC			glSamplerParameterfv;
		PFNGLSAMPLERPARAMETERIPROC			glSamplerParameteri;
		PFNGLSHADERSOURCEPROC				glShaderSource;
		PFNGLSTENCILFUNCSEPARATEPROC        glStencilFuncSeparate;
		PFNGLSTENCILOPSEPARATEPROC          glStencilOpSeparate;
		PFNGLUNIFORM1IPROC					glUniform1i;
		PFNGLUNIFORMBLOCKBINDINGPROC		glUniformBlockBinding;
		PFNGLUSEPROGRAMPROC					glUseProgram;
		PFNGLVERTEXATTRIBIPOINTERPROC		glVertexAttribIPointer;
		PFNGLVERTEXATTRIBPOINTERPROC		glVertexAttribPointer;

	protected:
		std::shared_ptr<openglFramebuffer> _framebuffer;
		std::shared_ptr<openglProgram> _program;
		std::shared_ptr<openglAttributeArray> _attribute_array;
		std::shared_ptr<openglBlendState> _blend_state;
		std::shared_ptr<openglDepthStencilState> _depth_stencil_state;
		std::shared_ptr<openglRasterizerState> _rasterizer_state;

	private:

	};

}

#endif
