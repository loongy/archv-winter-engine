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

#include "GInstancePass.h"

#include "Material.h"
#include "Mesh.h"
#include "MeshCollection.h"
#include "MeshInstance.h"

namespace wntr {
    
	std::shared_ptr<GInstancePass> GInstancePass::Create(const RenderContext* context) {
		return std::make_shared<GInstancePass>(context);
    }
    
	GInstancePass::GInstancePass(const RenderContext* context)
    : RenderPass() {
		createProgram(context);
    }

	void GInstancePass::drawBatch(RenderContext* context) {
		for (const auto& keys : _batch) {
			// set instance transformations
			setUniform(UNIFORM_INSTANCE_WORLD,
				&keys.second[0],
				(unsigned)std::min(sizeof(glm::mat4)*keys.second.size(), sizeof(glm::mat4) * 1024),
				context);
			// get mesh
			auto mesh = keys.first;
			// bind material
			assert(mesh);
			if (mesh->getMaterial()) {
				setTexture(TEXTURE_DIFFUSE, mesh->getMaterial()->getTextureDiffuse(), context);
				setTexture(TEXTURE_SPECULAR, mesh->getMaterial()->getTextureSpecular(), context);
				setTexture(TEXTURE_EMISSIVE, mesh->getMaterial()->getTextureEmissive(), context);
			}
			else {
				setTexture(TEXTURE_DIFFUSE, nullptr, context);
				setTexture(TEXTURE_SPECULAR, nullptr, context);
				setTexture(TEXTURE_EMISSIVE, nullptr, context);
			}
			// draw instances
			assert(context);
			context->setAttributeArray(mesh->getAttributeArray());
			context->drawInstances((unsigned)std::min((long)keys.second.size(), (long)1024));
		}
		// empty the instance map, ready for the next batch
		_batch.clear();
	}
    
	void GInstancePass::addToBatch(const std::shared_ptr<Entity>& entity, const glm::mat4& world, RenderContext* context) {
        assert(entity);

        // make sure the entity is transformable
        auto transform = entity->get<Transform3D>();
        if (!transform)
            return;

        // make sure the entity has a mesh instance
        auto mesh_instance = entity->get<MeshInstance>();
		if (!mesh_instance)
			return;

		// use this mesh instance to add the entity to the batch
        auto it = _batch.find(mesh_instance->getMesh());
		if (it == _batch.end())
			_batch[mesh_instance->getMesh()] = { world };
        else
            it->second.push_back(world);
    }

	void GInstancePass::createProgram(const RenderContext* context) {
		std::string vs_code =
			"#version 330\n"

			"layout(std140) uniform uniform_world {\n"
			"	mat4 world[1024];\n"
			"};\n"
			"layout(std140) uniform uniform_view {\n"
			"	mat4 view;\n"
			"};\n"
			"layout(std140) uniform uniform_projection {\n"
			"	mat4 projection;\n"
			"};\n"

			"in vec3 attribute_position;\n"
			"in vec3 attribute_normal;\n"
			"in vec2 attribute_uv;\n"

			"out vec3 position;\n"
			"out vec3 normal;\n"
			"out vec2 uv;\n"

			"void main () {\n"
			"	vec4 world_position = world[gl_InstanceID]*vec4(attribute_position, 1.0f);\n"
			"	gl_Position = projection*view*world_position;\n"
			"	position = world_position.xyz;\n"
			"	normal = (world[gl_InstanceID]*vec4(attribute_normal, 0.0f)).xyz;\n"
			"   uv = attribute_uv;\n"
			"}\n";
		auto vs = VertexShader::Create(vs_code, context); assert(vs);

		std::string fs_code =
			"#version 330\n"

			"in vec3 position;\n"
			"in vec3 normal;\n"
			"in vec2 uv;\n"

			"layout(location=0) out vec4 out_diffuse;\n"
			"layout(location=1) out vec3 out_position;\n"
			"layout(location=2) out vec3 out_normal;\n"
			"layout(location=3) out float out_specular;\n"
			"layout(location=4) out float out_emissive;\n"

			"uniform sampler2D texture_diffuse;\n"
			"uniform sampler2D texture_specular;\n"
			"uniform sampler2D texture_emissive;\n"

			"void main () {\n"
			"	out_diffuse  = texture(texture_diffuse, uv);\n"
			"	out_position = position;\n"
			"	out_normal   = normalize(normal);\n"
			"	out_specular = texture(texture_specular, uv).r;\n"
			"	out_emissive = texture(texture_emissive, uv).r;\n"
			"}\n";
		auto fs = FragmentShader::Create(fs_code, context); assert(fs);

		_program = Program::Create(vs, nullptr, fs, context); assert(_program);
		_program->linkAttribute("attribute_position", ATTRIBUTE_POSITION);
		_program->linkAttribute("attribute_normal", ATTRIBUTE_NORMAL);
		_program->linkAttribute("attribute_uv", ATTRIBUTE_UV);
		_program->linkUniform("uniform_world", UNIFORM_INSTANCE_WORLD);
		_program->linkUniform("uniform_view", UNIFORM_VIEW);
		_program->linkUniform("uniform_projection", UNIFORM_PROJECTION);
		_program->linkTexture("texture_diffuse", TEXTURE_DIFFUSE);
		_program->linkSampler("texture_diffuse", SAMPLER_DIFFUSE);
		_program->linkTexture("texture_specular", TEXTURE_SPECULAR);
		_program->linkSampler("texture_specular", SAMPLER_SPECULAR);
		_program->linkTexture("texture_emissive", TEXTURE_EMISSIVE);
		_program->linkSampler("texture_emissive", SAMPLER_EMISSIVE);
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

		glm::mat4 world[1024];
		for (unsigned i = 0; i < 1024; i++)
			world[i] = glm::mat4();
		setUniform(UNIFORM_INSTANCE_WORLD, (void*)world, sizeof(glm::mat4) * 1024, USAGE::MUTABLE, context);
		setUniform(UNIFORM_VIEW, glm::mat4(), context);
		setUniform(UNIFORM_PROJECTION, glm::mat4(), context);
	}
    
}