/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include <common/Color.h>
#include <common/Direction.h>
#include <common/Entity.h>
#include <common/Transform3D.h>

#include <graphics/AttributeArray.h>
#include <graphics/AttributeBuffer.h>
#include <graphics/BlendState.h>
#include <graphics/DepthStencilState.h>
#include <graphics/FragmentShader.h>
#include <graphics/Program.h>
#include <graphics/RenderContext.h>
#include <graphics/Uniform.h>
#include <graphics/VertexShader.h>

#include "LOrthoPass.h"

#include "Material.h"
#include "Mesh.h"
#include "MeshCollection.h"
#include "View.h"

namespace wntr {
    
	std::shared_ptr<LOrthoPass> LOrthoPass::Create(const RenderContext* context) {
		return std::make_shared<LOrthoPass>(context);
    }
    
	LOrthoPass::LOrthoPass(const RenderContext* context)
    : RenderPass() {
		createAttributeArray(context);
		createProgram(context);
    }
    
	void LOrthoPass::bind(RenderContext* context) {
        RenderPass::bind(context);
        
        assert(context);
        context->setAttributeArray(_attribute_array);
    }

	void LOrthoPass::drawBatch(RenderContext* context) {
		assert(_batch_direction.size() == _batch_color.size());
		// set instance directions
		setUniform(UNIFORM_INSTANCE_LIGHT_DIRECTION,
			&_batch_direction[0],
			(unsigned)std::min(sizeof(glm::vec4)*_batch_direction.size(), sizeof(glm::vec4) * 1024),
			context);
		// set instance colors
		setUniform(UNIFORM_INSTANCE_LIGHT_COLOR,
			&_batch_color[0],
			(unsigned)std::min(sizeof(glm::vec4)*_batch_color.size(), sizeof(glm::vec4) * 1024),
			context);
		// draw instances
		assert(context);
		context->drawInstances((unsigned)std::min((long)_batch_direction.size(), (long)1024));
		// empty the instance map, ready for the next batch
		_batch_direction.clear();
		_batch_color.clear();
	}
    
	void LOrthoPass::addToBatch(const std::shared_ptr<Entity>& entity, const glm::mat4& world, RenderContext* context) {
        assert(entity);
        
		auto view = entity->get<View>();
		if (view) {
			// prefer getting the direction from a view than a direction component
			_batch_direction.push_back(glm::vec4(view->getForward(), 0.0f));
		}
		else {
			// ignore entities with no direction and no view
			auto direction = entity->get<Direction>();
			if (!direction)
				return;
			_batch_direction.push_back(glm::vec4(direction->getDirection(), 0.0f));
		}
        
        // set the color data
        auto color = entity->get<Color>();
		if (color)
			_batch_color.push_back(color->getColor());
        else
			_batch_color.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    }

	void LOrthoPass::createAttributeArray(const RenderContext* context) {

		float positions[] = {
			-1.0f, 1.0, 0.0f,
			1.0, 1.0, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
		};

		unsigned elements[] = {
			0, 1, 2,
			2, 3, 0,
		};

		auto position_buffer = AttributeBuffer::Create(positions, 4, ATTRIBUTE_POSITION, FORMAT::FLOAT3, USAGE::DEFAULT, context); assert(position_buffer);
		auto element_buffer  = AttributeBuffer::Create(elements, 6, ATTRIBUTE_ELEMENTS, FORMAT::UINT, USAGE::DEFAULT, context);    assert(element_buffer);

		_attribute_array = AttributeArray::Create(POLYGON_TYPE::TRIANGLE, context); assert(_attribute_array);
		_attribute_array->setAttribute(position_buffer);
		_attribute_array->setAttribute(element_buffer);
	}

