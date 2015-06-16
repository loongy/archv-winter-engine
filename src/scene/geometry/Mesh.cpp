/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "Material.h"
#include "Mesh.h"

#include <graphics/AttributeArray.h>
#include <graphics/AttributeBuffer.h>
#include <graphics/Program.h>
#include <graphics/RenderContext.h>

#include <cassert>

namespace wntr {
    
	std::shared_ptr<Mesh> Mesh::Create(const RenderContext* context) {
		return std::make_shared<Mesh>(context);
	}

	std::shared_ptr<Mesh> Mesh::CreateCube(float size, const RenderContext* context) {
		return CreatePrism(size, size, size, context);
	}

	std::shared_ptr<Mesh> Mesh::CreatePrism(float size_x, float size_y, float size_z, const RenderContext* context) {
		auto mesh = Create(context);
		float h_width  = size_x * 0.5f;
		float h_height = size_y * 0.5f;
		float h_depth  = size_z * 0.5f;

		float positions[] = {
			// -- front face
			-h_width, -h_height, -h_depth,
			-h_width,  h_height, -h_depth,
			 h_width,  h_height, -h_depth,
			 h_width, -h_height, -h_depth,

			// -- back face
			h_width, -h_height, h_depth,
			h_width, h_height, h_depth,
			-h_width, h_height, h_depth,
			-h_width, -h_height, h_depth,

			// -- left face
			-h_width, -h_height, h_depth,
			-h_width, h_height, h_depth,
			-h_width, h_height, -h_depth,
			-h_width, -h_height, -h_depth,

			// -- right face
			h_width, -h_height, -h_depth,
			h_width, h_height, -h_depth,
			h_width, h_height, h_depth,
			h_width, -h_height, h_depth,

			// -- top face
			-h_width, h_height, -h_depth,
			-h_width, h_height, h_depth,
			h_width, h_height, h_depth,
			h_width, h_height, -h_depth,

			// -- bottom face
			h_width, -h_height, -h_depth,
			h_width, -h_height, h_depth,
			-h_width, -h_height, h_depth,
			-h_width, -h_height, -h_depth,
		};
		float normals[] = {
			// -- front face
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,

			// -- back face
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,

			// -- left face
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,

			// -- right face
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			// -- top face
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,

			// -- bottom face
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
		};
		float uvs[] = {
			// -- front face
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,

			// -- back face
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,

			// -- left face
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,

			// -- right face
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,

			// -- top face
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,

			// -- bottom face
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
		};
		unsigned int elements[] = {
			0, 1, 2,
			0, 2, 3,

			4, 5, 6,
			4, 6, 7,

			8, 9, 10,
			8, 10, 11,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,

			20, 21, 22,
			20, 22, 23,
		};

		mesh->getAttributeArray()->setAttribute(AttributeBuffer::Create(positions, 24, ATTRIBUTE_POSITION, FORMAT::FLOAT3, USAGE::DEFAULT, context));
		mesh->getAttributeArray()->setAttribute(AttributeBuffer::Create(normals, 24, ATTRIBUTE_NORMAL, FORMAT::FLOAT3, USAGE::DEFAULT, context));
		mesh->getAttributeArray()->setAttribute(AttributeBuffer::Create(uvs, 24, ATTRIBUTE_UV, FORMAT::FLOAT2, USAGE::DEFAULT, context));
		mesh->getAttributeArray()->setAttribute(AttributeBuffer::Create(elements, 36, ATTRIBUTE_ELEMENTS, FORMAT::UINT, USAGE::DEFAULT, context));

		return mesh;
	}
    
