#include "MeshManagerD3D11.h"
#include <d3dcompiler.h>
#include <string>

namespace Eunoia { namespace Rendering { 

	MeshManagerD3D11::MeshManagerD3D11() :
		m_pRenderContext(RenderContextD3D11::GetRenderContext()),
		m_mappedVertexBuffer(EU_INVALID_MESH_ID),
		m_mappedIndexBuffer(EU_INVALID_MESH_ID)
	{
		InitInputLayout();
		m_pRenderContext->GetDeviceContext()->IASetInputLayout(m_pInputLayout);
	}

	MeshManagerD3D11::~MeshManagerD3D11()
	{
		for (uint32 i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i].pVertexBuffer->Release();
			m_meshes[i].pIndexBuffer->Release();
		}
	}

	MeshID MeshManagerD3D11::CreateMesh(const MeshInfo & meshInfo, MeshUsage vertexBufferUsage, MeshUsage indexBufferUsage)
	{
		MeshInfoD3D11 mesh;

		// Create vertex buffer

		D3D11_BUFFER_DESC vertex_buffer_desc {};
		vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertex_buffer_desc.ByteWidth = sizeof(Vertex) * meshInfo.vertices.size();
		vertex_buffer_desc.CPUAccessFlags = vertexBufferUsage == MESH_USAGE_STATIC ? 0 : D3D11_CPU_ACCESS_WRITE;
		vertex_buffer_desc.MiscFlags = 0;
		vertex_buffer_desc.StructureByteStride = 0;
		vertex_buffer_desc.Usage = (D3D11_USAGE)vertexBufferUsage;

		D3D11_SUBRESOURCE_DATA vertex_initial_data {};
		vertex_initial_data.pSysMem = &meshInfo.vertices[0];
		vertex_initial_data.SysMemPitch = 0;
		vertex_initial_data.SysMemSlicePitch = 0;

		HRESULT err = m_pRenderContext->GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_initial_data, &mesh.pVertexBuffer);
		EU_ASSERT(err == S_OK, "Error creating d3d11 vertex buffer");

		// Create index buffer

		D3D11_BUFFER_DESC index_buffer_desc {};
		index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		index_buffer_desc.ByteWidth = sizeof(uint32) * meshInfo.indices.size();
		index_buffer_desc.CPUAccessFlags = indexBufferUsage == MESH_USAGE_STATIC ? 0 : D3D11_CPU_ACCESS_WRITE;
		index_buffer_desc.MiscFlags = 0;
		index_buffer_desc.StructureByteStride = 0;
		index_buffer_desc.Usage = (D3D11_USAGE)indexBufferUsage;

		D3D11_SUBRESOURCE_DATA index_initial_data {};
		index_initial_data.pSysMem = &meshInfo.indices[0];
		index_initial_data.SysMemPitch = 0;
		index_initial_data.SysMemSlicePitch = 0;

		err = m_pRenderContext->GetDevice()->CreateBuffer(&index_buffer_desc, &index_initial_data, &mesh.pIndexBuffer);
		EU_ASSERT(err == S_OK, "Error creating d3d11 index buffer");

		for (uint32 i = 0; i < meshInfo.submeshes.size(); i++)
		{
			mesh.submeshes.push_back(meshInfo.submeshes[i]);
		}

