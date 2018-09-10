#pragma once

#include "ResourceTypes.h"
#include "../Math/Math.h"
#include <vector>

namespace Eunoia { namespace Rendering {

	struct Renderable
	{
		MeshID meshID;
		MaterialID materialID;
		Math::Matrix4f transform;
	};

	class RenderingEngine
	{
	public:
		virtual ~RenderingEngine() {}

		virtual void Render() = 0;

		void PreRender()
		{
			m_renderables.clear();
		}

		void SubmitRenderable(const Renderable& renderable)
		{
			//Sort by materials
			bool added = false;
			for (uint32 i = 0; i < m_renderables.size(); i++)
			{
				if (renderable.materialID < m_renderables[i].materialID)
				{
					m_renderables.insert(m_renderables.begin() + i, renderable);
					added = true;
					break;
				}
			}
			if (!added)
				m_renderables.push_back(renderable);
		}

		inline void SetViewProjection(const Math::Matrix4f& view, const Math::Matrix4f& projection)
		{
			m_viewMatrix = view;
			m_projectionMatrix = projection;
		}

	protected:
		Math::Matrix4f m_viewMatrix;
		Math::Matrix4f m_projectionMatrix;

		std::vector<Renderable> m_renderables;
	};
} }
