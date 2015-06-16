/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "openglFragmentShader.h"
#include "openglGeometryShader.h"
#include "openglProgram.h"
#include "openglRenderContext.h"
#include "openglSampler.h"
#include "openglTexture.h"
#include "openglTextureResource.h"
#include "openglUniform.h"
#include "openglVertexShader.h"

#include <cassert>
#include <iostream>

namespace wntr {

	openglProgram::openglProgram(const std::shared_ptr<openglVertexShader>& vs, const std::shared_ptr<openglGeometryShader>& gs, const std::shared_ptr<openglFragmentShader>& fs, const openglRenderContext* context)
	: Program(vs, gs, fs, context->getApi()) {
		assert(vs);
		assert(fs);
		assert(context);

		if (context->getApi() != API::OPENGL)
			throw std::invalid_argument("error: openglProgram::openglProgram: expected RenderContext to implement API::OPENGL");
		
		_opengl_id = static_cast<const openglRenderContext*>(context)->glCreateProgram();
	}

	openglProgram::~openglProgram() {
		glDeleteProgram(_opengl_id);
		_opengl_id = 0;
	}

	void openglProgram::link(const RenderContext* context) {
		assert(context);

		if (context->getApi() != API::OPENGL)
			throw std::invalid_argument("error: openglProgram::link: expected RenderContext to implement API::OPENGL");
		
		auto opengl_context = static_cast<const openglRenderContext*>(context);

		if (!attachShaders(opengl_context))
			throw std::runtime_error("error: openglProgram::link: failed to attach shaders");

		auto current_program = static_cast<openglProgram*>(opengl_context->getProgram().get());

		GLint status;
		opengl_context->glLinkProgram(_opengl_id);
		opengl_context->glGetProgramiv(_opengl_id, GL_LINK_STATUS, &status);

		if (status != GL_TRUE) {
			throw std::runtime_error("error: openglProgram::link: failed to link program");
		}
		else {
			if (!findShaderAttributeLocations(opengl_context))
				throw std::runtime_error("error: openglProgram::link: failed to find attribute locations");
			if (!findShaderBindingPoints(opengl_context))
				throw std::runtime_error("error: openglProgram::link: failed to find binding points");
		}

		if (current_program)
			opengl_context->glUseProgram(current_program->_opengl_id);
	}

	void openglProgram::bindUniform(const std::shared_ptr<Uniform>& uniform, uniform_id id, const RenderContext* context) {
		assert(context);

		if (context->getApi() != API::OPENGL)
			throw std::invalid_argument("error: openglProgram::bindUniform: expected RenderContext to implement API::OPENGL");
		
		auto opengl_context = static_cast<const openglRenderContext*>(context);

		if (!uniform) {
			int binding_point = getUniformBindingPoint(id);
			if (binding_point < 0) {
				// throw std::invalid_argument("error: openglProgram::bindUniform: could not find a binding point with the given UniformId");
				// upon bad binding, do nothing
				return;
			}

			_uniforms[id] = nullptr;
			if (opengl_context->getProgram().get() == (Program*)this)
				opengl_context->glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, _uniforms[id]->getOpenGLId());

			return;
		}

		if (uniform->getApi() != API::OPENGL)
			throw std::invalid_argument("error: openglProgram::bindUniform: expected IUniform to implement API::OPENGL");

		int binding_point = getUniformBindingPoint(id);
		if (binding_point < 0) {
			// throw std::invalid_argument("error: openglProgram::bindUniform: could not find a binding point with the given UniformId");
			// upon bad binding, do nothing
			return;
		}

