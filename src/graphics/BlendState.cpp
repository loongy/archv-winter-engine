/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "BlendState.h"
#include "RenderContext.h"
#include "impl-opengl/openglBlendState.h"
#include "impl-opengl/openglRenderContext.h"

#include <cassert>

namespace wntr {
    
    std::shared_ptr<BlendState> BlendState::Create(const BlendConfig& config, const RenderContext* context) {
        switch (context->getApi()) {
            case API::OPENGL:
                return std::make_shared<openglBlendState>(config, static_cast<const openglRenderContext*>(context));
            default:
                return nullptr;
        }
    }

	BlendConfig::BlendConfig(const BlendConfig& config) {
		blend_enabled = config.blend_enabled;
		alpha_to_coverage = config.alpha_to_coverage;
		src = config.src;
		dst = config.dst;
		op = config.op;
		src_alpha = config.src_alpha;
		dst_alpha = config.dst_alpha;
		op_alpha = config.op_alpha;
		color_write_mask = config.color_write_mask;
	}

	BlendConfig& BlendConfig::operator=(const BlendConfig& config) {
		blend_enabled = config.blend_enabled;
		alpha_to_coverage = config.alpha_to_coverage;
		src = config.src;
		dst = config.dst;
		op = config.op;
		src_alpha = config.src_alpha;
		dst_alpha = config.dst_alpha;
		op_alpha = config.op_alpha;
		color_write_mask = config.color_write_mask;
		return *this;
	}

	BlendConfig::BlendConfig() {
		blend_enabled = false;
		alpha_to_coverage = false;
		src = BLEND_FUNC::ONE;
		dst = BLEND_FUNC::ZERO;
		op = BLEND_OP::ADD;
		src_alpha = BLEND_FUNC::ONE;
		dst_alpha = BLEND_FUNC::ZERO;
		op_alpha = BLEND_OP::ADD;
		color_write_mask = 0xFFFFFFFF;
	}

	bool BlendConfig::operator== (const BlendConfig& config) {
		return blend_enabled == config.blend_enabled      &&
			alpha_to_coverage == config.alpha_to_coverage &&
			src == config.src &&
			dst == config.dst &&
			op == config.op   &&
			src_alpha == config.src_alpha &&
			dst_alpha == config.dst_alpha &&
			op_alpha == config.op_alpha   &&
			color_write_mask == config.color_write_mask;
	}

	BlendState::BlendState(const BlendConfig& config, API api)
	: _config(config), _api(api) {
	}

	const BlendConfig& BlendState::getConfig() const {
		return _config;
	}

	API BlendState::getApi() const {
		return _api;
	}

}
