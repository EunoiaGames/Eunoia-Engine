#pragma once

#include "ResourceTypes.h"
#include "../DataStructures/String.h"
#include <vector>

namespace Eunoia { namespace Rendering {

	class MaterialManager : private ResourceIDAssigner
	{
	public:
		MaterialManager();
		~MaterialManager();

		MaterialID CreateMaterial(TextureID albedo);
		MaterialID CreateMaterial(const String& albedoFile);

		void DeleteMaterial(MaterialID materialID, bool deleteTextures);
		void BindMaterialToShader(MaterialID materialID, ShaderID shaderID) const;
	private:
		enum MaterialTextureType
		{
			MATERIAL_TEXTURE_ALBEDO,

			NUM_MATERIAL_TEXTURE_TYPES
		};

		struct Material
		{
			inline Material(TextureID albedo)
			{
				textures[MATERIAL_TEXTURE_ALBEDO] = albedo;
			}

			TextureID textures[NUM_MATERIAL_TEXTURE_TYPES];
		};
	private:
		std::vector<Material> m_materials;
	};

} }
