/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglDepthbuffer.h"
#include "openglFramebuffer.h"
#include "openglRenderbuffer.h"
#include "openglRenderContext.h"
#include "openglTextureResource.h"

#include <algorithm>
#include <cassert>
#include <vector>

namespace wntr {
    
    openglFramebuffer::openglFramebuffer(unsigned num_renderbuffers, std::shared_ptr<Renderbuffer>* renderbuffers, const std::shared_ptr<Depthbuffer>& depthbuffer, const openglRenderContext* context)
    : Framebuffer(num_renderbuffers, renderbuffers, depthbuffer, context->getApi()), _max_width(0), _max_height(0) {
        assert(context);
        assert(renderbuffers || num_renderbuffers == 0);
        
        // save the current framebuffer
		GLint currently_bound_framebuffer;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currently_bound_framebuffer);

        context->glGenFramebuffers(1, &_opengl_id);
        context->glBindFramebuffer(GL_FRAMEBUFFER, _opengl_id); // this will override the current state
        
        std::vector<GLenum> drawbuffers;
        for (unsigned int i = 0; i < num_renderbuffers; ++i) {
            assert(renderbuffers[i]);
            std::shared_ptr<Renderbuffer>& renderbuffer = renderbuffers[i];
            if (renderbuffer->getResource()->getApi() != API::OPENGL)
                throw std::invalid_argument("error: openglFramebuffer::openglFramebuffer: expected IRenderbuffer to implement API::OPENGL");
            
            auto resource = std::static_pointer_cast<openglTextureResource>(renderbuffer->getResource());
            context->glFramebufferTexture(GL_FRAMEBUFFER,
                                          GL_COLOR_ATTACHMENT0 + i,
                                          resource->getOpenGLId(),
                                          0);
            drawbuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
            
            _max_width = std::max(_max_width, resource->getWidth());
            _max_height = std::max(_max_height, resource->getHeight());
        }
        context->glDrawBuffers((unsigned)drawbuffers.size(), &drawbuffers[0]);
        
        if (depthbuffer) {
            if (depthbuffer->getResource()->getApi() != API::OPENGL)
                throw std::invalid_argument("error: openglFramebuffer::openglFramebuffer: expected Depthbuffer to implement API::OPENGL");
            
            auto resource = std::static_pointer_cast<openglTextureResource>(depthbuffer->getResource());
            _max_width = std::max(_max_width, resource->getWidth());
            _max_height = std::max(_max_height, resource->getHeight());
            
            context->glFramebufferTexture(GL_FRAMEBUFFER,
                                          GL_DEPTH_ATTACHMENT,
                                          resource->getOpenGLId(),
                                          0);
        }
        else {
            context->glFramebufferTexture(GL_FRAMEBUFFER,
                                          GL_DEPTH_ATTACHMENT,
                                          0,
                                          0);
        }

        switch (context->glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
        case GL_FRAMEBUFFER_COMPLETE:
        	break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        	throw std::runtime_error("error: openglFramebuffer::openglFramebuffer: framebuffer is incomplete: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        	throw std::runtime_error("error: openglFramebuffer::openglFramebuffer: framebuffer is incomplete: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER :
        	throw std::runtime_error("error: openglFramebuffer::openglFramebuffer: framebuffer is incomplete: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER :
            throw std::runtime_error("error: openglFramebuffer::openglFramebuffer: framebuffer is incomplete: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE :
            throw std::runtime_error("error: openglFramebuffer::openglFramebuffer: framebuffer is incomplete: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            throw std::runtime_error("error: openglFramebuffer::openglFramebuffer: framebuffer is incomplete: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
        case GL_FRAMEBUFFER_UNSUPPORTED:
            throw std::runtime_error("error: openglFramebuffer::openglFramebuffer: framebuffer is incomplete: GL_FRAMEBUFFER_UNSUPPORTED");
        default:
        	throw std::runtime_error("error: openglFramebuffer::openglFramebuffer: framebuffer is incomplete");
        }

        // restore the previously framebuffer
		context->glBindFramebuffer(GL_FRAMEBUFFER, currently_bound_framebuffer);
    }
    
    openglFramebuffer::~openglFramebuffer() {
        glDeleteFramebuffers(1, &_opengl_id);
        _opengl_id = 0;
    }
    
    unsigned openglFramebuffer::getMaxWidth() const {
        return _max_width;
    }
    
    unsigned openglFramebuffer::getMaxHeight() const {
        return _max_height;
    }
    
    unsigned openglFramebuffer::getOpenGLId() const {
        return _opengl_id;
    }
    
}
