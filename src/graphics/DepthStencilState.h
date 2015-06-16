/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_DEPTHSTENCILSTATE_H_
#define _WNTR_DEPTHSTENCILSTATE_H_

#include "CommonGraphics.h"

#include <memory>

namespace wntr {

	class RenderContext;

	class DepthStencilConfig {
	public:
#ifndef _MSC_VER
		DepthStencilConfig(DepthStencilConfig&&) = default;
		DepthStencilConfig& operator= (DepthStencilConfig&&) = default;
#endif
		DepthStencilConfig(const DepthStencilConfig&);
		DepthStencilConfig& operator=(const DepthStencilConfig&);

		explicit DepthStencilConfig();
		~DepthStencilConfig() = default;

		bool operator== (const DepthStencilConfig& config);

		bool         depth_enabled;
		unsigned int depth_write_mask;
		COMPARISON   depth_comparison;

		bool         stencil_enabled;
		unsigned int stencil_read_mask;
		unsigned int stencil_write_mask;
		STENCIL      stencil_op_front;
		STENCIL      stencil_op_back;
		unsigned int stencil_ref;
	};

	class DepthStencilState {
	public:
		static std::shared_ptr<DepthStencilState> Create(const DepthStencilConfig& config, const RenderContext* context);

#ifndef _MSC_VER
		DepthStencilState(DepthStencilState&& _state) = default;
		DepthStencilState& operator= (DepthStencilState&& _state) = default;
#endif
		DepthStencilState(const DepthStencilState&) = delete;
		DepthStencilState& operator=(const DepthStencilState&) = delete;

		explicit DepthStencilState(const DepthStencilConfig& config, API api);
		virtual ~DepthStencilState() = default;

		virtual void setConfig(const DepthStencilConfig& config, const RenderContext* context) = 0;

		const DepthStencilConfig& getConfig() const;
		API getApi(void) const;

	protected:
		DepthStencilConfig _config;
		const API _api;

	private:

	};

}

#endif