    std::shared_ptr<Mesh> Mesh::CreateUVSphere(float radius, unsigned div_lon, unsigned div_lat, const RenderContext* context) {
        assert(div_lon >= 5);
        assert(div_lat >= 1);
    
        // create vertices
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        float div_lon_angle = 2.0f*glm::pi<float>() / ((float)div_lon);
        float div_lat_angle = (glm::pi<float>() * 0.5f) / ((float)div_lat + 1.0f);
        float div_u = 1.0f / ((float)div_lon);
        float div_v = 0.5f / ((float)div_lat + 1.0f);
        // top hemisphere
        for (unsigned i = 0; i < div_lat + 2; i++) {
            float lat_angle = div_lat_angle * i;
            float axis_height = glm::sin(lat_angle) * radius;
            float lat_radius = glm::sqrt(radius*radius - axis_height*axis_height);
            
            for (unsigned j = 0; j < div_lon + 1; j++) {
                glm::vec3 r(0.0f, axis_height, lat_radius);
                r = glm::rotate(r, (float)j*div_lon_angle, glm::vec3(0.0f, 1.0f, 0.0f));
                
                positions.push_back(r);
                normals.push_back(glm::normalize(r));
                uvs.push_back(glm::vec2(div_u * j, div_v * i + 0.5f));
            }
        }
        // bottom hemisphere
        for (unsigned i = 1; i < div_lat + 2; i++) {
            float lat_angle = div_lat_angle * i;
            float axis_height = glm::sin(lat_angle) * radius;
            float lat_radius = glm::sqrt(radius*radius - axis_height*axis_height);
            
            for (unsigned j = 0; j < div_lon + 1; j++) {
                glm::vec3 r(0.0f, -axis_height, lat_radius);
                r = glm::rotate(r, (float)j*div_lon_angle, glm::vec3(0.0f, 1.0f, 0.0f));
                
                positions.push_back(r);
                normals.push_back(glm::normalize(r));
                uvs.push_back(glm::vec2(div_u * j, 0.5f - div_v * i));
            }
        }
        
        // create elements
        std::vector<unsigned> elements;
        unsigned top_offset    = (div_lon+1);
        unsigned bottom_offset = (div_lon+1)*(div_lat+2);
        // top hemisphere
        for (unsigned i = 0; i < div_lat+1; i++) {
            unsigned lat_offset = i * (div_lon+1) + top_offset;
            for (unsigned j = 0; j < (div_lon+1); j++) {
                elements.push_back(lat_offset + j);
                elements.push_back(lat_offset + ((j + 1)%(div_lon+1)));
                elements.push_back(lat_offset + ((j + 1)%(div_lon+1) - (div_lon+1)));
                
                elements.push_back(lat_offset + ((j + 1)%(div_lon+1) - (div_lon+1)));
                elements.push_back(lat_offset + j - (div_lon+1));
                elements.push_back(lat_offset + j);
            }
        }
        // bottom hemisphere
        for (unsigned j = 0; j < (div_lon+1); j++) {
            elements.push_back(bottom_offset + j);
            elements.push_back(bottom_offset + ((j + 1)%(div_lon+1)));
            elements.push_back((j + 1)%(div_lon+1));
            
            elements.push_back((j + 1)%(div_lon+1));
            elements.push_back(j);
            elements.push_back(bottom_offset + j);
        }
        for (unsigned i = 1; i < div_lat+1; i++) {
            unsigned lat_offset = i * (div_lon+1) + bottom_offset;
            for (unsigned j = 0; j < (div_lon+1); j++) {
                elements.push_back(lat_offset + j);
                elements.push_back(lat_offset + ((j + 1)%(div_lon+1)));
                elements.push_back(lat_offset + ((j + 1)%(div_lon+1) - (div_lon+1)));
                
                elements.push_back(lat_offset + ((j + 1)%(div_lon+1) - (div_lon+1)));
                elements.push_back(lat_offset + j - (div_lon+1));
                elements.push_back(lat_offset + j);
            }
        }
        
        auto mesh = Mesh::Create(context);
        assert(mesh);
        
        mesh->getAttributeArray()->setAttribute(AttributeBuffer::Create(&positions.at(0), (unsigned)positions.size(), ATTRIBUTE_POSITION, FORMAT::FLOAT3, USAGE::DEFAULT, context));
		mesh->getAttributeArray()->setAttribute(AttributeBuffer::Create(&normals.at(0), (unsigned)normals.size(), ATTRIBUTE_NORMAL, FORMAT::FLOAT3, USAGE::DEFAULT, context));
		mesh->getAttributeArray()->setAttribute(AttributeBuffer::Create(&uvs.at(0), (unsigned)uvs.size(), ATTRIBUTE_UV, FORMAT::FLOAT2, USAGE::DEFAULT, context));
		mesh->getAttributeArray()->setAttribute(AttributeBuffer::Create(&elements.at(0), (unsigned)elements.size(), ATTRIBUTE_ELEMENTS, FORMAT::UINT, USAGE::DEFAULT, context));
        
        return mesh;
    }

    Mesh::Mesh(const RenderContext* context)
    : Component() {
        assert(context);
        
        _attribute_array = AttributeArray::Create(POLYGON_TYPE::TRIANGLE, context);
        assert(_attribute_array);
    }
    /*
    void Mesh::draw(RenderContext* context) const {
        assert(context);
        assert(_attribute_array);
        
        auto program = context->getProgram();
        if (program) {
            if (_material) {
                program->bindTexture(_material->getTextureDiffuse(), TEXTURE_DIFFUSE, context);
                program->bindTexture(_material->getTextureSpecular(), TEXTURE_SPECULAR, context);
                program->bindTexture(_material->getTextureEmissive(), TEXTURE_EMISSIVE, context);
            }
            else {
                program->bindTexture(nullptr, TEXTURE_DIFFUSE, context);
                program->bindTexture(nullptr, TEXTURE_SPECULAR, context);
                program->bindTexture(nullptr, TEXTURE_EMISSIVE, context);
            }
            context->setAttributeArray(_attribute_array);
            context->draw();
        }
    }
    */
    const component_hierarchy& Mesh::hierarchy() const {
        return _Hierarchy;
    }
    
    void Mesh::setMaterial(const std::shared_ptr<Material>& material) {
        _material = material;
    }
    
    const std::shared_ptr<AttributeArray>& Mesh::getAttributeArray() const {
        return _attribute_array;
    }
    
    const std::shared_ptr<Material>& Mesh::getMaterial() const {
        return _material;
    }
    
    const component_hierarchy Mesh::_Hierarchy({typeid(Component),typeid(Mesh)});
    
}