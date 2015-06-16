/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include <cassert>
#include <string>

#include <common/Entity.h>
#include <common/Transform2D.h>

#include <graphics/AttributeArray.h>
#include <graphics/AttributeBuffer.h>
#include <graphics/BlendState.h>
#include <graphics/DepthStencilState.h>
#include <graphics/FragmentShader.h>
#include <graphics/Program.h>
#include <graphics/RenderContext.h>
#include <graphics/Sampler.h>
#include <graphics/Uniform.h>
#include <graphics/VertexShader.h>

#include "UIBitmapPass.h"

#include "Bitmap.h"

namespace wntr {

	std::shared_ptr<UIBitmapPass> UIBitmapPass::Create(const RenderContext* context) {
		return std::make_shared<UIBitmapPass>(context);
	}

	UIBitmapPass::UIBitmapPass(const RenderContext* context)
	: RenderPass() {
		createAttributeArray(context);
		createProgram(context);
	}

	void UIBitmapPass::drawBatch(RenderContext* context) {
		for (const auto& keys : _batch) {
			// set instance transformations
			setUniform(UNIFORM_INSTANCE_WORLD,
					   &keys.second[0],
				       (unsigned)std::min(sizeof(glm::mat4)*keys.second.size(), sizeof(glm::mat4) * 1024),
				       context);
			// get texture
			auto texture = keys.first;
			// bind texture
			setTexture(TEXTURE_DIFFUSE, texture, context);
			// draw instances
			assert(context);
			context->setAttributeArray(_attribute_array);
			context->drawInstances((unsigned)std::min((long)keys.second.size(), (long)1024));
		}
		// empty the instance map, ready for the next batch
		_batch.clear();
	}

	void UIBitmapPass::addToBatch(const std::shared_ptr<Entity>& entity, const glm::mat4& world, RenderContext* context) {
		assert(entity);

		// make sure the entity is transformable
		auto transform = entity->get<Transform2D>();
		if (!transform)
			return;

		// make sure the entity contains a bitmap
		auto bitmap = entity->get<Bitmap>();
		if (!bitmap)
			return;

		// create a matrix that has been scaled to represent the size of the bitmap
		glm::mat4 scaled_world = world * glm::scale(glm::vec3(bitmap->getWidth(), bitmap->getHeight(), 1.0f));

		// add this matrix to the batch
		auto it = _batch.find(bitmap->getTexture());
		if (it == _batch.end())
			_batch[bitmap->getTexture()] = { scaled_world };
		else
			it->second.push_back(scaled_world);
	}

	void UIBitmapPass::createAttributeArray(const RenderContext* context) {

		float positions[] = {
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
		};

		float uvs[] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
		};

		unsigned elements[] = {
			0, 1, 2,
			2, 3, 0,
		};

		auto position_buffer = AttributeBuffer::Create(positions, 4, ATTRIBUTE_POSITION, FORMAT::FLOAT3, USAGE::DEFAULT, context); assert(position_buffer);
		auto uv_buffer       = AttributeBuffer::Create(uvs, 4, ATTRIBUTE_UV, FORMAT::FLOAT2, USAGE::DEFAULT, context);             assert(uv_buffer);
		auto element_buffer  = AttributeBuffer::Create(elements, 6, ATTRIBUTE_ELEMENTS, FORMAT::UINT, USAGE::DEFAULT, context);    assert(element_buffer);

		_attribute_array = AttributeArray::Create(POLYGON_TYPE::TRIANGLE, context); assert(_attribute_array);
		_attribute_array->setAttribute(position_buffer);
		_attribute_array->setAttribute(uv_buffer);
		_attribute_array->setAttribute(element_buffer);
	}

	void UIBitmapPass::createProgram(const RenderContext* context) {

		std::string vs_code =
			"#version 150\n"

			"layout(std140) uniform uniform_world {\n"
			"	mat4 world[1024];\n"
			"};\n"

			"layout(std140) uniform uniform_projection {\n"
			"	mat4 projection;\n"
			"};\n"

			"in vec3 attribute_position;\n"
			"in vec2 attribute_uv;\n"
			"out vec2 uv;\n"

			"void main () {\n"
			"	gl_Position = projection*world[gl_InstanceID]*vec4(attribute_position, 1.0f);\n"
			"   uv = attribute_uv;\n"
			"}\n";
		auto vs = VertexShader::Create(vs_code, context); assert(vs);

		std::string fs_code =
			"#version 150\n"

			"in vec2 uv;\n"

			"out vec4 out_color;\n"

			"uniform sampler2D texture_diffuse;\n"

			"void main () {\n"
			"	out_color = texture(texture_diffuse, uv);\n"
			"}\n";
		auto fs = FragmentShader::Create(fs_code, context); assert(fs);

		_program = Program::Create(vs, nullptr, fs, context); assert(_program);
		_program->linkAttribute("attribute_position", ATTRIBUTE_POSITION);
		_program->linkAttribute("attribute_uv", ATTRIBUTE_UV);
		_program->linkUniform("uniform_world", UNIFORM_INSTANCE_WORLD);
		_program->linkUniform("uniform_projection", UNIFORM_PROJECTION);
		_program->linkTexture("texture_diffuse", TEXTURE_DIFFUSE);
		_program->linkSampler("texture_diffuse", SAMPLER_DIFFUSE);
		_program->link(context);

		BlendConfig blend_config;
		blend_config.blend_enabled     = true;
		blend_config.alpha_to_coverage = false;
		blend_config.src = BLEND_FUNC::SRC_ALPHA;
		blend_config.dst = BLEND_FUNC::INV_SRC_ALPHA;
		blend_config.op  = BLEND_OP::ADD;
		blend_config.src_alpha = BLEND_FUNC::SRC_ALPHA;
		blend_config.dst_alpha = BLEND_FUNC::INV_SRC_ALPHA;
		blend_config.op_alpha  = BLEND_OP::ADD;
		_blend_state = BlendState::Create(blend_config, context); assert(_blend_state);

		DepthStencilConfig depth_stencil_config;
		depth_stencil_config.depth_enabled = false;
		_depth_stencil_state = DepthStencilState::Create(depth_stencil_config, context); assert(_depth_stencil_state);

		SamplerConfig sampler_config;
		sampler_config.min_filter = FILTER::LINEAR_MIPMAP_LINEAR;
		sampler_config.mag_filter = FILTER::LINEAR;
		setSampler(SAMPLER_DIFFUSE, Sampler::Create(sampler_config, context), context);

		glm::mat4 world[1024];
		for (unsigned i = 0; i < 1024; i++)
			world[i] = glm::mat4();
		setUniform(UNIFORM_INSTANCE_WORLD, (void*)world, sizeof(glm::mat4)*1024, USAGE::MUTABLE, context);
		setUniform(UNIFORM_PROJECTION, glm::mat4(), context);
	}

}

