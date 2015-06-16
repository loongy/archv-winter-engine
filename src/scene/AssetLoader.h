/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_ASSERTLOADER_H_
#define _WNTR_ASSERTLOADER_H_

#include <memory>
#include <string>
#include <vector>

namespace wntr {

	class Bitmap;
	class Entity;
    class Material;
	class Mesh;
	class RenderContext;
	class Texture;

	class AssetLoader {
	public:
        static std::shared_ptr<AssetLoader> Create(const std::shared_ptr<RenderContext>& context);
		static std::shared_ptr<AssetLoader> Create(const std::string& root, const std::shared_ptr<RenderContext>& context);

#ifndef _MSC_VER
		AssetLoader(AssetLoader&&) = default;
		AssetLoader& operator=(AssetLoader&&) = default;
#endif
		AssetLoader(const AssetLoader&) = delete;
		AssetLoader& operator=(const AssetLoader&) = delete;

		explicit AssetLoader(const std::string& root, const std::shared_ptr<RenderContext>& context);
		virtual ~AssetLoader() = default;

        std::shared_ptr<Texture> loadTexture(const std::string& path) const;
        std::shared_ptr<Material> loadMaterial(const std::string& path_diffuse, const std::string& path_specular="", const std::string& path_emissive="") const;
		std::shared_ptr<Bitmap> loadBitmap(const std::string& path) const;
		std::shared_ptr<Bitmap> loadBitmap(const std::string& path, float width, float height) const;
		std::shared_ptr<Entity> loadSceneWithoutAnimations(const std::string& path) const;
		std::shared_ptr<Entity> loadSceneWithAnimations(const std::string& path) const;

	protected:

	private:
        static bool _FreeImage_Has_Been_Initialized;
        
		std::string _root;
        std::shared_ptr<RenderContext> _context;

	};

}

#endif