/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_TEXTURE_H_
#define _WNTR_TEXTURE_H_

#include "CommonGraphics.h"

#include <memory>

namespace wntr {

	class RenderContext;
	class TextureResource;

	class Texture {
	public:
		static std::shared_ptr<Texture> Create (const void* data, unsigned width, unsigned height, TEXTURE_FORMAT format, USAGE usage, const RenderContext* context);

#ifndef _MSC_VER
		Texture(Texture&&) = default;
		Texture& operator= (Texture&&) = default;
#endif
		Texture(const Texture&) = delete;
		Texture& operator= (const Texture&) = delete;

		explicit Texture(API api);
		virtual ~Texture() = default;

		virtual std::shared_ptr<TextureResource> getResource() const = 0;

		API getApi() const;

	protected:
		const API _api;

	private:

	};

}

#endif