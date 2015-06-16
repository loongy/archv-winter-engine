/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include <string>

#include <common/Entity.h>

#include <graphics/AttributeArray.h>
#include <graphics/AttributeBuffer.h>
#include <graphics/BlendState.h>
#include <graphics/DepthStencilState.h>
#include <graphics/FragmentShader.h>
#include <graphics/Program.h>
#include <graphics/RenderContext.h>
#include <graphics/Uniform.h>
#include <graphics/VertexShader.h>

#include "LCompositionEffect.h"

namespace wntr {

	std::shared_ptr<LCompositionEffect> LCompositionEffect::Create(const RenderContext* context) {
		return std::make_shared<LCompositionEffect>(context);
	}

	LCompositionEffect::LCompositionEffect(const RenderContext* context)
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

			"layout(std140) uniform uniform_light_ambient {\n"
			"	vec4 light_ambient;\n"
			"};\n"

			"in vec2 uv;\n"

			"layout(location=0) out vec4 out_diffuse;\n"

			"uniform sampler2D texture_diffuse;\n"
			"uniform sampler2D texture_light;\n"
			"uniform sampler2D texture_specular;\n"
			"uniform sampler2D texture_emissive;\n"

			"void main () {\n"
			"   vec4 color = texture(texture_diffuse, uv);\n"
			"   vec4 light = texture(texture_light, uv);\n"
			"   vec4 specular = texture(texture_specular, uv);\n"
			"   vec4 emissive = texture(texture_emissive, uv);\n"
			"	out_diffuse = color*(light_ambient + light) + specular + emissive;\n"
			"}\n";
		auto fs = FragmentShader::Create(fs_code, context); assert(fs);

		_program = Program::Create(vs, nullptr, fs, context); assert(_program);
		_program->linkAttribute("attribute_position", ATTRIBUTE_POSITION);
		_program->linkUniform("uniform_light_ambient", UNIFORM_LIGHT_AMBIENT);
		_program->linkTexture("texture_diffuse", TEXTURE_DIFFUSE);
		_program->linkSampler("texture_diffuse", SAMPLER_DIFFUSE);
		_program->linkTexture("texture_light", TEXTURE_LIGHT);
		_program->linkSampler("texture_light", SAMPLER_LIGHT);
		_program->linkTexture("texture_specular", TEXTURE_SPECULAR);
		_program->linkSampler("texture_specular", SAMPLER_SPECULAR);
		_program->linkTexture("texture_emissive", TEXTURE_EMISSIVE);
		_program->linkSampler("texture_emissive", SAMPLER_EMISSIVE);
		_program->link(context);

		BlendConfig blend_config;
		blend_config.blend_enabled = false;
		blend_config.alpha_to_coverage = false;
        _blend_state = BlendState::Create(blend_config, context); assert(_blend_state);
        
        DepthStencilConfig depth_stencil_config;
		depth_stencil_config.depth_enabled = false;
		_depth_stencil_state = DepthStencilState::Create(depth_stencil_config, context); assert(_depth_stencil_state);

		setUniform(UNIFORM_LIGHT_AMBIENT, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), context);
	}

}
