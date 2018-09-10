#pragma once

#include "../../Core/Common.h"
#include "../../Rendering/MeshInfo.h"
#include "../../Rendering/RenderApiEnums.h"
#include "RenderContextD3D11.h"
#include "../../Rendering/ResourceTypes.h"

namespace Eunoia { namespace Rendering {

	class MeshManagerD3D11 : private ResourceIDAssigner
	{
	public:
		MeshManagerD3D11();
		~MeshManagerD3D11();

		MeshID CreateMesh(const MeshInfo& meshInfo, MeshUsage vertexBufferUsage = MESH_USAGE_STATIC, MeshUsage indexBufferUsage = MESH_USAGE_STATIC);
		void DeleteMesh(MeshID meshID);
		void RenderMesh(MeshID meshID) const;

		Vertex* MapVertexBuffer(MeshID meshID) const;
		uint32* MapIndexBuffer(MeshID meshID) const;

		void UnmapVertexBuffer() const;
		void UnmapIndexBuffer() const;
	private:
		void InitInputLayout();

		static D3D11_INPUT_ELEMENT_DESC CreateInputElementDesc(const char* semanticName, DXGI_FORMAT format, uint32 byteOffset);
		static ID3DBlob* CreateDummyShader(D3D11_INPUT_ELEMENT_DESC* pInputElements, uint32 numInputElements);
	private:
		struct MeshInfoD3D11
		{
			ID3D11Buffer* pVertexBuffer;
			ID3D11Buffer* pIndexBuffer;
			std::vector<SubMeshInfo> submeshes;
		};
	private:
		RenderContextD3D11* m_pRenderContext;
		std::vector<MeshInfoD3D11> m_meshes;
		ID3D11InputLayout* m_pInputLayout;

		mutable MeshID m_mappedVertexBuffer;
		mutable MeshID m_mappedIndexBuffer;
	};

} }
