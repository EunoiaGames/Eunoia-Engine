#include "MaterialManager.h"
#include "ResourceManager.h"

namespace Eunoia { namespace Rendering {

	MaterialManager::MaterialManager()
	{
	}

	MaterialManager::~MaterialManager()
	{
		
	}

	MaterialID MaterialManager::CreateMaterial(TextureID albedo)
	{
		for (uint32 i = 0; i < m_materials.size(); i++)
		{
			if (albedo == m_materials[i].textures[MATERIAL_TEXTURE_ALBEDO]) // already exists
				return i + 1;
		}

		MaterialID id = GetNewID();

		if (id - 1 == m_materials.size())
			m_materials.push_back(Material(albedo));
		else
			m_materials[id - 1] = Material(albedo);
		return id;
	}

	MaterialID MaterialManager::CreateMaterial(const String & albedoFile)
	{
		TextureID albedoID = ResourceManager::pTextureManager->CreateTexture2D(albedoFile);
		return CreateMaterial(albedoID);

	}

	void MaterialManager::DeleteMaterial(MaterialID materialID, bool deleteTextures)
	{
		FreeID(materialID);

		if (deleteTextures)
			for (uint32 i = 0; i < NUM_MATERIAL_TEXTURE_TYPES; i++)
				ResourceManager::pTextureManager->DeleteTexture(m_materials[materialID - 1].textures[i]);
	}

	void MaterialManager::BindMaterialToShader(MaterialID materialID, ShaderID shaderID) const
	{
		ResourceManager::pShaderManager->BindTextureToShader(shaderID, m_materials[materialID - 1].textures[MATERIAL_TEXTURE_ALBEDO], "Albedo");
	}

} }
