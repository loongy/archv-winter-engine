/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Font.h"
#include "Texture.h"

#include <freetype/ft2build.h>
#include <freetype/freetype.h>

#include <cassert>

namespace wntr {

	std::shared_ptr<Font> Font::Create(const std::string & file, unsigned size, const RenderContext* context) {
		return std::make_shared<Font>(file, size, context);
	}

	Font::Font(const std::string& file, unsigned size, const RenderContext* context) {
		assert(size);
		assert(context);

		static bool free_type_has_been_initialized = false;
		if (!free_type_has_been_initialized) {
			auto err = FT_Init_FreeType(&_FreeType_Library);
			if (err)
				throw std::runtime_error("error: Font::Font: failed to initialize FreeType library");
			free_type_has_been_initialized = true;
		}

		FT_Face font_face;
		if (FT_New_Face(_FreeType_Library, file.c_str(), 0, &font_face))
			throw std::runtime_error("error: Font::Font: failed to create face for font: " + file);
		_face = std::shared_ptr<FT_FaceRec_>(font_face, FT_Done_Face);

		if (FT_Select_Charmap(font_face, FT_ENCODING_UNICODE))
			throw std::runtime_error("error: Font::Font: failed to select character map for font: " + file);

		if (FT_Set_Pixel_Sizes(font_face, size, size))
			throw std::runtime_error("error: Font::Font: failed to set font pixel size for font: " + file);

		int bmp_width = INT_MIN;
		int row_height = INT_MIN;
		int most_above_line = INT_MIN;

		int num_rows = 0;
		for (int y = 0; y < 16; y++) { // maximum of 16 rows
			bool all_chars_done = false;
			int width = 0;

			for (int x = 0; x < 16; x++) { // 16 chars in each row
				int char_index = (y * 16 + x) + 32; // printable chars start at 32
				if (char_index > 126) {
					all_chars_done = true;
					break;
				}

				if (FT_Load_Char(font_face, (char)char_index, FT_LOAD_RENDER))
					throw std::runtime_error("error: Font::Font: failed to load character: " + std::string((char*)&char_index) + " from font: " + file);
				FT_GlyphSlot glyph = font_face->glyph;

				width += glyph->bitmap.width + 2; // 2 padding pixels
				row_height = row_height > (glyph->bitmap.rows + 2) ? row_height : (glyph->bitmap.rows + 2); // 2 padding pixels
				most_above_line = most_above_line > glyph->bitmap_top ? most_above_line : glyph->bitmap_top;
			}

			bmp_width = bmp_width > width ? bmp_width : width;
			num_rows++;

			if (all_chars_done)
				break;
		}

		int bmp_height = num_rows * row_height;

		_ascender = (float)most_above_line;
		_descender = (float)(most_above_line - row_height);
		_new_line_height = (float)row_height;

		// create a bitmap ready to receive the character images
		int bmp_size = bmp_width*bmp_height * 4;
		unsigned char* bmp = new unsigned char[bmp_size];
		for (int i = 0; i < bmp_size; i++)
			bmp[i] = 0;

		int x_base = 0;
		int y_base = 0;
		for (int y = 0; y < num_rows; y++) {
			bool all_chars_done = false;

			for (int x = 0; x < 16; x++) {
				int char_index = (y * 16 + x) + 32;
				if (char_index > 126) {
					all_chars_done = true;
					break;
				}

				if (FT_Load_Char(font_face, (char)char_index, FT_LOAD_RENDER)) {
					delete[] bmp; 
					bmp = nullptr;
					throw std::runtime_error("error: Font::Font: failed to load character: " + std::string((char*)&char_index) + " from font: " + file);
				}
				FT_GlyphSlot glyph = font_face->glyph;

				int base_index = y_base*bmp_width*row_height + x_base;
				for (int j = 0; j < glyph->bitmap.rows; j++) {
					for (int k = 0; k < glyph->bitmap.width; k++) {
						unsigned char color = glyph->bitmap.buffer[j*glyph->bitmap.width + k];

						bmp[(base_index + j*bmp_width + k) * 4 + 0] = color ? 255 : 0;
						bmp[(base_index + j*bmp_width + k) * 4 + 1] = color ? 255 : 0;
						bmp[(base_index + j*bmp_width + k) * 4 + 2] = color ? 255 : 0;
						bmp[(base_index + j*bmp_width + k) * 4 + 3] = color;
					}
				}

				FontChar fchar;
				fchar.ascii = char_index;

				fchar.width = (float)glyph->bitmap.width;
				fchar.height = (float)glyph->bitmap.rows;

				fchar.advance = (float)glyph->advance.x / 64.0f;
				fchar.bearing_x = (float)glyph->bitmap_left;
				fchar.bearing_y = (float)glyph->bitmap_top;

				fchar.uv_start[0] = ((float)(x_base) / (float)bmp_width);
				fchar.uv_end[0] = (fchar.uv_start[0] + (float)(glyph->bitmap.width) / (float)bmp_width);

				fchar.uv_start[1] = ((float)(y_base*row_height) / (float)(bmp_height));
				fchar.uv_end[1] = (fchar.uv_start[1] + (float)(glyph->bitmap.rows) / (float)(bmp_height));

				_characters.push_back(fchar);

				x_base += glyph->bitmap.width + 2;
			}

			if (all_chars_done)
				break;

			x_base = 0;
			y_base++;
		}

		_texture = Texture::Create(bmp, bmp_width, bmp_height, TEXTURE_FORMAT::RGBA_UBYTE, USAGE::IMMUTABLE, context); 
		assert(_texture);

		delete[] bmp; 
		bmp = nullptr;
	}

	float Font::getAscender() const {
		return _ascender;
	}

	float Font::getDescender() const {
		return _descender;
	}

	float Font::getNewLineHeight() const {
		return _new_line_height;
	}

	const std::shared_ptr<Texture>& Font::getTexture() const {
		return _texture;
	}

	const Font::FontChar& Font::getFontChar(char ascii) const {
		assert(ascii >= 32 && ascii <= 126);
		return _characters.at(ascii - 32);
	}

	FT_Library Font::_FreeType_Library = nullptr;
}