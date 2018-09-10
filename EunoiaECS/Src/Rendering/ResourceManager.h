#pragma once

#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"

namespace Eunoia { namespace Rendering {

	struct ResourceManager
	{
		inline static void InitResourceManager()
		{
			pMeshManager = new MeshManager();
			pShaderManager = new ShaderManager();
			pTextureManager = new TextureManager();
			pMaterialManager = new MaterialManager();
		}

		inline static void DestroyResourceManager()
		{
			delete pMeshManager;
			delete pShaderManager;
			delete pTextureManager;
			delete pMaterialManager;
		}

		static MeshManager* pMeshManager;
		static ShaderManager* pShaderManager;
		static TextureManager* pTextureManager;
		static MaterialManager* pMaterialManager;
	};

} }
