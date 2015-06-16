/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_OPENGLPROGRAM_H_
#define _WNTR_OPENGLPROGRAM_H_

#include "../Program.h"

#include <unordered_map>

namespace wntr {

	class openglFragmentShader;
    class openglGeometryShader;
	class openglRenderContext;
	class openglSampler;
	class openglTexture;
	class openglUniform;
	class openglVertexShader;

	class openglProgram : public Program {
	public:
#ifndef _MSC_VER
		openglProgram(openglProgram&&) = default;
		openglProgram& operator= (openglProgram&&) = default;
#endif
		openglProgram(const openglProgram&) = delete;
		openglProgram& operator= (const openglProgram&) = delete;

		explicit openglProgram(const std::shared_ptr<openglVertexShader>& vs, const std::shared_ptr<openglGeometryShader>& gs, const std::shared_ptr<openglFragmentShader>& fs, const openglRenderContext* context);
		virtual ~openglProgram();

		virtual void link(const RenderContext* context) override;

		virtual void linkAttribute(const std::string& name, attribute_id id) override;
		virtual void linkUniform(const std::string& name, uniform_id id) override;
		virtual void linkTexture(const std::string& name, texture_id id) override;
		virtual void linkSampler(const std::string& name, sampler_id id) override;

		virtual void bindUniform(const std::shared_ptr<Uniform>& uniform, uniform_id id, const RenderContext* context) override;
		virtual void bindTexture(const std::shared_ptr<Texture>& texture, texture_id id, const RenderContext* context) override;
		virtual void bindSampler(const std::shared_ptr<Sampler>& sampler, sampler_id id, const RenderContext* context) override;

		virtual const std::vector<attribute_id>& getAttributes() const override;
		virtual int getAttributeLocation(attribute_id id) const override;
		virtual int getUniformBindingPoint(uniform_id id) const override;
		virtual int getTextureBindingPoint(texture_id id) const override;
		virtual int getSamplerBindingPoint(sampler_id id) const override;

		void bindAllShaderConstants(const openglRenderContext* context);
		void unbindAllShaderConstants(const openglRenderContext* context);
		unsigned getOpenGLId() const;

	protected:

	private:
		bool attachShaders(const openglRenderContext* context);
		bool findShaderAttributeLocations(const openglRenderContext* context);
		bool findShaderBindingPoints(const openglRenderContext* context);
		bool isSamplerType(unsigned opengl_enum);

		std::vector<attribute_id> _attributes;

		std::unordered_map<std::string, attribute_id> _attribute_links;
		std::unordered_map<std::string, uniform_id> _uniform_links;
		std::unordered_map<std::string, texture_id> _texture_links;
		std::unordered_map<std::string, sampler_id> _sampler_links;

		std::unordered_map<std::string, unsigned> _attribute_name_locations;
		std::unordered_map<std::string, unsigned> _uniform_name_bindings;
		std::unordered_map<std::string, unsigned> _texture_name_bindings;
		std::unordered_map<std::string, unsigned> _sampler_name_bindings;

		std::unordered_map<attribute_id, unsigned> _attribute_id_locations;
		std::unordered_map<uniform_id, unsigned> _uniform_id_bindings;
		std::unordered_map<texture_id, unsigned> _texture_id_bindings;
		std::unordered_map<sampler_id, unsigned> _sampler_id_bindings;

		std::unordered_map<uniform_id, std::shared_ptr<openglUniform>> _uniforms;
		std::unordered_map<texture_id, std::shared_ptr<openglTexture>> _textures;
		std::unordered_map<sampler_id, std::shared_ptr<openglSampler>> _samplers;

		std::unordered_map<int, unsigned> _texture_units;

		unsigned  _opengl_id;

	};

}

#endif