	void LOrthoPass::createProgram(const RenderContext* context) {
		std::string vs_code =
			"#version 330\n"

			"layout(std140) uniform uniform_light_direction {\n"
			"	vec4 light_direction[1024];\n"
			"};\n"

			"layout(std140) uniform uniform_light_color {\n"
			"	vec4 light_color[1024];\n"
			"};\n"

			"in vec3 attribute_position;\n"

			"out vec4 l_direction; \n"
			"out vec4 l_color;\n"
			"out vec2 uv;\n"

			"void main () {\n"
			"	gl_Position = vec4(attribute_position, 1.0f);\n"
			"   l_direction = light_direction[gl_InstanceID];\n"
			"   l_color = light_color[gl_InstanceID];\n"
			"   uv = (attribute_position.xy + vec2(1.0f, 1.0f))*0.5f;\n" 
			"}\n";
		auto vs = VertexShader::Create(vs_code, context); assert(vs);

		std::string fs_code =
			"#version 330\n"

			"layout(std140) uniform uniform_camera_position {\n"
			"	vec4 camera_position;\n"
			"};\n"

			"in vec4 l_direction; \n"
			"in vec4 l_color;\n"
			"in vec2 uv;\n"

			"layout(location=0) out vec3 out_light;\n"
			"layout(location=1) out vec3 out_specular;\n"

			"uniform sampler2D texture_position;\n"
			"uniform sampler2D texture_normal;\n"
			"uniform sampler2D texture_specular;\n"

			"void main () {\n"
			"   vec3 to_light = normalize(-l_direction.xyz);\n"
			"   vec3 position = texture(texture_position, uv).xyz;\n"
			"   vec3 normal = normalize(texture(texture_normal, uv).xyz);\n"

			"	float diffuse_coeff = max(0.0f, dot(normal, to_light));\n"

			"   float shininess = texture(texture_specular, uv).r;\n"
			"   float specular_coeff = 0.0f;\n"
			"   if (diffuse_coeff > 0.0f && shininess > 0.0f) {\n"
			"		vec3 reflection = reflect(-to_light, normal);\n"
			"		float angle = max(0.0f, dot(normalize(camera_position.xyz-position), reflection));\n"
			"		specular_coeff = pow(angle, shininess);\n"
			"   }\n"

			"	out_light    = diffuse_coeff * l_color.rgb;\n"
			"	out_specular = diffuse_coeff * specular_coeff * l_color.rgb;\n"
			"}\n";
		auto fs = FragmentShader::Create(fs_code, context); assert(fs);

		_program = Program::Create(vs, nullptr, fs, context); assert(_program);
		_program->linkAttribute("attribute_position", ATTRIBUTE_POSITION);
		_program->linkUniform("uniform_camera_position", UNIFORM_CAMERA_POSITION);
		_program->linkUniform("uniform_light_direction", UNIFORM_INSTANCE_LIGHT_DIRECTION);
		_program->linkUniform("uniform_light_color", UNIFORM_INSTANCE_LIGHT_COLOR);
		_program->linkTexture("texture_position", TEXTURE_POSITION);
		_program->linkSampler("texture_position", SAMPLER_POSITION);
		_program->linkTexture("texture_normal", TEXTURE_NORMAL);
		_program->linkSampler("texture_normal", SAMPLER_NORMAL);
		_program->linkTexture("texture_specular", TEXTURE_SPECULAR);
		_program->linkSampler("texture_specular", SAMPLER_SPECULAR);
		_program->link(context);

		BlendConfig blend_config;
		blend_config.blend_enabled = true;
		blend_config.alpha_to_coverage = false;
		blend_config.src = BLEND_FUNC::ONE;
		blend_config.dst = BLEND_FUNC::ONE;
		blend_config.op = BLEND_OP::ADD;
		blend_config.src_alpha = BLEND_FUNC::ONE;
		blend_config.dst_alpha = BLEND_FUNC::ONE;
		blend_config.op_alpha = BLEND_OP::ADD;
		_blend_state = BlendState::Create(blend_config, context); assert(_blend_state);

		DepthStencilConfig depth_stencil_config;
		depth_stencil_config.depth_enabled = false;
		_depth_stencil_state = DepthStencilState::Create(depth_stencil_config, context); assert(_depth_stencil_state);


		setUniform(UNIFORM_CAMERA_POSITION, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), context);

		glm::vec4 direction[1024];
		for (unsigned i = 0; i < 1024; i++)
			direction[i] = glm::vec4();
		setUniform(UNIFORM_INSTANCE_LIGHT_DIRECTION, (void*)direction, sizeof(glm::vec4) * 1024, USAGE::MUTABLE, context);

		glm::vec4 color[1024];
		for (unsigned i = 0; i < 1024; i++)
			color[i] = glm::vec4();
		setUniform(UNIFORM_INSTANCE_LIGHT_COLOR, (void*)color, sizeof(glm::vec4) * 1024, USAGE::MUTABLE, context);
	}

}
