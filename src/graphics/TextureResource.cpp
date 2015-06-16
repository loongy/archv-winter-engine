/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "TextureResource.h"

namespace wntr {

	TextureResource::TextureResource(unsigned width, unsigned height, TEXTURE_FORMAT format, USAGE usage, API api)
	: _width(width), _height(height), _format(format), _usage(usage), _api(api) {

	}

	unsigned TextureResource::getWidth() const {
		return _width;
	}

	unsigned TextureResource::getHeight() const {
		return _height;
	}

	TEXTURE_FORMAT TextureResource::getPixelFormat() const {
		return _format;
	}

	USAGE TextureResource::getUsage() const {
		return _usage;
	}

	API TextureResource::getApi() const {
		return _api;
	}

}