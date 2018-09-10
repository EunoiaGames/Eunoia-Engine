#include "MeshManagerGL.h"

namespace Eunoia { namespace Rendering {

	MeshManagerGL::MeshManagerGL() :
		m_isVertexBufferMapped(false),
		m_isIndexBufferMapped(false)
	{}

	MeshManagerGL::~MeshManagerGL()
	{
		for (uint32 i = 0; i < m_meshes.size(); i++)
		{
			const MeshInfoGL& mesh = m_meshes[i];
			glDeleteVertexArrays(1, &mesh.vao);
			glDeleteBuffers(1, &mesh.vbo);
			glDeleteBuffers(1, &mesh.ibo);
		}
	}

	MeshID MeshManagerGL::CreateMesh(const MeshInfo & meshInfo, MeshUsage vertexBufferUsage, MeshUsage indexBufferUsage)
	{
		MeshInfoGL mesh;
		glGenVertexArrays(1, &mesh.vao);
		glGenBuffers(1, &mesh.vbo);
		glGenBuffers(1, &mesh.ibo);

		glBindVertexArray(mesh.vao);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * meshInfo.vertices.size(), &meshInfo.vertices[0], vertexBufferUsage);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * meshInfo.indices.size(), &meshInfo.indices[0], indexBufferUsage);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Vertex::pos));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Vertex::texCoord));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

	void MeshManagerGL::DeleteMesh(MeshID meshID)
	{
		MeshInfoGL& mesh = m_meshes[meshID - 1];
		glDeleteVertexArrays(1, &mesh.vao);
		glDeleteBuffers(1, &mesh.vbo);
		glDeleteBuffers(1, &mesh.ibo);
		mesh.submeshes.resize(0);

		FreeID(meshID);
	}

	void MeshManagerGL::RenderMesh(MeshID meshID) const
	{
		const MeshInfoGL& mesh = m_meshes[meshID - 1];
		glBindVertexArray(mesh.vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

		uint32 startIndex = 0;
		for (uint32 i = 0; i < mesh.submeshes.size(); i++)
		{
			uint32 lastIndex = mesh.submeshes[i].lastIndex;
			glDrawElements(GL_TRIANGLES, lastIndex - startIndex + 1, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * startIndex));
			startIndex = lastIndex + 1;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	Vertex * MeshManagerGL::MapVertexBuffer(MeshID meshID) const
	{
		if (m_isVertexBufferMapped)
			return NULL;

		m_isVertexBufferMapped = true;
		glBindBuffer(GL_ARRAY_BUFFER, m_meshes[meshID - 1].vbo);
		return (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	uint32 * MeshManagerGL::MapIndexBuffer(MeshID meshID) const
	{
		if (m_isIndexBufferMapped)
			return NULL;

		m_isIndexBufferMapped = true;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshes[meshID - 1].ibo);
		return (uint32*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void MeshManagerGL::UnmapVertexBuffer() const
	{
		if (!m_isVertexBufferMapped)
			return;

		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_isVertexBufferMapped = false;
	}

	void MeshManagerGL::UnmapIndexBuffer() const
	{
		if (!m_isIndexBufferMapped)
			return;

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		m_isIndexBufferMapped = false;
	}

} }
