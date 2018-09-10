#pragma once

#include "../../Core/Common.h"
#include "../../Rendering/MeshInfo.h"
#include <GL\glew.h>
#include "../../Rendering/RenderApiEnums.h"
#include "../../Rendering/ResourceTypes.h"

namespace Eunoia { namespace Rendering {

	class MeshManagerGL : private ResourceIDAssigner
	{
	public:
		MeshManagerGL();
		~MeshManagerGL();

		MeshID CreateMesh(const MeshInfo& meshInfo, MeshUsage vertexBufferUsage = MESH_USAGE_STATIC, MeshUsage indexBufferUsage = MESH_USAGE_STATIC);
		void DeleteMesh(MeshID meshID);
		void RenderMesh(MeshID meshID) const;

		Vertex* MapVertexBuffer(MeshID meshID) const;
		uint32* MapIndexBuffer(MeshID meshID) const;

		void UnmapVertexBuffer() const;
		void UnmapIndexBuffer() const;
	private:
		struct MeshInfoGL
		{
			GLuint vao;
			GLuint vbo;
			GLuint ibo;
			std::vector<SubMeshInfo> submeshes;
		};
	private:
		std::vector<MeshInfoGL> m_meshes;

		mutable bool m_isVertexBufferMapped;
		mutable bool m_isIndexBufferMapped;
	};

} }
