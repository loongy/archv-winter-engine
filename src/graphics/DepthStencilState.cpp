/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "DepthStencilState.h"
#include "impl-opengl/openglDepthStencilState.h"
#include "impl-opengl/openglRenderContext.h"


namespace wntr {
    
    std::shared_ptr<DepthStencilState> DepthStencilState::Create(const DepthStencilConfig& config, const RenderContext* context) {
        switch (context->getApi()) {
            case API::OPENGL:
                return std::make_shared<openglDepthStencilState>(config, static_cast<const openglRenderContext*>(context));
            default:
                return nullptr;
        }
    }

	DepthStencilConfig::DepthStencilConfig(const DepthStencilConfig& config) {
		depth_enabled = config.depth_enabled;
		depth_write_mask = config.depth_write_mask;
		depth_comparison = config.depth_comparison;
		stencil_enabled = config.stencil_enabled;
		stencil_write_mask = config.stencil_write_mask;
		stencil_read_mask = config.stencil_read_mask;
		stencil_op_front = config.stencil_op_front;
		stencil_op_back = config.stencil_op_back;
		stencil_ref = config.stencil_ref;
	}

	DepthStencilConfig& DepthStencilConfig::operator=(const DepthStencilConfig& config) {
		depth_enabled = config.depth_enabled;
		depth_write_mask = config.depth_write_mask;
		depth_comparison = config.depth_comparison;
		stencil_enabled = config.stencil_enabled;
		stencil_write_mask = config.stencil_write_mask;
		stencil_read_mask = config.stencil_read_mask;
		stencil_op_front = config.stencil_op_front;
		stencil_op_back = config.stencil_op_back;
		stencil_ref = config.stencil_ref;
		return *this;
	}

	DepthStencilConfig::DepthStencilConfig() {
		depth_enabled = true;
		depth_write_mask = 0xFF;
		depth_comparison = COMPARISON::LESSER_EQUAL;
		stencil_enabled = false;
		stencil_write_mask = 0xFF;
		stencil_read_mask = 0xFF;
		stencil_op_front = { STENCIL_OP::KEEP, STENCIL_OP::KEEP, STENCIL_OP::KEEP, COMPARISON::ALWAYS };
		stencil_op_back = { STENCIL_OP::KEEP, STENCIL_OP::KEEP, STENCIL_OP::KEEP, COMPARISON::ALWAYS };
		stencil_ref = 0;
	}

	bool DepthStencilConfig::operator== (const DepthStencilConfig& config) {
		return depth_enabled == config.depth_enabled    &&
			depth_write_mask == config.depth_write_mask &&
			depth_comparison == config.depth_comparison &&
			stencil_enabled == config.stencil_enabled   &&
			stencil_write_mask == config.stencil_write_mask &&
			stencil_read_mask == config.stencil_read_mask   &&
			stencil_op_front.spass_dpass == config.stencil_op_front.spass_dpass &&
			stencil_op_front.spass_dfail == config.stencil_op_front.spass_dfail &&
			stencil_op_front.sfail == config.stencil_op_front.sfail &&
			stencil_op_front.comparison == config.stencil_op_front.comparison &&
			stencil_op_back.spass_dpass == config.stencil_op_back.spass_dpass &&
			stencil_op_back.spass_dfail == config.stencil_op_back.spass_dfail &&
			stencil_op_back.sfail == config.stencil_op_back.sfail &&
			stencil_op_back.comparison == config.stencil_op_back.comparison &&
			stencil_ref == config.stencil_ref;
	}

	DepthStencilState::DepthStencilState(const DepthStencilConfig& config, API api)
	: _config(config), _api(api) {
	}

	const DepthStencilConfig& DepthStencilState::getConfig() const {
		return _config;
	}

	API DepthStencilState::getApi() const {
		return _api;
	}

}