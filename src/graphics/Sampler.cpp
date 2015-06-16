/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Sampler.h"
#include "impl-opengl/openglRenderContext.h"
#include "impl-opengl/openglSampler.h"

#include <limits>

namespace wntr {

	SamplerConfig::SamplerConfig(const SamplerConfig& config) {
		min_lod = config.min_lod;
		max_lod = config.max_lod;
		min_filter = config.min_filter;
		mag_filter = config.mag_filter;
		edge_u = config.edge_u;
		edge_v = config.edge_v;
		edge_w = config.edge_w;
		comparison = config.comparison;
		border[0] = config.border[0];
		border[1] = config.border[1];
		border[2] = config.border[2];
		border[3] = config.border[3];
	}

	SamplerConfig& SamplerConfig::operator= (const SamplerConfig& config) {
		min_lod = config.min_lod;
		max_lod = config.max_lod;
		min_filter = config.min_filter;
		mag_filter = config.mag_filter;
		edge_u = config.edge_u;
		edge_v = config.edge_v;
		edge_w = config.edge_w;
		comparison = config.comparison;
		border[0] = config.border[0];
		border[1] = config.border[1];
		border[2] = config.border[2];
		border[3] = config.border[3];
		return *this;
	}

	SamplerConfig::SamplerConfig() {
		min_lod = -std::numeric_limits<float>::max();
		max_lod = std::numeric_limits<float>::max();
		min_filter = FILTER::NEAREST;
		mag_filter = FILTER::NEAREST;
		edge_u = TEXTURE_EDGE::CLAMP;
		edge_v = TEXTURE_EDGE::CLAMP;
		edge_w = TEXTURE_EDGE::CLAMP;
		comparison = COMPARISON::LESSER;
		border[0] = 1.0f;
		border[1] = 1.0f;
		border[2] = 1.0f;
		border[3] = 1.0f;
	}

	bool SamplerConfig::operator==(const SamplerConfig& config) {
		return min_lod == config.min_lod && 
			max_lod == config.max_lod    &&
			min_filter == config.min_filter && 
			mag_filter == config.mag_filter &&
			edge_u == config.edge_u && 
			edge_v == config.edge_v &&
			edge_w == config.edge_w &&
			comparison == config.comparison &&
			border[0] == config.border[0]   && 
			border[1] == config.border[1]   &&
			border[2] == config.border[2]   &&
			border[3] == config.border[3];
	}

	std::shared_ptr<Sampler> Sampler::Create(const SamplerConfig& config, const RenderContext* context) {
		switch (context->getApi()) {
		case API::OPENGL:
			return std::make_shared<openglSampler>(config, static_cast<const openglRenderContext*>(context));
		default:
			return nullptr;
		}
	}

	Sampler::Sampler(const SamplerConfig& config, API api)
	: _config(config), _api(api) {

	}

	const SamplerConfig& Sampler::getConfig() const {
		return _config;
	}

	API Sampler::getApi() const {
		return _api;
	}

}