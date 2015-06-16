/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglRenderContext.h"
#include "openglSampler.h"

#include <cassert>

namespace wntr {
    
    openglSampler::openglSampler(const SamplerConfig& config, const openglRenderContext* context)
    : Sampler(config, context->getApi()) {
        assert(context);
        context->glGenSamplers(1, &_opengl_id);
        setSamplerParameters(context);
    }
    
    openglSampler::~openglSampler() {
        glDeleteSamplers(1, &_opengl_id);
        _opengl_id = 0;
    }
    
    void openglSampler::setConfig(const SamplerConfig& config, const RenderContext* context) {
        _config = config;
        setSamplerParameters(static_cast<const openglRenderContext*>(context));
    }
    
    unsigned openglSampler::getOpenGLId() const {
        return _opengl_id;
    }
    
    void openglSampler::setSamplerParameters(const openglRenderContext* context) {
        context->glSamplerParameterf(_opengl_id, GL_TEXTURE_MIN_LOD, _config.min_lod);
        context->glSamplerParameterf(_opengl_id, GL_TEXTURE_MAX_LOD, _config.max_lod);
        context->glSamplerParameterfv(_opengl_id, GL_TEXTURE_BORDER_COLOR, _config.border);
        
        switch (_config.min_filter) {
            case FILTER::NEAREST:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                break;
            case FILTER::LINEAR:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                break;
            case FILTER::NEAREST_MIPMAP_NEAREST:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                break;
            case FILTER::NEAREST_MIPMAP_LINEAR:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                break;
            case FILTER::LINEAR_MIPMAP_NEAREST:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                break;
            case FILTER::LINEAR_MIPMAP_LINEAR:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                break;
            default:
                throw std::logic_error("error: openglSampler::setSamplerParameters: encountered an unsupported FILTER");
        }
        switch (_config.mag_filter) {
            case FILTER::NEAREST:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                break;
            case FILTER::LINEAR:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;
            case FILTER::NEAREST_MIPMAP_NEAREST:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                break;
            case FILTER::NEAREST_MIPMAP_LINEAR:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                break;
            case FILTER::LINEAR_MIPMAP_NEAREST:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                break;
            case FILTER::LINEAR_MIPMAP_LINEAR:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                break;
            default:
                throw std::logic_error("error: openglSampler::setSamplerParameters: encountered an unsupported FILTER");
        }
        
        switch (_config.edge_u) {
            case TEXTURE_EDGE::CLAMP:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                break;
            case TEXTURE_EDGE::CLAMP_BORDER:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                break;
            case TEXTURE_EDGE::REPEAT:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
                break;
            case TEXTURE_EDGE::REPEAT_MIRRORED:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                break;
            default:
                throw std::logic_error("error: openglSampler::setSamplerParameters: encountered an unsupported TEXTURE_EDGE");
        }
        switch (_config.edge_v) {
            case TEXTURE_EDGE::CLAMP:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                break;
            case TEXTURE_EDGE::CLAMP_BORDER:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                break;
            case TEXTURE_EDGE::REPEAT:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;
            case TEXTURE_EDGE::REPEAT_MIRRORED:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                break;
            default:
                throw std::logic_error("error: openglSampler::setSamplerParameters: encountered an unsupported TEXTURE_EDGE");
        }
        switch (_config.edge_w) {
            case TEXTURE_EDGE::CLAMP:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                break;
            case TEXTURE_EDGE::CLAMP_BORDER:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
                break;
            case TEXTURE_EDGE::REPEAT:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_WRAP_R, GL_REPEAT);
                break;
            case TEXTURE_EDGE::REPEAT_MIRRORED:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
                break;
            default:
                throw std::logic_error("error: openglSampler::setSamplerParameters: encountered an unsupported TEXTURE_EDGE");
        }
        
        context->glSamplerParameteri(_opengl_id, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        switch (_config.comparison) {
            case COMPARISON::ALWAYS:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_COMPARE_FUNC, GL_ALWAYS);
                break;
            case COMPARISON::EQUAL:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_COMPARE_FUNC, GL_EQUAL);
                break;
            case COMPARISON::GREATER:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
                break;
            case COMPARISON::GREATER_EQUAL:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_COMPARE_FUNC, GL_GEQUAL);
                break;
            case COMPARISON::LESSER:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
                break;
            case COMPARISON::LESSER_EQUAL:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
                break;
            case COMPARISON::NEVER:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_COMPARE_FUNC, GL_NEVER);
                break;
            case COMPARISON::NOT_EQUAL:
                context->glSamplerParameteri(_opengl_id, GL_TEXTURE_COMPARE_FUNC, GL_NOTEQUAL);
                break;
            default:
                throw std::logic_error("error: openglSampler::setSamplerParameters: encountered an unsupported COMPARISON");
        }
    }
}