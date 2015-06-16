/*
* Copyright Benjamin Wang 2014.
* 
* This source code may not be distributed, or used for any commercial or non-commerical   
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include "TextField.h"

#include <graphics/AttributeArray.h>
#include <graphics/AttributeBuffer.h>
#include <graphics/Font.h>
#include <graphics/RenderContext.h>

#include <iostream>

namespace wntr {

	std::shared_ptr<TextField> TextField::Create(const std::string& text, const std::shared_ptr<Font>& font, const RenderContext* context) {
		return std::make_shared<TextField>(text, font, context);
	}

	TextField::TextField(const std::string& text, const std::shared_ptr<Font>& font, const RenderContext* context) 
	: Component(), _text(text), _textDidChange(true), _font(font) {

		_attribute_array = AttributeArray::Create(POLYGON_TYPE::TRIANGLE, context);
		assert(_attribute_array);

		updateAttributeArray(context);
	}

	const component_hierarchy& TextField::hierarchy() const {
		return _Hierarchy;
	}

	void TextField::updateAttributeArray(const RenderContext* context) {
		if (!_textDidChange)
			return;

		assert(context);

		std::vector<unsigned> elements;
		std::vector<float> positions;
		std::vector<float> uvs;

		_width  = 0.0f;
		_height = _font->getAscender() + _font->getNewLineHeight();
		float x = 0.0f;
		float y = _font->getAscender();
		for (unsigned i = 0; i < _text.length(); i++) {
			char ascii_char = _text.at(i);
			if (ascii_char == '\n') {
				if (x > _width)
					_width = x;
				x = 0.0f;
				y += _font->getNewLineHeight();
				if (y > _height)
					_height = y;
				continue;
			}
			else if (ascii_char < 32 || ascii_char > 126) {
				continue;
			}

			Font::FontChar font_char = _font->getFontChar(ascii_char);
			if (ascii_char != ' ') {
				elements.push_back(positions.size() / 3);     // element
				elements.push_back(positions.size() / 3 + 1); // element
				elements.push_back(positions.size() / 3 + 2); // element
				elements.push_back(positions.size() / 3 + 2); // element
				elements.push_back(positions.size() / 3 + 3); // element
				elements.push_back(positions.size() / 3);     // element
				positions.push_back(x + font_char.bearing_x); // x               // position
				positions.push_back(y - font_char.bearing_y); // y
				positions.push_back(0.0f);                    // z 
				positions.push_back(x + font_char.bearing_x + font_char.width);  // position
				positions.push_back(y - font_char.bearing_y);
				positions.push_back(0.0f);
				positions.push_back(x + font_char.bearing_x + font_char.width);  // position
				positions.push_back(y - font_char.bearing_y + font_char.height);
				positions.push_back(0.0f);
				positions.push_back(x + font_char.bearing_x);                    // position
				positions.push_back(y - font_char.bearing_y + font_char.height);
				positions.push_back(0.0f);
				uvs.push_back(font_char.uv_start[0]); // u // uv
				uvs.push_back(font_char.uv_start[1]); // v
				uvs.push_back(font_char.uv_end[0]);        // uv
				uvs.push_back(font_char.uv_start[1]);
				uvs.push_back(font_char.uv_end[0]);        // uv
				uvs.push_back(font_char.uv_end[1]);
				uvs.push_back(font_char.uv_start[0]);      // uv
				uvs.push_back(font_char.uv_end[1]);
			}

			x += font_char.advance;
			if (x > _width)
				_width = x;
		}

		auto position_buffer = AttributeBuffer::Create(&positions[0], positions.size() / 3, ATTRIBUTE_POSITION, FORMAT::FLOAT3, USAGE::DEFAULT, context);
		auto uv_buffer = AttributeBuffer::Create(&uvs[0], uvs.size() / 2, ATTRIBUTE_UV, FORMAT::FLOAT2, USAGE::DEFAULT, context);
		auto element_buffer = AttributeBuffer::Create(&elements[0], elements.size(), ATTRIBUTE_ELEMENTS, FORMAT::UINT, USAGE::DEFAULT, context);
		assert(position_buffer);
		assert(uv_buffer);
		assert(element_buffer);

		_attribute_array->setAttribute(position_buffer);
		_attribute_array->setAttribute(uv_buffer);
		_attribute_array->setAttribute(element_buffer);

		_textDidChange = false;
	}

	void TextField::updateSize() {
		_width = 0.0f;
		_height = _font->getAscender() + _font->getNewLineHeight();
		float x = 0.0f;
		float y = _font->getAscender();
		for (unsigned i = 0; i < _text.length(); i++) {
			char ascii_char = _text.at(i);
			if (ascii_char == '\n') {
				if (x > _width)
					_width = x;
				x = 0.0f;
				y += _font->getNewLineHeight();
				if (y > _height)
					_height = y;
				continue;
			}
			else if (ascii_char < 32 || ascii_char > 126) {
				continue;
			}

			x += _font->getFontChar(ascii_char).advance;
			if (x > _width)
				_width = x;
		}
	}

	void TextField::setFont(const std::shared_ptr<Font>& font) {
		_font = font;
		_textDidChange = true;
		updateSize();
	}

	void TextField::setText(const std::string& text) {
		_text = text;
		_textDidChange = true;
		updateSize();
	}

	const std::string& TextField::getText() const {
		return _text;
	}

	float TextField::getWidth() const {
		return _width;
	}

	float TextField::getHeight() const {
		return _height;
	}

	const std::shared_ptr<Font>& TextField::getFont() const {
		return _font;
	}

	const std::shared_ptr<AttributeArray>& TextField::getAttributeArray() const {
		return _attribute_array;
	}

	const component_hierarchy TextField::_Hierarchy({ typeid(Component), typeid(TextField) });

}