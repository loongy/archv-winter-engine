/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

// c++ standard library
#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// wntr common
#include <common/Entity.h>
#include <common/Transform3D.h>

// wntr graphics
#include <graphics/AttributeArray.h>
#include <graphics/AttributeBuffer.h>
#include <graphics/Texture.h>
#include <graphics/TextureResource.h>

// assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// freeimage
#define FREEIMAGE_LIB
#include <freeimage/FreeImage.h>

// wntr scene
#include "AssetLoader.h"
#include "ui/Bitmap.h"
#include "geometry/Material.h"
#include "geometry/Mesh.h"
#include "geometry/MeshCollection.h"

namespace wntr {

    std::shared_ptr<AssetLoader> AssetLoader::Create(const std::shared_ptr<RenderContext>& context) {
		return std::make_shared<AssetLoader>("", context);
	}
    
	std::shared_ptr<AssetLoader> AssetLoader::Create(const std::string& root, const std::shared_ptr<RenderContext>& context) {
		return std::make_shared<AssetLoader>(root, context);
	}

	AssetLoader::AssetLoader(const std::string& root, const std::shared_ptr<RenderContext>& context)
	: _root(root), _context(context) {

	}
    
    std::shared_ptr<Texture> AssetLoader::loadTexture(const std::string& path) const {
        if (!_FreeImage_Has_Been_Initialized) {
			FreeImage_Initialise();
			atexit((void(*)()) &FreeImage_DeInitialise);
			_FreeImage_Has_Been_Initialized = true;
		}
        
        std::string file = _root + path;
        
		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(file.c_str());
		if (format == FIF_UNKNOWN)
			format = FreeImage_GetFIFFromFilename(file.c_str());
		if (format == FIF_UNKNOWN)
			throw std::runtime_error("error: AssetLoader::loadBitmap: unsupported image file: " + file);
        
		FIBITMAP* fi_bitmap = FreeImage_Load(format, file.c_str());
		if (!fi_bitmap)
			throw std::runtime_error("error: AssetLoader::loadBitmap: failed to open file: " + file);
        
		FIBITMAP* fi_image = FreeImage_ConvertTo32Bits(fi_bitmap);
		assert(fi_image);
		int fi_width = FreeImage_GetWidth(fi_image);
		int fi_height = FreeImage_GetHeight(fi_image);
        
		auto texture = Texture::Create((void*)FreeImage_GetBits(fi_image), fi_width, fi_height, TEXTURE_FORMAT::BGRA_UBYTE, USAGE::DEFAULT, _context.get());
		assert(texture);
		FreeImage_Unload(fi_image);
		FreeImage_Unload(fi_bitmap);
        
        return texture;
    }
    
    std::shared_ptr<Material> AssetLoader::loadMaterial(const std::string& path_diffuse, const std::string& path_specular, const std::string& path_emissive) const {
        auto diffuse = loadTexture(path_diffuse);
        auto specular = path_specular == "" ? nullptr : loadTexture(path_specular);
        auto emissive = path_emissive == "" ? nullptr : loadTexture(path_emissive);
        
        auto material = Material::Create(diffuse, specular, emissive);
        return material;
    }

	std::shared_ptr<Bitmap> AssetLoader::loadBitmap(const std::string& path) const {
		return loadBitmap(path, -1.0f, -1.0f);
	}

	std::shared_ptr<Bitmap> AssetLoader::loadBitmap(const std::string& path, float width, float height) const {

		auto texture = loadTexture(path);
		assert(texture);

		auto bitmap  = Bitmap::Create(width  <= 0.0f ? (float) texture->getResource()->getWidth()  : width, 
									  height <= 0.0f ? (float) texture->getResource()->getHeight() : height,
									  texture, _context.get());
		assert(bitmap);

		return bitmap;
	}

