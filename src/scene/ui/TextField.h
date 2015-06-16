/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#pragma once
#ifndef _WNTR_TEXTFIELD_H_
#define _WNTR_TEXTFIELD_H_

#include <common/Component.h>
#include <common/utility.h>

namespace wntr {

	class AttributeArray;
	class RenderContext;
	class Font;

	class TextField : public Component {
	public:
		static std::shared_ptr<TextField> Create(const std::string& text, const std::shared_ptr<Font>& font, const RenderContext* context);

#ifndef _MSC_VER
		Bitmap(Bitmap&&) = default;
		Bitmap& operator=(Bitmap&&) = default;
#endif
		TextField(const TextField&) = delete;
		TextField& operator=(const TextField&) = delete;

		explicit TextField(const std::string& text, const std::shared_ptr<Font>& font, const RenderContext* context);
		virtual ~TextField() = default;

		virtual const component_hierarchy& hierarchy() const override;

		void updateAttributeArray(const RenderContext* context);
		void updateSize();

		void setFont(const std::shared_ptr<Font>& font);
		void setText(const std::string& text);

		const std::string& getText() const;
		float getWidth() const;
		float getHeight() const;
		const std::shared_ptr<Font>& getFont() const;
		const std::shared_ptr<AttributeArray>& getAttributeArray() const;

	protected:

	private:
		static const component_hierarchy _Hierarchy;

		std::string _text;
		bool _textDidChange;

		float _width;
		float _height;

		std::shared_ptr<Font> _font;
		std::shared_ptr<AttributeArray> _attribute_array;

	};

}

#endif
