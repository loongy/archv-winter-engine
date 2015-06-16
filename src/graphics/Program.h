/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_PROGRAM_H_
#define _WNTR_PROGRAM_H_

#include "CommonGraphics.h"

#include <memory>
#include <string>
#include <vector>

namespace wntr {

    class GeometryShader;
	class FragmentShader;
	class RenderContext;
	class Sampler;
	class Texture;
	class Uniform;
	class VertexShader;

	class Program {
	public:
		static std::shared_ptr<Program> Create(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<GeometryShader>& gs, const std::shared_ptr<FragmentShader>& fs, const RenderContext* context);

#ifndef _MSC_VER
		Program(Program&&) = default;
		Program& operator= (Program&&) = default;
#endif
		Program(const Program&) = delete;
		Program& operator= (const Program&) = delete;

		explicit Program(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<GeometryShader>& gs, const std::shared_ptr<FragmentShader>& fs, API api);
		virtual ~Program() = default;

		virtual void link(const RenderContext* context) = 0;

		virtual void linkAttribute(const std::string& name, attribute_id id) = 0;
		virtual void linkUniform(const std::string& name, uniform_id id) = 0;
		virtual void linkTexture(const std::string& name, texture_id id) = 0;
		virtual void linkSampler(const std::string& name, sampler_id id) = 0;

		virtual void bindUniform(const std::shared_ptr<Uniform>& uniform, uniform_id id, const RenderContext* context) = 0;
		virtual void bindTexture(const std::shared_ptr<Texture>& texture, texture_id id, const RenderContext* context) = 0;
		virtual void bindSampler(const std::shared_ptr<Sampler>& sampler, sampler_id id, const RenderContext* context) = 0;

		virtual const std::vector<attribute_id>& getAttributes() const = 0;
		virtual int getAttributeLocation(attribute_id id) const = 0;
		virtual int getUniformBindingPoint(uniform_id id) const = 0;
		virtual int getTextureBindingPoint(texture_id id) const = 0;
		virtual int getSamplerBindingPoint(sampler_id id) const = 0;

		const std::shared_ptr<VertexShader>& getVertexShader() const;
        const std::shared_ptr<GeometryShader>& getGeometryShader() const;
		const std::shared_ptr<FragmentShader>& getFragmentShader() const;
		API getApi() const;

	protected:
		const std::shared_ptr<VertexShader> _vertex_shader;
        const std::shared_ptr<GeometryShader> _geometry_shader;
		const std::shared_ptr<FragmentShader> _fragment_shader;

		const API _api;

	private:

	};

}

#endif