		_uniforms[id] = std::static_pointer_cast<openglUniform>(uniform);
		if (context->getProgram().get() == (Program*)this)
			opengl_context->glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, _uniforms[id]->getOpenGLId());
	}

	void openglProgram::bindTexture(const std::shared_ptr<Texture>& texture, texture_id id, const RenderContext* context) {
		assert(context);

		if (context->getApi() != API::OPENGL)
			throw std::invalid_argument("error: openglProgram::bindTexture: expected RenderContext to implement API::OPENGL");
		
		auto opengl_context = static_cast<const openglRenderContext*>(context);

		if (!texture) {
			int binding_point = getTextureBindingPoint(id);
			if (binding_point < 0) {
				// throw std::invalid_argument("error: openglProgram::bindTexture: could not find a binding point with the given TextureId");
				// upon bad binding, do nothing
				return;
			}
			auto texture_unit = _texture_units.find(binding_point);
			if (texture_unit == _texture_units.end())
				throw std::invalid_argument("error: openglProgram::bindTexture: could not find a texture unit for the given texture_id");

			_textures[id] = nullptr;
			if (context->getProgram().get() == (Program*)this) {
				opengl_context->glUniform1i(binding_point, texture_unit->second);
				opengl_context->glActiveTexture(GL_TEXTURE0 + texture_unit->second);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			return;
		}

		auto resource = static_cast<openglTextureResource*>(texture->getResource().get());
		if (!resource) {
			return;
		}
		if (resource->getApi() != API::OPENGL) {
			throw std::invalid_argument("error: openglProgram::bindTexture: expected ITextureResource to implement API::OPENGL");
		}

		int binding_point = getTextureBindingPoint(id);
		if (binding_point < 0) {
			// throw std::invalid_argument("error: openglProgram::bindTexture: could not find a binding point with the given TextureId");
			// upon bad binding, do nothing
			return;
		}
		auto texture_unit = _texture_units.find(binding_point);
		if (texture_unit == _texture_units.end()) {
			throw std::invalid_argument("error: openglProgram::bindTexture: could not find a texture unit for the given texture_id");
		}

		_textures[id] = std::static_pointer_cast<openglTexture>(texture);
		if (opengl_context->getProgram().get() == (Program*)this) {
			opengl_context->glUniform1i(binding_point, texture_unit->second);
			opengl_context->glActiveTexture(GL_TEXTURE0 + texture_unit->second);
			glBindTexture(GL_TEXTURE_2D, resource->getOpenGLId());
		}
		return;
	}

	void openglProgram::bindSampler(const std::shared_ptr<Sampler>& sampler, sampler_id id, const RenderContext* context) {
		assert(context);

		if (context->getApi() != API::OPENGL)
			throw std::invalid_argument("error: openglProgram::bindSampler: expected RenderContext to implement API::OPENGL");
		
		auto opengl_context = static_cast<const openglRenderContext*>(context);

		if (!sampler) {
			int binding_point = getSamplerBindingPoint(id);
			if (binding_point < 0) {
				// throw std::invalid_argument("error: openglProgram::bindSampler: could not find a binding point with the given SamplerId");
				// upon bad binding, do nothing
				return;
			}
			auto texture_unit = _texture_units.find(binding_point);
			if (texture_unit == _texture_units.end()) {
				throw std::invalid_argument("error: openglProgram::bindSampler: could not find a texture unit for the given sampler_id");
			}

			_samplers[id] = nullptr;
			if (context->getProgram().get() == (Program*)this) {
				opengl_context->glUniform1i(binding_point, texture_unit->second);
				opengl_context->glActiveTexture(GL_TEXTURE0 + texture_unit->second);
				opengl_context->glBindSampler(texture_unit->second, 0);
			}
			return;
		}

		if (sampler->getApi() != API::OPENGL) {
			throw std::invalid_argument("error: openglProgram::bindSampler: expected Sampler to implement API::OPENGL");
		}

		int binding_point = getSamplerBindingPoint(id);
		if (binding_point < 0) {
			// throw std::invalid_argument("error: openglProgram::bindSampler: could not find a binding point with the given SamplerId");
			// upon bad binding, do nothing
			return;
		}
		auto texture_unit = _texture_units.find(binding_point);
		if (texture_unit == _texture_units.end()) {
			throw std::invalid_argument("error: openglProgram::bindSampler: could not find a texture unit for the given sampler_id");
		}

		_samplers[id] = std::static_pointer_cast<openglSampler>(sampler);
		if (context->getProgram().get() == (Program*)this) {
			opengl_context->glUniform1i(binding_point, texture_unit->second);
			opengl_context->glActiveTexture(GL_TEXTURE0 + texture_unit->second);
			opengl_context->glBindSampler(texture_unit->second, _samplers[id]->getOpenGLId());
		}
	}

	void openglProgram::linkAttribute(const std::string& name, attribute_id id) {
		_attribute_links[name] = id;

		auto it = _attribute_name_locations.find(name);
		if (it != _attribute_name_locations.end()) {
			_attribute_id_locations[id] = it->second;
		}
	}

	void openglProgram::linkUniform(const std::string& name, uniform_id id) {
		_uniform_links[name] = id;

		auto it = _uniform_name_bindings.find(name);
		if (it != _uniform_name_bindings.end()) {
			_uniform_id_bindings[id] = it->second;
		}
	}

	void openglProgram::linkTexture(const std::string& name, texture_id id) {
		_texture_links[name] = id;

		auto it = _texture_name_bindings.find(name);
		if (it != _texture_name_bindings.end()) {
			_texture_id_bindings[id] = it->second;
		}
	}

	void openglProgram::linkSampler(const std::string& name, sampler_id id) {
		_sampler_links[name] = id;

		auto it = _sampler_name_bindings.find(name);
		if (it != _sampler_name_bindings.end()) {
			_sampler_id_bindings[id] = it->second;
		}
	}

	const std::vector<attribute_id>& openglProgram::getAttributes() const {
		return _attributes;
	}

	int openglProgram::getAttributeLocation(attribute_id id) const {
		auto it = _attribute_id_locations.find(id);
		if (it != _attribute_id_locations.end()) {
			return it->second;
		}
		else {
			return -1;
		}
	}

	int openglProgram::getUniformBindingPoint(uniform_id id) const {
		auto it = _uniform_id_bindings.find(id);
		if (it != _uniform_id_bindings.end()) {
			return it->second;
		}
		else {
			return -1;
		}
	}

	int openglProgram::getTextureBindingPoint(texture_id id) const {
		auto it = _texture_id_bindings.find(id);
		if (it != _texture_id_bindings.end()) {
			return it->second;
		}
		else {
			return -1;
		}
	}

	int openglProgram::getSamplerBindingPoint(sampler_id id) const {
		auto it = _sampler_id_bindings.find(id);
		if (it != _sampler_id_bindings.end()) {
			return it->second;
		}
		else {
			return -1;
		}
	}

	void openglProgram::bindAllShaderConstants(const openglRenderContext* context) {
		for (auto& uniform : _uniforms) {
			assert(_uniform_id_bindings.find(uniform.first) != _uniform_id_bindings.end());
			assert(_uniform_id_bindings[uniform.first] >= 0);

			auto bind_point = _uniform_id_bindings[uniform.first];
			if (uniform.second) {
				context->glBindBufferBase(GL_UNIFORM_BUFFER, bind_point, uniform.second->getOpenGLId());
			}
			else {
				context->glBindBufferBase(GL_UNIFORM_BUFFER, bind_point, 0);
			}
		}
		for (auto& texture : _textures) {
			assert(_texture_id_bindings.find(texture.first) != _texture_id_bindings.end());
			assert(_texture_id_bindings[texture.first] >= 0);
			assert(_texture_units.find(_texture_id_bindings[texture.first]) != _texture_units.end());

			auto bind_point = _texture_id_bindings[texture.first];
			auto texture_unit = _texture_units[bind_point];
			if (texture.second) {
				context->glUniform1i(bind_point, texture_unit);
				context->glActiveTexture(GL_TEXTURE0 + texture_unit);
				glBindTexture(GL_TEXTURE_2D, static_cast<openglTextureResource*>(texture.second->getResource().get())->getOpenGLId());
			}
			else {
				context->glUniform1i(bind_point, texture_unit);
				context->glActiveTexture(GL_TEXTURE0 + texture_unit);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		for (auto& sampler : _samplers) {
			assert(_sampler_id_bindings.find(sampler.first) != _sampler_id_bindings.end());
			assert(_sampler_id_bindings[sampler.first] >= 0);
			assert(_texture_units.find(_sampler_id_bindings[sampler.first]) != _texture_units.end());

			auto bind_point = _sampler_id_bindings[sampler.first];
			auto texture_unit = _texture_units[bind_point];
			if (sampler.second) {
				context->glUniform1i(bind_point, texture_unit);
				context->glActiveTexture(GL_TEXTURE0 + texture_unit);
				context->glBindSampler(texture_unit, sampler.second->getOpenGLId());
			}
			else {
				context->glUniform1i(bind_point, texture_unit);
				context->glActiveTexture(GL_TEXTURE0 + texture_unit);
				context->glBindSampler(texture_unit, 0);
			}
		}
	}

	void openglProgram::unbindAllShaderConstants(const openglRenderContext* context) {
		for (auto& uniform : _uniforms) {
			assert(_uniform_id_bindings.find(uniform.first) != _uniform_id_bindings.end());
			assert(_uniform_id_bindings[uniform.first] >= 0);

			auto bind_point = _uniform_id_bindings[uniform.first];

			context->glBindBufferBase(GL_UNIFORM_BUFFER, bind_point, 0);
		}
		for (auto& texture : _textures) {
			assert(_texture_id_bindings.find(texture.first) != _texture_id_bindings.end());
			assert(_texture_id_bindings[texture.first] >= 0);
			assert(_texture_units.find(_texture_id_bindings[texture.first]) != _texture_units.end());

			auto bind_point = _texture_id_bindings[texture.first];
			auto texture_unit = _texture_units[bind_point];

			context->glUniform1i(bind_point, texture_unit);
			context->glActiveTexture(GL_TEXTURE0 + texture_unit);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		for (auto& sampler : _samplers) {
			assert(_sampler_id_bindings.find(sampler.first) != _sampler_id_bindings.end());
			assert(_sampler_id_bindings[sampler.first] >= 0);
			assert(_texture_units.find(_sampler_id_bindings[sampler.first]) != _texture_units.end());

			auto bind_point = _sampler_id_bindings[sampler.first];
			auto texture_unit = _texture_units[bind_point];

			context->glUniform1i(bind_point, texture_unit);
			context->glActiveTexture(GL_TEXTURE0 + texture_unit);
			context->glBindSampler(texture_unit, 0);
		}
	}

	GLuint openglProgram::getOpenGLId() const {
		return _opengl_id;
	}

	bool openglProgram::attachShaders(const openglRenderContext* context) {
		assert(context);

		if (_vertex_shader) {
			context->glAttachShader(_opengl_id, static_cast<openglVertexShader*>(_vertex_shader.get())->getOpenGLId());
		}
		else {
			return false;
		}
        
        if (_geometry_shader) {
			context->glAttachShader(_opengl_id, static_cast<openglGeometryShader*>(_geometry_shader.get())->getOpenGLId());
		}

		if (_fragment_shader) {
			context->glAttachShader(_opengl_id, static_cast<openglFragmentShader*>(_fragment_shader.get())->getOpenGLId());
		}
		else {
			return false;
		}

		return true;
	}
	bool openglProgram::findShaderAttributeLocations(const openglRenderContext* context) {
		if (_vertex_shader) {
			GLint active_attribs;
			context->glGetProgramiv(_opengl_id, GL_ACTIVE_ATTRIBUTES, &active_attribs);

			GLint max_attrib_name_length;
			context->glGetProgramiv(_opengl_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_attrib_name_length);

			char* attrib_name = new(std::nothrow) char[max_attrib_name_length + 1];
			if (!attrib_name) {
				return false;
			}

			for (int i = 0; i < active_attribs; ++i) {
				GLint attrib_location;
				GLint size;
				GLenum type;

				context->glGetActiveAttrib(_opengl_id, i, max_attrib_name_length + 1, nullptr, &size, &type, attrib_name);

				attrib_location = context->glGetAttribLocation(_opengl_id, attrib_name);

				if (attrib_location == (GLint)GL_INVALID_INDEX) {
					std::cerr << "warning: openglProgram::findShaderAttributeLocations: invalid attribute: " << attrib_name << std::endl;
					continue;
				}

				_attribute_name_locations[attrib_name] = attrib_location;
				auto it = _attribute_links.find(attrib_name);
				if (it != _attribute_links.end()) {
					_attributes.push_back(it->second);
					_attribute_id_locations[it->second] = attrib_location;
				}
			}

			delete[] attrib_name;
		}

		return true;
	}
	bool openglProgram::findShaderBindingPoints(const openglRenderContext* context) {
		// bind all uniform blocks
		{
			GLint active_uniform_blocks;
			context->glGetProgramiv(_opengl_id, GL_ACTIVE_UNIFORM_BLOCKS, &active_uniform_blocks);

			GLint max_uniform_nlock_name_length;
			context->glGetProgramiv(_opengl_id, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &max_uniform_nlock_name_length);

			char* uniform_block_name = new(std::nothrow) char[max_uniform_nlock_name_length + 1];
			if (!uniform_block_name) {
				return false;
			}

			for (int i = 0; i < active_uniform_blocks; ++i) {
				GLint binding_index;
				GLint binding_point;
				GLsizei name_length;

				context->glGetActiveUniformBlockiv(_opengl_id, i, GL_UNIFORM_BLOCK_NAME_LENGTH, &name_length);
				context->glGetActiveUniformBlockName(_opengl_id, i, name_length, nullptr, uniform_block_name);

				binding_index = context->glGetUniformBlockIndex(_opengl_id, uniform_block_name);
				binding_point = i;
				context->glUniformBlockBinding(_opengl_id, binding_index, binding_point);

				if (binding_index == (GLint)GL_INVALID_INDEX) {
					// std::cerr << "openglProgram::findShaderBindingPoints found an invalid uniform block" << std::endl;
					continue;
				}
				else {
					// std::cout << "openglProgram::findShaderBindingPoints found " << uniform_block_name << " at " << binding_index << std::endl;
				}

				_uniform_name_bindings[uniform_block_name] = binding_point;
				auto it = _uniform_links.find(uniform_block_name);
				if (it != _uniform_links.end()) {
					_uniform_id_bindings[it->second] = binding_point;
				}
			}
			delete[] uniform_block_name;
		}
		// bind all uniforms (ie textures and samplers)
		{
		GLint active_uniforms;
		context->glGetProgramiv(_opengl_id, GL_ACTIVE_UNIFORMS, &active_uniforms);

		GLint max_uniform_name_length;
		context->glGetProgramiv(_opengl_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_uniform_name_length);

		char* uniform_name = new(std::nothrow) char[max_uniform_name_length + 1];
		if (!uniform_name) {
			return false;
		}

		unsigned int textures_found = 0;
		for (int i = 0; i < active_uniforms; ++i) {
			GLenum type;
			GLint size;
			context->glGetActiveUniform(_opengl_id, i, max_uniform_name_length, nullptr, &size, &type, uniform_name);
			GLint binding_point = context->glGetUniformLocation(_opengl_id, uniform_name);

			if (binding_point == (GLint)GL_INVALID_INDEX) {
				//std::cerr << "openglProgram::findShaderBindingPoints found an invalid uniform (\"" << uniform_name << "\")" << std::endl;
				continue;
			}
			else {
				//std::cout << "openglProgram::findShaderBindingPoints found " << uniform_name << " at " << binding_point << std::endl;
			}

			if (isSamplerType(type)) {
				_texture_name_bindings[uniform_name] = binding_point;
				auto texture_it = _texture_links.find(uniform_name);
				if (texture_it != _texture_links.end()) {
					_texture_id_bindings[texture_it->second] = binding_point;
				}
				_sampler_name_bindings[uniform_name] = binding_point;
				auto sampler_it = _sampler_links.find(uniform_name);
				if (sampler_it != _sampler_links.end()) {
					_sampler_id_bindings[sampler_it->second] = binding_point;
				}

				_texture_units[binding_point] = textures_found;
				++textures_found;
			}
			else {
				std::cerr << "openglProgram::findShaderBindingPoints found an invalid uniform (\"" << uniform_name << "\")" << std::endl;
				continue;
			}
		}

		delete[] uniform_name;
	}

		return true;
	}

	bool openglProgram::isSamplerType(GLenum _type) {
		switch (_type) {
		case GL_SAMPLER_1D:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_3D:
			//case GL_SAMPLER_CUBE:
			//case GL_SAMPLER_1D_SHADOW:
			//case GL_SAMPLER_2D_SHADOW:
			//case GL_SAMPLER_1D_ARRAY:
			//case GL_SAMPLER_2D_ARRAY:
			//case GL_SAMPLER_1D_ARRAY_SHADOW:
			//case GL_SAMPLER_2D_ARRAY_SHADOW:
			//case GL_SAMPLER_2D_MULTISAMPLE:
			//case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
			//case GL_SAMPLER_CUBE_SHADOW:
			//case GL_SAMPLER_BUFFER:
			//case GL_SAMPLER_2D_RECT:
			//case GL_SAMPLER_2D_RECT_SHADOW:
			return true;
		default:
			return false;
		}
	}

}
