/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_BLENDSTATE_H_
#define _WNTR_BLENDSTATE_H_

#include "CommonGraphics.h"

#include <memory>

namespace wntr {

	class RenderContext;

	class BlendConfig {
	public:
#ifndef _MSC_VER
		BlendConfig(BlendConfig&&) = default;
		BlendConfig& operator= (BlendConfig&&) = default;
#endif
		BlendConfig(const BlendConfig&);
		BlendConfig& operator=(const BlendConfig&);

		explicit BlendConfig();
		~BlendConfig() = default;

		bool operator== (const BlendConfig& config);

		bool       blend_enabled;
		bool       alpha_to_coverage;
		BLEND_FUNC src;
		BLEND_FUNC dst;
		BLEND_OP   op;
		BLEND_FUNC src_alpha;
		BLEND_FUNC dst_alpha;
		BLEND_OP   op_alpha;
		unsigned   color_write_mask;
	};

	class BlendState {
	public:
		static std::shared_ptr<BlendState> Create(const BlendConfig& config, const RenderContext* context);

#ifndef _MSC_VER
		BlendState(BlendState&&) = default;
		BlendState& operator= (BlendState&&) = default;
#endif
		BlendState(const BlendState&) = delete;
		BlendState& operator=(const BlendState&) = delete;

		explicit BlendState(const BlendConfig& config, API api);
		virtual ~BlendState() = default;

		virtual void setConfig(const BlendConfig& config, const RenderContext* context) = 0;

		const BlendConfig& getConfig() const;
		API getApi() const;

	protected:
		BlendConfig _config;
		const API _api;

	private:

	};

}

#endif