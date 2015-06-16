/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_LCOMPOSITIONEFFECT_H_
#define _WNTR_LCOMPOSITIONEFFECT_H_

#include "../RenderEffect.h"

namespace wntr {

	class AttributeArray;
	class Uniform;

	class LCompositionEffect : public RenderEffect {
	public:
		static std::shared_ptr<LCompositionEffect> Create(const RenderContext* context);

#ifndef _MSC_VER
		LCompositionEffect(LCompositionEffect&&) = default;
		LCompositionEffect& operator=(LCompositionEffect&&) = default;
#endif
		LCompositionEffect(const LCompositionEffect&) = delete;
		LCompositionEffect& operator=(const LCompositionEffect&) = delete;

		explicit LCompositionEffect(const RenderContext* context);
		virtual ~LCompositionEffect() = default;

	protected:

	private:
        
	};

}

#endif