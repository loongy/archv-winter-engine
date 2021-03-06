/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include <common/Entity.h>
#include <common/Transform3D.h>

#include <graphics/BlendState.h>
#include <graphics/DepthStencilState.h>
#include <graphics/FragmentShader.h>
#include <graphics/Program.h>
#include <graphics/RenderContext.h>
#include <graphics/Uniform.h>
#include <graphics/VertexShader.h>

#include "GSPass.h"

#include "Material.h"
#include "Mesh.h"
#include "MeshCollection.h"

namespace wntr {

	std::shared_ptr<GSPass> GSPass::Create(const RenderContext* context) {
		return std::make_shared<GSPass>(context);
	}

	GSPass::GSPass(const RenderContext* context)
	: RenderPass() {
		createProgram(context);
	}

	void GSPass::drawBatch(RenderContext* context) {
		assert(context);
		// for all materials in the batch
		for (const auto& keys : _batch) {
			// bind the material
			if (keys.first)
				setTexture(TEXTURE_DIFFUSE, keys.first->getTextureDiffuse(), context);
			else
				setTexture(TEXTURE_DIFFUSE, nullptr, context);
			// draw all meshes that use this material
			for (const auto& mat_mesh_pair : keys.second) {
				setUniform(UNIFORM_WORLD, mat_mesh_pair.first, context);
				context->setAttributeArray(mat_mesh_pair.second->getAttributeArray());
				context->draw();
			}
		}
		// empty the instance map, ready for the next batch
		_batch.clear();
	}

	void GSPass::addToBatch(const std::shared_ptr<Entity>& entity, const glm::mat4& world, RenderContext* context) {
		assert(entity);

		// make sure the entity is transformable
		auto transform = entity->get<Transform3D>();
		if (!transform)
			return;

		// if the entity has a mesh
		auto mesh = entity->get<Mesh>();
		if (mesh) {
			// store the mesh
			auto material = mesh->getMaterial();
			auto it = _batch.find(material);
			if (it == _batch.end())
				_batch[material] = { std::make_pair(world, mesh) };
			else
				it->second.push_back(std::make_pair(world, mesh));
		}

		// if the entity has a mesh collection
		auto mesh_collection = entity->get<MeshCollection>();
		if (mesh_collection)
		for (const auto& mesh : mesh_collection->getMeshes()) {
			assert(mesh);
			// store the mesh
			auto material = mesh->getMaterial();
			auto it = _batch.find(material);
			if (it == _batch.end())
				_batch[material] = { std::make_pair(world, mesh) };
			else
				it->second.push_back(std::make_pair(world, mesh));
		}
	}

	void GSPass::createProgram(const RenderContext* context) {
		std::string vs_code =
			"#version 330\n"

			"layout(std140) uniform uniform_world {\n"
			"	mat4 world;\n"
			"};\n"
			"layout(std140) uniform uniform_light_view {\n"
			"	mat4 light_view;\n"
			"};\n"
			"layout(std140) uniform uniform_light_projection {\n"
			"	mat4 light_projection;\n"
			"};\n"

			"in vec3 attribute_position;\n"
			"in vec2 attribute_uv;\n"

			"out vec4 position;\n"
			"out vec2 uv;\n"

			"void main () {\n"
			"	position = light_view*world*vec4(attribute_position, 1.0f);\n"
			"   uv = attribute_uv;\n"
			"	gl_Position = light_projection*position;\n"
			"}\n";
		auto vs = VertexShader::Create(vs_code, context); assert(vs);

		std::string fs_code =
			"#version 330\n"

			"in vec4 position;\n"
			"in vec2 uv;\n"

			"layout(location=0) out float out_depth;\n"

			"uniform sampler2D texture_diffuse;\n"

			"void main () {\n"
			"	vec4 color_diffuse = texture(texture_diffuse, uv);\n"
			"	if (color_diffuse.a > 0.5f) {\n"
			"		out_depth = position.z;\n"
			"	}\n"
			"	else {\n"
			"		discard;\n"
			"	}\n"
			"}\n";
		auto fs = FragmentShader::Create(fs_code, context); assert(fs);

		_program = Program::Create(vs, nullptr, fs, context); assert(_program);
		_program->linkAttribute("attribute_position", ATTRIBUTE_POSITION);
		_program->linkAttribute("attribute_uv", ATTRIBUTE_UV);
		_program->linkUniform("uniform_world", UNIFORM_WORLD);
		_program->linkUniform("uniform_light_view", UNIFORM_LIGHT_VIEW);
		_program->linkUniform("uniform_light_projection", UNIFORM_LIGHT_PROJECTION);
		_program->linkTexture("texture_diffuse", TEXTURE_DIFFUSE);
		_program->linkSampler("texture_diffuse", SAMPLER_DIFFUSE);
		_program->link(context);

		BlendConfig blend_config;
		blend_config.blend_enabled = false;
		blend_config.alpha_to_coverage = false;
		blend_config.src = BLEND_FUNC::SRC_ALPHA;
		blend_config.dst = BLEND_FUNC::INV_SRC_ALPHA;
		blend_config.op = BLEND_OP::ADD;
		blend_config.src_alpha = BLEND_FUNC::SRC_ALPHA;
		blend_config.dst_alpha = BLEND_FUNC::INV_SRC_ALPHA;
		blend_config.op_alpha = BLEND_OP::ADD;
		_blend_state = BlendState::Create(blend_config, context); assert(_blend_state);

		DepthStencilConfig depth_stencil_config;
		depth_stencil_config.depth_enabled = true;
		_depth_stencil_state = DepthStencilState::Create(depth_stencil_config, context); assert(_depth_stencil_state);

		setUniform(UNIFORM_WORLD, glm::mat4(), USAGE::MUTABLE, context);
		setUniform(UNIFORM_LIGHT_VIEW, glm::mat4(), context);
		setUniform(UNIFORM_LIGHT_PROJECTION, glm::mat4(), context);
	}

}