/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_TEXTURERESOURCE_H_
#define _WNTR_TEXTURERESOURCE_H_

#include "CommonGraphics.h"

namespace wntr {

	class TextureResource {
	public:
#ifndef _MSC_VER
		TextureResource(TextureResource&&) = default;
		TextureResource& operator= (TextureResource&&) = default;
#endif
		TextureResource(const TextureResource&) = delete;
		TextureResource& operator= (const TextureResource&) = delete;

		explicit TextureResource(unsigned width, unsigned height, TEXTURE_FORMAT format, USAGE usage, API api);
		virtual ~TextureResource() = default;

		unsigned getWidth() const;
		unsigned getHeight() const;
		TEXTURE_FORMAT getPixelFormat() const;
		USAGE          getUsage() const;
		API            getApi() const;

	protected:
		unsigned _width;
		unsigned _height;
		const TEXTURE_FORMAT _format;
		const USAGE _usage;
		const API _api;

	private:

	};

}

#endif