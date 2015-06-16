/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#ifndef _WNTR_LSORTHOEFFECT_H_
#define _WNTR_LSORTHOEFFECT_H_

#include "../RenderEffect.h"

namespace wntr {

	class AttributeArray;

	class LSOrthoEffect : public RenderEffect {
	public:
		static std::shared_ptr<LSOrthoEffect> Create(const RenderContext* context);

#ifndef _MSC_VER
		LSOrthoEffect(LSOrthoEffect&&) = default;
		LSOrthoEffect& operator=(LSOrthoEffect&&) = default;
#endif
		LSOrthoEffect(const LSOrthoEffect&) = delete;
		LSOrthoEffect& operator=(const LSOrthoEffect&) = delete;

		explicit LSOrthoEffect(const RenderContext* context);
		virtual ~LSOrthoEffect() = default;

	protected:

	private:

	};

}

#endif