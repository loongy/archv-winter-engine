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

#include "LSOrthoEffect.h"

#include "Material.h"
#include "Mesh.h"
#include "MeshCollection.h"
#include "Projection.h"
#include "View.h"

namespace wntr {

	std::shared_ptr<LSOrthoEffect> LSOrthoEffect::Create(const RenderContext* context) {
		return std::make_shared<LSOrthoEffect>(context);
	}

	LSOrthoEffect::LSOrthoEffect(const RenderContext* context)
	: RenderEffect(context) {
		std::string vs_code =
			"#version 330\n"

			"in vec3 attribute_position;\n"

			"out vec2 uv;\n"

			"void main () {\n"
			"	gl_Position = vec4(attribute_position, 1.0f);\n"
			"   uv = (attribute_position.xy + vec2(1.0f, 1.0f))*0.5f;\n"
			"}\n";
		auto vs = VertexShader::Create(vs_code, context); assert(vs);

		std::string fs_code =
			"#version 330\n"

			"layout(std140) uniform uniform_camera_position {\n"
			"	vec4 camera_position;\n"
			"};\n"

			"layout(std140) uniform uniform_light_view {\n"
			"	mat4 light_view;\n"
			"};\n"

			"layout(std140) uniform uniform_light_projection {\n"
			"	mat4 light_projection;\n"
			"};\n"

			"layout(std140) uniform uniform_light_direction {\n"
			"	vec4 light_direction;\n"
			"};\n"

			"layout(std140) uniform uniform_light_color {\n"
			"	vec4 light_color;\n"
			"};\n"

			"in vec2 uv;\n"

			"layout(location=0) out vec3 out_light;\n"
			"layout(location=1) out vec3 out_specular;\n"

			"uniform sampler2D texture_shadow;\n"
			"uniform sampler2D texture_position;\n"
			"uniform sampler2D texture_normal;\n"
			"uniform sampler2D texture_specular;\n"

			"void main () {\n"
			"   vec3 position = texture(texture_position, uv).xyz;\n"
			"   vec4 world_view_position = light_view*vec4(position, 1.0f);\n"
			"   vec4 world_view_proj_position = light_projection*world_view_position;\n"

			"   vec2 uv_shadow = vec2( world_view_proj_position.x / world_view_proj_position.w / 2.0f + 0.5f,"
			"                          1.0f - (-world_view_proj_position.y / world_view_proj_position.w / 2.0f + 0.5f));\n"

			"	if ((clamp(uv_shadow.x, 0.0f, 1.0f) == uv_shadow.x) && (clamp(uv_shadow.y, 0.0f, 1.0f) == uv_shadow.y)) {"
			"		float depth_bias = 0.1f;"
			"		float depth      = texture(texture_shadow, uv_shadow).r;\n"
			"		float frag_depth = world_view_position.z - depth_bias;\n"

			"		if (frag_depth > depth) {"
			"			out_light    = vec3(0.0f);\n"
			"			out_specular = vec3(0.0f);\n"
			"			return;\n"
			"		}"
			"	}"

			"	vec3 to_light = normalize(-light_direction.xyz);\n"

			"	vec3 normal = normalize(texture(texture_normal, uv).xyz);\n"

			"	float diffuse_coeff = max(0.0f, dot(normal, to_light));\n"

			"	float shininess = texture(texture_specular, uv).r;\n"
			"	float specular_coeff = 0.0f;\n"
			"	if (diffuse_coeff > 0.0f && shininess > 0.0f) {\n"
			"		vec3 reflection = reflect(-to_light, normal);\n"
			"		float angle = max(0.0f, dot(normalize(camera_position.xyz-position), reflection));\n"
			"		specular_coeff = pow(angle, shininess);\n"
			"	}\n"

			"	out_light    = diffuse_coeff * light_color.rgb;\n"
			"	out_specular = diffuse_coeff * specular_coeff * light_color.rgb;\n"

			"}\n";
		auto fs = FragmentShader::Create(fs_code, context); assert(fs);

		_program = Program::Create(vs, nullptr, fs, context); assert(_program);
		_program->linkAttribute("attribute_position", ATTRIBUTE_POSITION);
		_program->linkUniform("uniform_camera_position", UNIFORM_CAMERA_POSITION);
		_program->linkUniform("uniform_light_view", UNIFORM_LIGHT_VIEW);
		_program->linkUniform("uniform_light_projection", UNIFORM_LIGHT_PROJECTION);
		_program->linkUniform("uniform_light_direction", UNIFORM_LIGHT_DIRECTION);
		_program->linkUniform("uniform_light_color", UNIFORM_LIGHT_COLOR);
		_program->linkTexture("texture_shadow", TEXTURE_SHADOW);
		_program->linkSampler("texture_shadow", SAMPLER_SHADOW);
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
		setUniform(UNIFORM_LIGHT_VIEW, glm::mat4(), context);
		setUniform(UNIFORM_LIGHT_PROJECTION, glm::mat4(), context);
		setUniform(UNIFORM_LIGHT_DIRECTION, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), USAGE::MUTABLE, context);
		setUniform(UNIFORM_LIGHT_COLOR, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), USAGE::MUTABLE, context);
	}

	/*
	void LSOrthoEffect::draw(const std::shared_ptr<Entity>& entity, const glm::mat4& world, RenderContext* context) {
		assert(entity);

		// ignore entities with no view
		auto view = entity->get<View>();
		if (!view)
			return;
		setUniform(UNIFORM_LIGHT_DIRECTION, glm::vec4(view->getForward(), 0.0f), context);
		setUniform(UNIFORM_LIGHT_VIEW, view->view(), context);

		// ignore entities with no projection
		auto projection = entity->get<Projection>();
		if (!projection)
			return;
		setUniform(UNIFORM_LIGHT_PROJECTION, projection->projection(), context);

		// set the color data
		auto color = entity->get<Color>();
		if (color)
			setUniform(UNIFORM_LIGHT_COLOR, color->getColor(), context);
		else
			setUniform(UNIFORM_LIGHT_COLOR, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), context);

		// draw the light
		assert(context);
		context->draw();
	}
	*/

}