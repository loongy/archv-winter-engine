/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_FONT_H_
#define _WNTR_FONT_H_

#include "CommonGraphics.h"

#include <memory>
#include <string>
#include <vector>

struct FT_LibraryRec_;
struct FT_FaceRec_;

namespace wntr {

	class RenderContext;
	class Texture;

	class Font {
	public:
		struct FontChar {
			char ascii;

			float width;
			float height;

			float advance;
			float bearing_x;
			float bearing_y;

			float uv_start[2];
			float uv_end[2];
		};

		static std::shared_ptr<Font> Create(const std::string & file, unsigned size, const RenderContext* context);

#ifndef _MSC_VER
		Font(Font&&) = default;
		Font& operator= (Font&&) = default;
#endif
		Font(const Font&) = delete;
		Font& operator= (const Font&) = delete;

		explicit Font(const std::string& file, unsigned size, const RenderContext* context);
		virtual ~Font() = default;

		float getAscender() const;
		float getDescender() const;
		float getNewLineHeight() const;
		const std::shared_ptr<Texture>& getTexture() const;
		const FontChar& getFontChar(char ascii) const;

	protected:

	private:
		static FT_LibraryRec_* _FreeType_Library;

		std::shared_ptr<FT_FaceRec_> _face;

		float _ascender;
		float _descender;
		float _new_line_height;

		std::vector<FontChar> _characters;
		std::shared_ptr<Texture> _texture;

	};

}

#endif