	std::shared_ptr<Entity> AssetLoader::loadSceneWithoutAnimations(const std::string& path) const {
		// get parent directory of scene
		std::string filepath = _root + path;
		std::string scene_dir = path.substr(0, path.find_last_of("\\/")) + "/";
		for (unsigned i = 0; i < scene_dir.length(); i++) {
			if (scene_dir[i] == '\\') {
				scene_dir[i] = '/';
			}
		}

		// load the assimp scene
		Assimp::Importer importer;
		auto assimp_scene = importer.ReadFile(filepath,
			aiProcess_MakeLeftHanded |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);
		if (!assimp_scene)
			throw std::runtime_error("error: AssetLoader::loadSceneWithoutAnimations: failed to open file: " + filepath + ": " + importer.GetErrorString());

		auto assimp_global_inverse = assimp_scene->mRootNode->mTransformation; assimp_global_inverse.Inverse(); assimp_global_inverse.Transpose();
		glm::mat4 global_inverse(assimp_global_inverse.a1, assimp_global_inverse.a2, assimp_global_inverse.a3, assimp_global_inverse.a4,
			assimp_global_inverse.b1, assimp_global_inverse.b2, assimp_global_inverse.b3, assimp_global_inverse.b4,
			assimp_global_inverse.c1, assimp_global_inverse.c2, assimp_global_inverse.c3, assimp_global_inverse.c4,
			assimp_global_inverse.d1, assimp_global_inverse.d2, assimp_global_inverse.d3, assimp_global_inverse.d4);

		// material count
		auto num_materials = assimp_scene->mNumMaterials;
		// mesh count
		auto num_meshes = assimp_scene->mNumMeshes;

		// all materials
		std::unordered_map<unsigned, std::shared_ptr<Material>> material_map;
		// all meshes
		std::unordered_map<unsigned, std::shared_ptr<Mesh>> mesh_map;
		// all entities
		std::unordered_map<std::string, std::shared_ptr<Entity>> entity_map;

		// for all materials
		for (unsigned i = 0; i < num_materials; i++) {
			// create material
			auto assimp_material = assimp_scene->mMaterials[i];
			auto material = Material::Create();
			// store material
			material_map[i] = material;

			// load diffuse textures
			auto num_diffuse_textures = assimp_material->GetTextureCount(aiTextureType_DIFFUSE);
			for (unsigned j = 0; j < num_diffuse_textures; j++) {
				aiString texture_path;
				if (assimp_material->GetTexture(aiTextureType_DIFFUSE, j, &texture_path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
					try {
						// create texture
						std::string path = scene_dir + texture_path.C_Str();
						auto texture = loadTexture(path);
						assert(texture);
						// store texture
						material->setTextureDiffuse(texture);
						std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded TEXTURE_DIFFUSE" << j << ": " << _root + path << std::endl;
					}
					catch (std::runtime_error& err) {
						std::cerr << "error: AssetLoader::loadSceneWithoutAnimations: failed to load TEXTURE_DIFFUSE" << j << ": " << std::endl << "  " << err.what() << std::endl;
						material->setTextureDiffuse(nullptr);
					}
				}
			}

			// load specular textures
			auto num_specular_textures = assimp_material->GetTextureCount(aiTextureType_SPECULAR);
			for (unsigned j = 0; j < num_specular_textures; j++) {
				aiString texture_path;
				if (assimp_material->GetTexture(aiTextureType_SPECULAR, j, &texture_path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
					try {
						// create texture
						std::string path = scene_dir + texture_path.C_Str();
						auto texture = loadTexture(path);
						assert(texture);
						// store texture
						material->setTextureSpecular(texture);
						std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded TEXTURE_SPECULAR" << j << ": " << _root + path << std::endl;
					}
					catch (std::runtime_error& err) {
						std::cerr << "error: AssetLoader::loadSceneWithoutAnimations: failed to load TEXTURE_SPECULAR" << j << ": " << std::endl << "  " << err.what() << std::endl;
						material->setTextureSpecular(nullptr);
					}
				}
			}

			// load emissive textures
			auto num_emissive_textures = assimp_material->GetTextureCount(aiTextureType_EMISSIVE);
			for (unsigned j = 0; j < num_emissive_textures; j++) {
				aiString texture_path;
				if (assimp_material->GetTexture(aiTextureType_EMISSIVE, j, &texture_path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
					try {
						// create texture
						std::string path = scene_dir + texture_path.C_Str();
						auto texture = loadTexture(path);
						assert(texture);
						// store texture
						material->setTextureEmissive(texture);
						std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded TEXTURE_EMISSIVE" << j << ": " << _root + path << std::endl;
					}
					catch (std::runtime_error& err) {
						std::cerr << "error: AssetLoader::loadSceneWithoutAnimations: failed to load TEXTURE_EMISSIVE" << j << ": " << std::endl << "  " << err.what() << std::endl;
						material->setTextureEmissive(nullptr);
					}
				}
			}
		}

		// for all meshes
		for (unsigned i = 0; i < num_meshes; i++) {
			// create mesh
			auto assimp_mesh = assimp_scene->mMeshes[i];
			auto mesh = Mesh::Create(_context.get());
			assert(mesh);
			// store mesh
			mesh_map[i] = mesh;

			// load elements
			if (assimp_mesh->HasFaces()) {
				auto num_faces = assimp_mesh->mNumFaces;
				std::vector<unsigned> elements;
				for (unsigned j = 0; j < num_faces; j++) {
					aiFace assimp_face = assimp_mesh->mFaces[j];
					if (assimp_face.mNumIndices != 3) {
						std::cerr << "warning: AssetLoader::loadSceneWithoutAnimations: encountered non-triangulated ATTRIBUTE_ELEMENT" << std::endl;
						continue;
					}
					elements.push_back(assimp_face.mIndices[0]);
					elements.push_back(assimp_face.mIndices[1]);
					elements.push_back(assimp_face.mIndices[2]);
				}
				// create buffer
				auto element_buffer = AttributeBuffer::Create(&elements[0], (unsigned)elements.size(), ATTRIBUTE_ELEMENTS, FORMAT::UINT, USAGE::DEFAULT, _context.get());
				assert(element_buffer);
				// store buffer
				mesh->getAttributeArray()->setAttribute(element_buffer);
				std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded ATTRIBUTE_ELEMENT" << std::endl;
			}

			// load positions
			if (assimp_mesh->HasPositions()) {
				auto num_vertices = assimp_mesh->mNumVertices;
				std::vector<float> positions;
				for (unsigned j = 0; j < num_vertices; j++) {
					aiVector3D assimp_vert = assimp_mesh->mVertices[j];
					positions.push_back(assimp_vert.x);
					positions.push_back(assimp_vert.y);
					positions.push_back(assimp_vert.z);
				}
				// create buffer
				auto position_buffer = AttributeBuffer::Create(&positions[0], (unsigned)positions.size() / 3, ATTRIBUTE_POSITION, FORMAT::FLOAT3, USAGE::DEFAULT, _context.get());
				assert(position_buffer);
				// store buffer
				mesh->getAttributeArray()->setAttribute(position_buffer);
				std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded ATTRIBUTE_POSITION" << std::endl;
			}

			// load normals
			if (assimp_mesh->HasNormals()) {
				auto num_normals = assimp_mesh->mNumVertices;
				std::vector<float> normals;
				for (unsigned j = 0; j < num_normals; j++) {
					aiVector3D assimp_normal = assimp_mesh->mNormals[j];
					normals.push_back(assimp_normal.x);
					normals.push_back(assimp_normal.y);
					normals.push_back(assimp_normal.z);
				}
				// create buffer
				auto normal_buffer = AttributeBuffer::Create(&normals[0], (unsigned)normals.size() / 3, ATTRIBUTE_NORMAL, FORMAT::FLOAT3, USAGE::DEFAULT, _context.get());
				assert(normal_buffer);
				// store buffer
				mesh->getAttributeArray()->setAttribute(normal_buffer);
				std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded ATTRIBUTE_NORMAL" << std::endl;
			}

			// load tangents and bi-tangents
			if (assimp_mesh->HasTangentsAndBitangents()) {
				auto num_tangents = assimp_mesh->mNumVertices;
				std::vector<float> tangents;
				std::vector<float> bitangents;
				for (unsigned j = 0; j < num_tangents; j++) {
					aiVector3D assimp_tangent = assimp_mesh->mTangents[j];
					aiVector3D assimp_bitangent = assimp_mesh->mBitangents[j];
					tangents.push_back(assimp_tangent.x);
					tangents.push_back(assimp_tangent.y);
					tangents.push_back(assimp_tangent.z);
					bitangents.push_back(assimp_bitangent.x);
					bitangents.push_back(assimp_bitangent.y);
					bitangents.push_back(assimp_bitangent.z);
				}
				// create buffers
				auto tangent_buffer = AttributeBuffer::Create(&tangents[0], (unsigned)tangents.size() / 3, ATTRIBUTE_TANGENT, FORMAT::FLOAT3, USAGE::DEFAULT, _context.get());
				auto bitangent_buffer = AttributeBuffer::Create(&bitangents[0], (unsigned)bitangents.size() / 3, ATTRIBUTE_BITANGENT, FORMAT::FLOAT3, USAGE::DEFAULT, _context.get());
				assert(tangent_buffer);
				assert(bitangent_buffer);
				// store buffers
				mesh->getAttributeArray()->setAttribute(tangent_buffer);
				mesh->getAttributeArray()->setAttribute(bitangent_buffer);
				std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded ATTRIBUTE_TANGENT" << std::endl;
				std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded ATTRIBUTE_BITANGENT" << std::endl;
			}

			// load uv-coordinates
			if (assimp_mesh->HasTextureCoords(0)) {
				if (assimp_mesh->mNumUVComponents[0] == 2) {
					auto num_uvs = assimp_mesh->mNumVertices;
					std::vector<float> uvs;
					for (unsigned j = 0; j < num_uvs; j++) {
						aiVector3D assimp_uvw = assimp_mesh->mTextureCoords[0][j];
						uvs.push_back(assimp_uvw.x);
						uvs.push_back(assimp_uvw.y);
					}
					// create buffer
					auto uv_buffer = AttributeBuffer::Create(&uvs[0], (unsigned)uvs.size() / 2, ATTRIBUTE_UV, FORMAT::FLOAT2, USAGE::DEFAULT, _context.get());
					assert(uv_buffer);
					// store buffer
					mesh->getAttributeArray()->setAttribute(uv_buffer);
					std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded ATTRIBUTE_UV" << std::endl;
				}
			}

			// load colors
			if (assimp_mesh->HasVertexColors(0)) {
				auto num_colors = assimp_mesh->mNumVertices;
				std::vector<float> colors;
				for (unsigned j = 0; j < num_colors; j++) {
					aiColor4D assimp_color = assimp_mesh->mColors[0][j];
					colors.push_back(assimp_color.r);
					colors.push_back(assimp_color.g);
					colors.push_back(assimp_color.b);
					colors.push_back(assimp_color.a);
				}
				// create buffer
				auto color_buffer = AttributeBuffer::Create(&colors[0], (unsigned)colors.size() / 4, ATTRIBUTE_COLOR, FORMAT::FLOAT4, USAGE::DEFAULT, _context.get());
				assert(color_buffer);
				// store buffer
				mesh->getAttributeArray()->setAttribute(color_buffer);
				std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded ATTRIBUTE_COLOR" << std::endl;
			}

			// set material
			mesh->setMaterial(material_map[assimp_mesh->mMaterialIndex]);

			// load skeleton
			{
				// initialize a vector of weights and groups for each vertex
				std::vector<float> vertex_weights;
				std::vector<int> vertex_groups;
				std::vector<float> vertex_weights2;
				std::vector<int> vertex_groups2;
				for (unsigned j = 0; j < assimp_mesh->mNumVertices; j++) {
					for (unsigned k = 0; k < 4; k++) {
						vertex_weights.push_back(-1.0f);
						vertex_weights2.push_back(-1.0f);
						vertex_groups.push_back(-1);
						vertex_groups2.push_back(-1);
					}
				}

				// load bones
				if (assimp_mesh->HasBones()) {
					for (unsigned bone_id = 0; bone_id < assimp_mesh->mNumBones; bone_id++) {
						// get bone
						aiBone* assimp_bone = assimp_mesh->mBones[bone_id];
						std::string bone_name(assimp_bone->mName.data);
						// create weights
						for (unsigned j = 0; j < assimp_mesh->mBones[bone_id]->mNumWeights; j++) {
							int vertex_id = assimp_mesh->mBones[bone_id]->mWeights[j].mVertexId;
							float weight = assimp_mesh->mBones[bone_id]->mWeights[j].mWeight;

							bool found_slot = false;
							for (int slot_id = 0; slot_id < 4 && !found_slot; slot_id++) {
								if (vertex_weights[vertex_id * 4 + slot_id] < 0.0f) {
									vertex_weights[vertex_id * 4 + slot_id] = weight;
									vertex_groups[vertex_id * 4 + slot_id] = bone_id;
									found_slot = true;
								}
							}
							for (int slot_id = 0; slot_id < 4 && !found_slot; slot_id++) {
								if (vertex_weights2[vertex_id * 4 + slot_id] < 0.0f) {
									vertex_weights2[vertex_id * 4 + slot_id] = weight;
									vertex_groups2[vertex_id * 4 + slot_id] = bone_id;
									found_slot = true;
								}
							}
						}
					}
				}
				// normalize
				for (unsigned j = 0; j < assimp_mesh->mNumVertices; j++) {
					float norm = 0.0f;
					for (unsigned k = 0; k < 4; k++) {
						if (vertex_weights[j * 4 + k] > 0.0f) {
							norm += vertex_weights[j * 4 + k];
						}
						else {
							vertex_weights[j * 4 + k] = 0.0f;
						}
						if (vertex_weights2[j * 4 + k] > 0.0f) {
							norm += vertex_weights2[j * 4 + k];
						}
						else {
							vertex_weights2[j * 4 + k] = 0.0f;
						}
					}
					if (norm > 0.0f) {
						for (unsigned k = 0; k < 4; k++) {
							vertex_weights[j * 4 + k] /= norm;
							vertex_weights2[j * 4 + k] /= norm;
							if (vertex_weights[j * 4 + k] < 0.0f) {
								vertex_weights[j * 4 + k] = 0.0f;
							}
							if (vertex_weights2[j * 4 + k] < 0.0f) {
								vertex_weights2[j * 4 + k] = 0.0f;
							}
						}
					}
				}
				// create buffers
				auto weight_buffer = AttributeBuffer::Create(&vertex_weights[0], (unsigned)vertex_weights.size() / 4, ATTRIBUTE_WEIGHTS, FORMAT::FLOAT4, USAGE::DEFAULT, _context.get());
				auto group_buffer = AttributeBuffer::Create(&vertex_groups[0], (unsigned)vertex_groups.size() / 4, ATTRIBUTE_GROUPS, FORMAT::INT4, USAGE::DEFAULT, _context.get());
				auto weight_buffer2 = AttributeBuffer::Create(&vertex_weights2[0], (unsigned)vertex_weights2.size() / 4, ATTRIBUTE_WEIGHTS2, FORMAT::FLOAT4, USAGE::DEFAULT, _context.get());
				auto group_buffer2 = AttributeBuffer::Create(&vertex_groups2[0], (unsigned)vertex_groups2.size() / 4, ATTRIBUTE_GROUPS2, FORMAT::INT4, USAGE::DEFAULT, _context.get());
				assert(weight_buffer);
				assert(group_buffer);
				assert(weight_buffer2);
				assert(group_buffer2);
				// store buffers
				mesh->getAttributeArray()->setAttribute(weight_buffer);
				mesh->getAttributeArray()->setAttribute(group_buffer);
				mesh->getAttributeArray()->setAttribute(weight_buffer2);
				mesh->getAttributeArray()->setAttribute(group_buffer2);
				std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded ATTRIBUTE_WEIGHTS" << std::endl;
				std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded ATTRIBUTE_GROUPS" << std::endl;
				std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded ATTRIBUTE_WEIGHTS2" << std::endl;
				std::cout << "AssetLoader::loadSceneWithoutAnimations: loaded ATTRIBUTE_GROUPS2" << std::endl;
			}
		}

		// load all nodes
		std::function<void(aiNode*, const std::shared_ptr<Entity>)> buildEntityHierarchy = nullptr;
		buildEntityHierarchy = [&](aiNode* assimp_node, const std::shared_ptr<Entity>& entity) {
			// store entity
			entity_map[entity->getName()] = entity;

			// extract the transform
			auto assimp_matrix = assimp_node->mTransformation; assimp_matrix.Transpose();
			glm::mat4 matrix(assimp_matrix.a1, assimp_matrix.a2, assimp_matrix.a3, assimp_matrix.a4,
				assimp_matrix.b1, assimp_matrix.b2, assimp_matrix.b3, assimp_matrix.b4,
				assimp_matrix.c1, assimp_matrix.c2, assimp_matrix.c3, assimp_matrix.c4,
				assimp_matrix.d1, assimp_matrix.d2, assimp_matrix.d3, assimp_matrix.d4);
			// create transform
			auto transform = Transform3D::Create();
			assert(transform);
			// store transform
			// transform->setMatrix(matrix);
			entity->attach(transform);

			// get meshes for this entity
			if (assimp_node->mNumMeshes > 0) {
				// create mesh/bone collection
				auto mesh_collection = MeshCollection::Create();
				assert(mesh_collection);
				// store mesh collection
				entity->attach(mesh_collection);
				// store meshes
				for (unsigned j = 0; j < assimp_node->mNumMeshes; j++) {
					auto mesh = mesh_map[assimp_node->mMeshes[j]];
					mesh_collection->addMesh(mesh);
				}
			}

			// for all children in this node
			for (unsigned i = 0; i < assimp_node->mNumChildren; i++) {
				// create child
				auto assimp_child = assimp_node->mChildren[i];
				std::shared_ptr<Entity> child = Entity::Create(std::string(assimp_child->mName.data));
				assert(child);
				// store in hierarchy and recurse
				entity->addChild(child);
				buildEntityHierarchy(assimp_child, child);
			}
		};

		// create root entity
		auto assimp_root = assimp_scene->mRootNode;
		auto root = Entity::Create(std::string(assimp_root->mName.data));

		// create entity hierarchy
		buildEntityHierarchy(assimp_root, root);		

		return root;
	}

	std::shared_ptr<Entity> AssetLoader::loadSceneWithAnimations(const std::string& path) const {
		return nullptr;
	}
    
    bool AssetLoader::_FreeImage_Has_Been_Initialized = false;

}