		MeshID id = GetNewID();
		if (id - 1 == m_meshes.size())
			m_meshes.push_back(mesh);
		else
			m_meshes[id - 1] = mesh;
		return id;
	}

	void MeshManagerD3D11::DeleteMesh(MeshID meshID)
	{
		const MeshInfoD3D11& mesh = m_meshes[meshID - 1];
		mesh.pVertexBuffer->Release();
		mesh.pIndexBuffer->Release();
		FreeID(meshID);
	}

	void MeshManagerD3D11::RenderMesh(MeshID meshID) const
	{
		const MeshInfoD3D11& mesh = m_meshes[meshID - 1];
		
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		m_pRenderContext->GetDeviceContext()->IASetVertexBuffers(0, 1, &mesh.pVertexBuffer, &stride, &offset);
		m_pRenderContext->GetDeviceContext()->IASetIndexBuffer(mesh.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		m_pRenderContext->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		uint32 firstIndex = 0;
		for (uint32 i = 0; i < mesh.submeshes.size(); i++)
		{
			uint32 lastIndex = mesh.submeshes[i].lastIndex;
			m_pRenderContext->GetDeviceContext()->DrawIndexed(lastIndex - firstIndex + 1, firstIndex, 0);
			firstIndex = lastIndex + 1;
		}
	}

	Vertex * MeshManagerD3D11::MapVertexBuffer(MeshID meshID) const
	{
		if (m_mappedIndexBuffer != EU_INVALID_MESH_ID)
			return NULL;

		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		m_pRenderContext->GetDeviceContext()->Map(m_meshes[meshID - 1].pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
		m_mappedVertexBuffer = meshID;
		return (Vertex*)mapped_resource.pData;
	}

	uint32 * MeshManagerD3D11::MapIndexBuffer(MeshID meshID) const
	{
		if (m_mappedIndexBuffer != EU_INVALID_MESH_ID)
			return NULL;

		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		m_pRenderContext->GetDeviceContext()->Map(m_meshes[meshID - 1].pIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
		m_mappedIndexBuffer = meshID;
		return (uint32*)mapped_resource.pData;
	}

	void MeshManagerD3D11::UnmapVertexBuffer() const
	{
		if (m_mappedVertexBuffer == EU_INVALID_MESH_ID)
			return;

		m_pRenderContext->GetDeviceContext()->Unmap(m_meshes[m_mappedVertexBuffer - 1].pIndexBuffer, 0);
		m_mappedVertexBuffer = EU_INVALID_MESH_ID;
	}

	void MeshManagerD3D11::UnmapIndexBuffer() const
	{
		if (m_mappedIndexBuffer == EU_INVALID_MESH_ID)
			return;

		m_pRenderContext->GetDeviceContext()->Unmap(m_meshes[m_mappedIndexBuffer - 1].pIndexBuffer, 0);
		m_mappedIndexBuffer = EU_INVALID_MESH_ID;
	}

	void MeshManagerD3D11::InitInputLayout()
	{
		D3D11_INPUT_ELEMENT_DESC inputElements[2];
		inputElements[0] = CreateInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, offsetof(Vertex, Vertex::pos));
		inputElements[1] = CreateInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT, offsetof(Vertex, Vertex::texCoord));

		ID3DBlob* pShaderCode = CreateDummyShader(inputElements, 2);
		HRESULT err = m_pRenderContext->GetDevice()->CreateInputLayout(inputElements, 2, pShaderCode->GetBufferPointer(), pShaderCode->GetBufferSize(), &m_pInputLayout);
		EU_ASSERT(err == S_OK, "Error creating d3d11 input layout");

		if(pShaderCode)
			pShaderCode->Release();
	}

	D3D11_INPUT_ELEMENT_DESC MeshManagerD3D11::CreateInputElementDesc(const char* semanticName, DXGI_FORMAT format, uint32 byteOffset)
	{
		D3D11_INPUT_ELEMENT_DESC input_element_desc {};

		input_element_desc.SemanticName = semanticName;
		input_element_desc.SemanticIndex = 0;
		input_element_desc.Format = format;
		input_element_desc.InputSlot = 0;
		input_element_desc.AlignedByteOffset = byteOffset;
		input_element_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		input_element_desc.InstanceDataStepRate = 0;

		return input_element_desc;
	}

	ID3DBlob * MeshManagerD3D11::CreateDummyShader(D3D11_INPUT_ELEMENT_DESC * pInputElements, uint32 numInputElements)
	{
		ID3DBlob* pCode;
		ID3DBlob* pErrMsgs;

		String shader = "struct Input{";
		for (uint32 i = 0; i < numInputElements; i++)
		{
			const D3D11_INPUT_ELEMENT_DESC& input_element = pInputElements[i];

			const char* semanticName = input_element.SemanticName;

			switch (input_element.Format)
			{
			case DXGI_FORMAT_R32_FLOAT: shader += String("float v") + String(std::to_string(i).c_str()) + " : " + semanticName + ";"; break;
			case DXGI_FORMAT_R32G32_FLOAT: shader += String("float2 v") + String(std::to_string(i).c_str()) + " : " + semanticName + ";"; break;
			case DXGI_FORMAT_R32G32B32_FLOAT: shader += String("float3 v") + String(std::to_string(i).c_str()) + " : " + semanticName + ";"; break;
			case DXGI_FORMAT_R32G32B32A32_FLOAT: shader += String("float4 v") + String(std::to_string(i).c_str()) + " : " + semanticName + ";"; break;
			}
		}

		shader += "};struct Output{float4 pos:SV_POSITION;};Output VSMain(Input input){Output output;output.pos=float4(0,0,0,1);return output;}";
		D3DCompile(shader.C_Str(), shader.Length(), NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"VSMain", "vs_5_0", D3DCOMPILE_DEBUG, 0, &pCode, &pErrMsgs);

		if (pErrMsgs)
			pErrMsgs->Release();

		return pCode;
	}

} }
