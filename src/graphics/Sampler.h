/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_SAMPLERSTATE_H_
#define _WNTR_SAMPLERSTATE_H_

#include "CommonGraphics.h"

#include <memory>

namespace wntr {

	class RenderContext;

	class SamplerConfig {
	public:
#ifndef _MSC_VER
		SamplerConfig(SamplerConfig&&) = default;
		SamplerConfig& operator= (SamplerConfig&&) = default;
#endif
		SamplerConfig(const SamplerConfig&);
		SamplerConfig& operator=(const SamplerConfig&);

		explicit SamplerConfig();
		~SamplerConfig() = default;

		bool operator== (const SamplerConfig& config);

		float min_lod;
		float max_lod;

		FILTER min_filter;
		FILTER mag_filter;

		TEXTURE_EDGE edge_u;
		TEXTURE_EDGE edge_v;
		TEXTURE_EDGE edge_w;

		COMPARISON comparison;

		float border[4];
	};

	class Sampler {
	public:
		static std::shared_ptr<Sampler> Create(const SamplerConfig& config, const RenderContext* context);

#ifndef _MSC_VER
		Sampler(Sampler&&) = default;
		Sampler& operator= (Sampler&&) = default;
#endif
		Sampler(const Sampler&) = delete;
		Sampler& operator= (const Sampler&) = delete;

		explicit Sampler(const SamplerConfig& config, API api);
		virtual ~Sampler() = default;

		virtual void setConfig(const SamplerConfig& config, const RenderContext* context) = 0;

		const SamplerConfig& getConfig() const;
		API getApi() const;

	protected:
		SamplerConfig _config;
		const API _api;

	private:

	};

}

#endif