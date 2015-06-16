/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglAttributeArray.h"
#include "openglAttributeBuffer.h"
#include "openglBlendState.h"
#include "openglDepthStencilState.h"
#include "openglFramebuffer.h"
#include "openglProgram.h"
#include "openglRasterizerState.h"
#include "openglRenderContext.h"

#include <cassert>

namespace wntr {

	openglRenderContext::openglRenderContext(unsigned width, unsigned height, bool vsync, bool fullscreen)
	: RenderContext(width, height, vsync, fullscreen, API::OPENGL) {

	}

	void openglRenderContext::clear(float red, float green, float blue, float alpha, float depth, unsigned stencil) {
		glClearColor(red, green, blue, alpha);
		glClearDepth(depth);
		glClearStencil(stencil);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void openglRenderContext::clearColor(float red, float green, float blue, float alpha) {
		glClearColor(red, green, blue, alpha);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void openglRenderContext::clearDepth(float depth) {
		glClearDepth(depth);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void openglRenderContext::clearStencil(unsigned stencil) {
		glClearStencil(stencil);
		glClear(GL_STENCIL_BUFFER_BIT);
	}

	void openglRenderContext::draw() {
		if (!_program)
			return;
		if (!_attribute_array)
			return;

		GLenum polygon;
		switch (_attribute_array->getPolygonType()) {
		case POLYGON_TYPE::POINT:
			polygon = GL_POINTS;
			break;
		case POLYGON_TYPE::LINE:
			polygon = GL_LINES;
			break;
		case POLYGON_TYPE::TRIANGLE:
			polygon = GL_TRIANGLES;
			break;
		default:
			throw std::logic_error("error: openglRenderContext::draw: encountered an unsupported POLYGON_TYPE");
		}

		auto element_buffer = _attribute_array->getAttribute(ATTRIBUTE_ELEMENTS);
		if (element_buffer) {
			GLenum format;
			switch (element_buffer->getFormat()) {
			case FORMAT::BYTE:
				format = GL_BYTE;
				break;
			case FORMAT::UBYTE:
				format = GL_UNSIGNED_BYTE;
				break;
			case FORMAT::SHORT:
				format = GL_SHORT;
				break;
			case FORMAT::USHORT:
				format = GL_UNSIGNED_SHORT;
				break;
			case FORMAT::INT:
				format = GL_INT;
				break;
			case FORMAT::UINT:
				format = GL_UNSIGNED_INT;
				break;
			default:
				throw std::logic_error("error: openglRenderContext::draw: encountered an unsupported data format");
			}

			glDrawElements(polygon, element_buffer->getCount(), format, (void*)0);
		}
		else {
			auto attribute = _attribute_array->getAttributeByIndex(0);
			if (attribute) {
				glDrawArrays(polygon, 0, attribute->getCount());
			}
		}
	}

	void openglRenderContext::drawInstances(unsigned count) {
		if (count == 0)
			return;
        
		if (!_program)
			return;
		if (!_attribute_array)
			return;
        
		GLenum polygon;
		switch (_attribute_array->getPolygonType()) {
            case POLYGON_TYPE::POINT:
                polygon = GL_POINTS;
                break;
            case POLYGON_TYPE::LINE:
                polygon = GL_LINES;
                break;
            case POLYGON_TYPE::TRIANGLE:
                polygon = GL_TRIANGLES;
                break;
            default:
                throw std::logic_error("error: openglRenderContext::draw: encountered an unsupported POLYGON_TYPE");
		}
        
		auto element_buffer = _attribute_array->getAttribute(ATTRIBUTE_ELEMENTS);
		if (element_buffer) {
			GLenum format;
			switch (element_buffer->getFormat()) {
                case FORMAT::BYTE:
                    format = GL_BYTE;
                    break;
                case FORMAT::UBYTE:
                    format = GL_UNSIGNED_BYTE;
                    break;
                case FORMAT::SHORT:
                    format = GL_SHORT;
                    break;
                case FORMAT::USHORT:
                    format = GL_UNSIGNED_SHORT;
                    break;
                case FORMAT::INT:
                    format = GL_INT;
                    break;
                case FORMAT::UINT:
                    format = GL_UNSIGNED_INT;
                    break;
                default:
                    throw std::logic_error("error: openglRenderContext::draw: encountered an unsupported data format");
			}
            
			glDrawElementsInstanced(polygon, element_buffer->getCount(), format, (void*)0, count);
		}
		else {
			auto attribute = _attribute_array->getAttributeByIndex(0);
			if (attribute) {
				glDrawArraysInstanced(polygon, 0, attribute->getCount(), count);
			}
		}
	}

	void openglRenderContext::setFramebuffer(const std::shared_ptr<Framebuffer>& framebuffer) {
        if (framebuffer) {
            if (framebuffer->getApi() != _api)
                throw std::invalid_argument("error: openglRenderContext::setFramebuffer: expected Framebuffer to implement API::OPENGL");
            
            _framebuffer = std::static_pointer_cast<openglFramebuffer>(framebuffer);
            glViewport(0, 0, _framebuffer->getMaxWidth(), _framebuffer->getMaxHeight());
            glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer->getOpenGLId());
        }
        else {
            _framebuffer = nullptr;
            glViewport(0, 0, _width, _height);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
	}

	void openglRenderContext::setProgram(const std::shared_ptr<Program>& program) {
        assert(program);
        
        if (program->getApi() != _api)
            throw std::invalid_argument("error: openglRenderContext::setProgram: expected Program to implement API::OPENGL");
        if (program.get() == _program.get())
            return;
        
        if (_program)
            _program->unbindAllShaderConstants(this);

        _program = std::static_pointer_cast<openglProgram>(program);

		glUseProgram(_program->getOpenGLId());

        _program->bindAllShaderConstants(this);		

		if (_attribute_array)
			_attribute_array->align(_program.get(), this);
	}

	void openglRenderContext::setAttributeArray(const std::shared_ptr<AttributeArray>& attribute_array) {
        if (!attribute_array) {
            _attribute_array = nullptr;
            glBindVertexArray(0);
            return;
        }
        
        if (attribute_array->getApi() != _api)
            throw std::invalid_argument("error: openglRenderContext::setAttributeArray: expected AttributeArray to implement API::OPENGL");
        if (attribute_array.get() == _attribute_array.get())
            return;
        
		if (_program)
			attribute_array->align(_program.get(), this);

        _attribute_array = std::static_pointer_cast<openglAttributeArray>(attribute_array);
        glBindVertexArray(_attribute_array->getOpenGLId());
	}

	void openglRenderContext::setBlendState(const std::shared_ptr<BlendState>& state) {
        if (state) {
            if (state->getApi() != _api)
                throw std::invalid_argument("error: openglRenderContext::setBlendState: expected BlendState to implement API::OPENGL");
            
            _blend_state = std::static_pointer_cast<openglBlendState>(state);
            auto config = _blend_state->getConfig();
            
            config.blend_enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
            config.alpha_to_coverage ? glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE) : glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
            
            GLenum op;
            GLenum op_alpha;
            switch (config.op) {
                case BLEND_OP::ADD:
                    op = GL_FUNC_ADD;
                    break;
                case BLEND_OP::SUB:
                    op = GL_FUNC_SUBTRACT;
                    break;
                case BLEND_OP::REV_SUB:
                    op = GL_FUNC_REVERSE_SUBTRACT;
                    break;
                case BLEND_OP::MIN:
                    op = GL_MIN;
                    break;
                case BLEND_OP::MAX:
                    op = GL_MAX;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setBlendState: encountered an unsupported BLEND_OP");
            }
            switch (config.op_alpha) {
                case BLEND_OP::ADD:
                    op_alpha = GL_FUNC_ADD;
                    break;
                case BLEND_OP::SUB:
                    op_alpha = GL_FUNC_SUBTRACT;
                    break;
                case BLEND_OP::REV_SUB:
                    op_alpha = GL_FUNC_REVERSE_SUBTRACT;
                    break;
                case BLEND_OP::MIN:
                    op_alpha = GL_MIN;
                    break;
                case BLEND_OP::MAX:
                    op_alpha = GL_MAX;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setBlendState: encountered an unsupported BLEND_OP");
            }
            glBlendEquationSeparate(op, op_alpha);
            
            GLenum src;
            GLenum dst;
            GLenum src_alpha;
            GLenum dst_alpha;
            switch (config.src) {
                case BLEND_FUNC::ZERO:
                    src = GL_ZERO;
                    break;
                case BLEND_FUNC::ONE:
                    src = GL_ONE;
                    break;
                case BLEND_FUNC::SRC_COLOR:
                    src = GL_SRC_COLOR;
                    break;
                case BLEND_FUNC::INV_SRC_COLOR:
                    src = GL_ONE_MINUS_SRC_COLOR;
                    break;
                case BLEND_FUNC::SRC_ALPHA:
                    src = GL_SRC_ALPHA;
                    break;
                case BLEND_FUNC::INV_SRC_ALPHA:
                    src = GL_ONE_MINUS_SRC_ALPHA;
                    break;
                case BLEND_FUNC::DEST_ALPHA:
                    src = GL_DST_ALPHA;
                    break;
                case BLEND_FUNC::INV_DEST_ALPHA:
                    src = GL_ONE_MINUS_DST_ALPHA;
                    break;
                case BLEND_FUNC::DEST_COLOR:
                    src = GL_DST_COLOR;
                    break;
                case BLEND_FUNC::INV_DEST_COLOR:
                    src = GL_ONE_MINUS_DST_COLOR;
                    break;
                case BLEND_FUNC::SRC_ALPHA_SAT:
                    src = GL_SRC_ALPHA_SATURATE;
                    break;
                case BLEND_FUNC::BLEND_FACTOR:
                    src = GL_CONSTANT_COLOR;
                    break;
                case BLEND_FUNC::INV_BLEND_FACTOR:
                    src = GL_ONE_MINUS_CONSTANT_COLOR;
                    break;
                case BLEND_FUNC::SRC1_COLOR:
                    src = GL_SRC1_COLOR;
                    break;
                case BLEND_FUNC::INV_SRC1_COLOR:
                    src = GL_ONE_MINUS_SRC1_COLOR;
                    break;
                case BLEND_FUNC::SRC1_ALPHA:
                    src = GL_SRC1_ALPHA;
                    break;
                case BLEND_FUNC::INV_SRC1_ALPHA:
                    src = GL_ONE_MINUS_SRC1_ALPHA;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setBlendState: encountered an unsupported BLEND_FUNC");
            }
            switch (config.dst) {
                case BLEND_FUNC::ZERO:
                    dst = GL_ZERO;
                    break;
                case BLEND_FUNC::ONE:
                    dst = GL_ONE;
                    break;
                case BLEND_FUNC::SRC_COLOR:
                    dst = GL_SRC_COLOR;
                    break;
                case BLEND_FUNC::INV_SRC_COLOR:
                    dst = GL_ONE_MINUS_SRC_COLOR;
                    break;
                case BLEND_FUNC::SRC_ALPHA:
                    dst = GL_SRC_ALPHA;
                    break;
                case BLEND_FUNC::INV_SRC_ALPHA:
                    dst = GL_ONE_MINUS_SRC_ALPHA;
                    break;
                case BLEND_FUNC::DEST_ALPHA:
                    dst = GL_DST_ALPHA;
                    break;
                case BLEND_FUNC::INV_DEST_ALPHA:
                    dst = GL_ONE_MINUS_DST_ALPHA;
                    break;
                case BLEND_FUNC::DEST_COLOR:
                    dst = GL_DST_COLOR;
                    break;
                case BLEND_FUNC::INV_DEST_COLOR:
                    dst = GL_ONE_MINUS_DST_COLOR;
                    break;
                case BLEND_FUNC::SRC_ALPHA_SAT:
                    dst = GL_SRC_ALPHA_SATURATE;
                    break;
                case BLEND_FUNC::BLEND_FACTOR:
                    dst = GL_CONSTANT_COLOR;
                    break;
                case BLEND_FUNC::INV_BLEND_FACTOR:
                    dst = GL_ONE_MINUS_CONSTANT_COLOR;
                    break;
                case BLEND_FUNC::SRC1_COLOR:
                    dst = GL_SRC1_COLOR;
                    break;
                case BLEND_FUNC::INV_SRC1_COLOR:
                    dst = GL_ONE_MINUS_SRC1_COLOR;
                    break;
                case BLEND_FUNC::SRC1_ALPHA:
                    dst = GL_SRC1_ALPHA;
                    break;
                case BLEND_FUNC::INV_SRC1_ALPHA:
                    dst = GL_ONE_MINUS_SRC1_ALPHA;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setBlendState: encountered an unsupported BLEND_FUNC");
            }
            switch (config.src_alpha) {
                case BLEND_FUNC::ZERO:
                    src_alpha = GL_ZERO;
                    break;
                case BLEND_FUNC::ONE:
                    src_alpha = GL_ONE;
                    break;
                case BLEND_FUNC::SRC_COLOR:
                    src_alpha = GL_SRC_COLOR;
                    break;
                case BLEND_FUNC::INV_SRC_COLOR:
                    src_alpha = GL_ONE_MINUS_SRC_COLOR;
                    break;
                case BLEND_FUNC::SRC_ALPHA:
                    src_alpha = GL_SRC_ALPHA;
                    break;
                case BLEND_FUNC::INV_SRC_ALPHA:
                    src_alpha = GL_ONE_MINUS_SRC_ALPHA;
                    break;
                case BLEND_FUNC::DEST_ALPHA:
                    src_alpha = GL_DST_ALPHA;
                    break;
                case BLEND_FUNC::INV_DEST_ALPHA:
                    src_alpha = GL_ONE_MINUS_DST_ALPHA;
                    break;
                case BLEND_FUNC::DEST_COLOR:
                    src_alpha = GL_DST_COLOR;
                    break;
                case BLEND_FUNC::INV_DEST_COLOR:
                    src_alpha = GL_ONE_MINUS_DST_COLOR;
                    break;
                case BLEND_FUNC::SRC_ALPHA_SAT:
                    src_alpha = GL_SRC_ALPHA_SATURATE;
                    break;
                case BLEND_FUNC::BLEND_FACTOR:
                    src_alpha = GL_CONSTANT_COLOR;
                    break;
                case BLEND_FUNC::INV_BLEND_FACTOR:
                    src_alpha = GL_ONE_MINUS_CONSTANT_COLOR;
                    break;
                case BLEND_FUNC::SRC1_COLOR:
                    src_alpha = GL_SRC1_COLOR;
                    break;
                case BLEND_FUNC::INV_SRC1_COLOR:
                    src_alpha = GL_ONE_MINUS_SRC1_COLOR;
                    break;
                case BLEND_FUNC::SRC1_ALPHA:
                    src_alpha = GL_SRC1_ALPHA;
                    break;
                case BLEND_FUNC::INV_SRC1_ALPHA:
                    src_alpha = GL_ONE_MINUS_SRC1_ALPHA;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setBlendState: encountered an unsupported BLEND_FUNC");
            }
            switch (config.dst_alpha) {
                case BLEND_FUNC::ZERO:
                    dst_alpha = GL_ZERO;
                    break;
                case BLEND_FUNC::ONE:
                    dst_alpha = GL_ONE;
                    break;
                case BLEND_FUNC::SRC_COLOR:
                    dst_alpha = GL_SRC_COLOR;
                    break;
                case BLEND_FUNC::INV_SRC_COLOR:
                    dst_alpha = GL_ONE_MINUS_SRC_COLOR;
                    break;
                case BLEND_FUNC::SRC_ALPHA:
                    dst_alpha = GL_SRC_ALPHA;
                    break;
                case BLEND_FUNC::INV_SRC_ALPHA:
                    dst_alpha = GL_ONE_MINUS_SRC_ALPHA;
                    break;
                case BLEND_FUNC::DEST_ALPHA:
                    dst_alpha = GL_DST_ALPHA;
                    break;
                case BLEND_FUNC::INV_DEST_ALPHA:
                    dst_alpha = GL_ONE_MINUS_DST_ALPHA;
                    break;
                case BLEND_FUNC::DEST_COLOR:
                    dst_alpha = GL_DST_COLOR;
                    break;
                case BLEND_FUNC::INV_DEST_COLOR:
                    dst_alpha = GL_ONE_MINUS_DST_COLOR;
                    break;
                case BLEND_FUNC::SRC_ALPHA_SAT:
                    dst_alpha = GL_SRC_ALPHA_SATURATE;
                    break;
                case BLEND_FUNC::BLEND_FACTOR:
                    dst_alpha = GL_CONSTANT_COLOR;
                    break;
                case BLEND_FUNC::INV_BLEND_FACTOR:
                    dst_alpha = GL_ONE_MINUS_CONSTANT_COLOR;
                    break;
                case BLEND_FUNC::SRC1_COLOR:
                    dst_alpha = GL_SRC1_COLOR;
                    break;
                case BLEND_FUNC::INV_SRC1_COLOR:
                    dst_alpha = GL_ONE_MINUS_SRC1_COLOR;
                    break;
                case BLEND_FUNC::SRC1_ALPHA:
                    dst_alpha = GL_SRC1_ALPHA;
                    break;
                case BLEND_FUNC::INV_SRC1_ALPHA:
                    dst_alpha = GL_ONE_MINUS_SRC1_ALPHA;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setBlendState: encountered an unsupported BLEND_FUNC");
            }
            glBlendFuncSeparate(src, dst, src_alpha, dst_alpha);
            glColorMask(config.color_write_mask >> 24,
                        config.color_write_mask >> 16,
                        config.color_write_mask >> 8,
                        config.color_write_mask);
        }
        else {
            _blend_state = nullptr;
            glDisable(GL_BLEND);
            glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
            glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
            glColorMask(0xFF,
                        0xFF,
                        0xFF,
                        0xFF);
        }
	}

	void openglRenderContext::setDepthStencilState(const std::shared_ptr<DepthStencilState>& state) {
        if (state) {
            if (state->getApi() != _api)
                throw std::invalid_argument("error: openglRenderContext::setDepthStencilState: expected DepthStencilState to implement API::OPENGL");
            
            _depth_stencil_state = std::static_pointer_cast<openglDepthStencilState>(state);
            auto config = _depth_stencil_state->getConfig();
            
            // configure depth state
            config.depth_enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
            glDepthMask(config.depth_write_mask);
            switch (config.depth_comparison) {
                case COMPARISON::EQUAL:
                    glDepthFunc(GL_EQUAL);
                    break;
                case COMPARISON::NOT_EQUAL:
                    glDepthFunc(GL_NOTEQUAL);
                    break;
                case COMPARISON::LESSER:
                    glDepthFunc(GL_LESS);
                    break;
                case COMPARISON::GREATER:
                    glDepthFunc(GL_GREATER);
                    break;
                case COMPARISON::LESSER_EQUAL:
                    glDepthFunc(GL_LEQUAL);
                    break;
                case COMPARISON::GREATER_EQUAL:
                    glDepthFunc(GL_GEQUAL);
                    break;
                case COMPARISON::ALWAYS:
                    glDepthFunc(GL_ALWAYS);
                    break;
                case COMPARISON::NEVER:
                    glDepthFunc(GL_NEVER);
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setDepthStencilState: encountered an unsupported COMPARISON");
            }
            
            // configure stencil state
            config.stencil_enabled ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
            glStencilMask(config.stencil_write_mask);
            GLenum spass_dpass;
            GLenum spass_dfail;
            GLenum sfail;
            GLenum comparison;
            switch (config.stencil_op_front.spass_dpass) {
                case STENCIL_OP::KEEP:
                    spass_dpass = GL_KEEP;
                    break;
                case STENCIL_OP::ZERO:
                    spass_dpass = GL_ZERO;
                    break;
                case STENCIL_OP::REPLACE:
                    spass_dpass = GL_REPLACE;
                    break;
                case STENCIL_OP::INC_SAT:
                    spass_dpass = GL_INCR;
                    break;
                case STENCIL_OP::DEC_SAT:
                    spass_dpass = GL_DECR;
                    break;
                case STENCIL_OP::INVERT:
                    spass_dpass = GL_INVERT;
                    break;
                case STENCIL_OP::INC:
                    spass_dpass = GL_INCR_WRAP;
                    break;
                case STENCIL_OP::DEC:
                    spass_dpass = GL_DECR_WRAP;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setDepthStencilState: encountered an unsupported STENCIL_OP");
            }
            switch (config.stencil_op_front.spass_dfail) {
                case STENCIL_OP::KEEP:
                    spass_dfail = GL_KEEP;
                    break;
                case STENCIL_OP::ZERO:
                    spass_dfail = GL_ZERO;
                    break;
                case STENCIL_OP::REPLACE:
                    spass_dfail = GL_REPLACE;
                    break;
                case STENCIL_OP::INC_SAT:
                    spass_dfail = GL_INCR;
                    break;
                case STENCIL_OP::DEC_SAT:
                    spass_dfail = GL_DECR;
                    break;
                case STENCIL_OP::INVERT:
                    spass_dfail = GL_INVERT;
                    break;
                case STENCIL_OP::INC:
                    spass_dfail = GL_INCR_WRAP;
                    break;
                case STENCIL_OP::DEC:
                    spass_dfail = GL_DECR_WRAP;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setDepthStencilState: encountered an unsupported STENCIL_OP");
            }
            switch (config.stencil_op_front.sfail) {
                case STENCIL_OP::KEEP:
                    sfail = GL_KEEP;
                    break;
                case STENCIL_OP::ZERO:
                    sfail = GL_ZERO;
                    break;
                case STENCIL_OP::REPLACE:
                    sfail = GL_REPLACE;
                    break;
                case STENCIL_OP::INC_SAT:
                    sfail = GL_INCR;
                    break;
                case STENCIL_OP::DEC_SAT:
                    sfail = GL_DECR;
                    break;
                case STENCIL_OP::INVERT:
                    sfail = GL_INVERT;
                    break;
                case STENCIL_OP::INC:
                    sfail = GL_INCR_WRAP;
                    break;
                case STENCIL_OP::DEC:
                    sfail = GL_DECR_WRAP;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setDepthStencilState: encountered an unsupported STENCIL_OP");
            }
            switch (config.stencil_op_front.comparison) {
                case COMPARISON::EQUAL:
                    comparison = GL_EQUAL;
                    break;
                case COMPARISON::NOT_EQUAL:
                    comparison = GL_NOTEQUAL;
                    break;
                case COMPARISON::LESSER:
                    comparison = GL_LESS;
                    break;
                case COMPARISON::GREATER:
                    comparison = GL_GREATER;
                    break;
                case COMPARISON::LESSER_EQUAL:
                    comparison = GL_LEQUAL;
                    break;
                case COMPARISON::GREATER_EQUAL:
                    comparison = GL_GEQUAL;
                    break;
                case COMPARISON::ALWAYS:
                    comparison = GL_ALWAYS;
                    break;
                case COMPARISON::NEVER:
                    comparison = GL_NEVER;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setDepthStencilState: encountered an unsupported COMPARISON");
            }
            glStencilOpSeparate(GL_FRONT, sfail, spass_dfail, spass_dpass);
            glStencilFuncSeparate(GL_FRONT, comparison, config.stencil_ref, config.stencil_read_mask);
            
            switch (config.stencil_op_back.spass_dpass) {
                case STENCIL_OP::KEEP:
                    spass_dpass = GL_KEEP;
                    break;
                case STENCIL_OP::ZERO:
                    spass_dpass = GL_ZERO;
                    break;
                case STENCIL_OP::REPLACE:
                    spass_dpass = GL_REPLACE;
                    break;
                case STENCIL_OP::INC_SAT:
                    spass_dpass = GL_INCR;
                    break;
                case STENCIL_OP::DEC_SAT:
                    spass_dpass = GL_DECR;
                    break;
                case STENCIL_OP::INVERT:
                    spass_dpass = GL_INVERT;
                    break;
                case STENCIL_OP::INC:
                    spass_dpass = GL_INCR_WRAP;
                    break;
                case STENCIL_OP::DEC:
                    spass_dpass = GL_DECR_WRAP;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setDepthStencilState: encountered an unsupported STENCIL_OP");
            }
            switch (config.stencil_op_back.spass_dfail) {
                case STENCIL_OP::KEEP:
                    spass_dfail = GL_KEEP;
                    break;
                case STENCIL_OP::ZERO:
                    spass_dfail = GL_ZERO;
                    break;
                case STENCIL_OP::REPLACE:
                    spass_dfail = GL_REPLACE;
                    break;
                case STENCIL_OP::INC_SAT:
                    spass_dfail = GL_INCR;
                    break;
                case STENCIL_OP::DEC_SAT:
                    spass_dfail = GL_DECR;
                    break;
                case STENCIL_OP::INVERT:
                    spass_dfail = GL_INVERT;
                    break;
                case STENCIL_OP::INC:
                    spass_dfail = GL_INCR_WRAP;
                    break;
                case STENCIL_OP::DEC:
                    spass_dfail = GL_DECR_WRAP;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setDepthStencilState: encountered an unsupported STENCIL_OP");
            }
            switch (config.stencil_op_back.sfail) {
                case STENCIL_OP::KEEP:
                    sfail = GL_KEEP;
                    break;
                case STENCIL_OP::ZERO:
                    sfail = GL_ZERO;
                    break;
                case STENCIL_OP::REPLACE:
                    sfail = GL_REPLACE;
                    break;
                case STENCIL_OP::INC_SAT:
                    sfail = GL_INCR;
                    break;
                case STENCIL_OP::DEC_SAT:
                    sfail = GL_DECR;
                    break;
                case STENCIL_OP::INVERT:
                    sfail = GL_INVERT;
                    break;
                case STENCIL_OP::INC:
                    sfail = GL_INCR_WRAP;
                    break;
                case STENCIL_OP::DEC:
                    sfail = GL_DECR_WRAP;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setDepthStencilState: encountered an unsupported STENCIL_OP");
            }
            switch (config.stencil_op_back.comparison) {
                case COMPARISON::EQUAL:
                    comparison = GL_EQUAL;
                    break;
                case COMPARISON::NOT_EQUAL:
                    comparison = GL_NOTEQUAL;
                    break;
                case COMPARISON::LESSER:
                    comparison = GL_LESS;
                    break;
                case COMPARISON::GREATER:
                    comparison = GL_GREATER;
                    break;
                case COMPARISON::LESSER_EQUAL:
                    comparison = GL_LEQUAL;
                    break;
                case COMPARISON::GREATER_EQUAL:
                    comparison = GL_GEQUAL;
                    break;
                case COMPARISON::ALWAYS:
                    comparison = GL_ALWAYS;
                    break;
                case COMPARISON::NEVER:
                    comparison = GL_NEVER;
                    break;
                default:
                    throw std::invalid_argument("error: openglRenderContext::setDepthStencilState: encountered an unsupported COMPARISON");
            }
            glStencilOpSeparate(GL_BACK, sfail, spass_dfail, spass_dpass);
            glStencilFuncSeparate(GL_BACK, comparison, config.stencil_ref, config.stencil_read_mask);
        }
        else {
            _depth_stencil_state = nullptr;
            glDisable(GL_DEPTH_TEST);
            glDepthMask(0xFF);
            glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_KEEP);
            glStencilFuncSeparate(GL_FRONT, GL_ALWAYS, 0, 0xFF);
            glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
            glStencilFuncSeparate(GL_BACK, GL_ALWAYS, 0, 0xFF);
        }
	}

	void openglRenderContext::setRasterizerState(const std::shared_ptr<RasterizerState>& state) {

	}

	std::shared_ptr<Framebuffer> openglRenderContext::getFramebuffer() const {
		return _framebuffer;
	}

	std::shared_ptr<Program> openglRenderContext::getProgram() const {
		return _program;
	}

	std::shared_ptr<AttributeArray> openglRenderContext::getAttributeArray() const {
		return _attribute_array;
	}

	std::shared_ptr<BlendState> openglRenderContext::getBlendState() const {
		return _blend_state;
	}

	std::shared_ptr<DepthStencilState> openglRenderContext::getDepthStencilState() const {
		return _depth_stencil_state;
	}

	std::shared_ptr<RasterizerState> openglRenderContext::getRasterizerState() const {
		return _rasterizer_state;
	}

}
