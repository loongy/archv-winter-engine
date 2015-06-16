/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#ifndef _WNTR_LSPOINTEFFECT_H_
#define _WNTR_LSPOINTEFFECT_H_

#include "../RenderEffect.h"

namespace wntr {
    
    class AttributeArray;

	class LSPointEffect : public RenderEffect {
	public:
		static std::shared_ptr<LSPointEffect> Create(const RenderContext* context);

#ifndef _MSC_VER
		LSPointEffect(LSPointEffect&&) = default;
		LSPointEffect& operator=(LSPointEffect&&) = default;
#endif
		LSPointEffect(const LSPointEffect&) = delete;
		LSPointEffect& operator=(const LSPointEffect&) = delete;

		explicit LSPointEffect(const RenderContext* context);
		virtual ~LSPointEffect() = default;
        
	protected:
        
	private:
        
	};

}

#endif