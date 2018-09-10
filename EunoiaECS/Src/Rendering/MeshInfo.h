#pragma once

#include <vector>
#include "Vertex.h"

namespace Eunoia { namespace Rendering {

	struct SubMeshInfo
	{
		uint32 lastIndex;
		uint32 materialIndex;
	};

	struct MeshInfo
	{
		MeshInfo(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) :
			vertices(vertices),
			indices(indices)
		{
			SubMeshInfo submesh;
			submesh.lastIndex = indices.size() - 1;
			submesh.materialIndex = 0;
			submeshes.push_back(submesh);
		}

		MeshInfo() {}

		std::vector<Vertex> vertices;
		std::vector<uint32> indices;
		std::vector<SubMeshInfo> submeshes;
		//std::vector<Material> materials
	};

} }
