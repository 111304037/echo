#pragma once

#include <engine/core/memory/MemManager.h>
#include "PixelFormat.h"

namespace Echo
{
	class RenderView
	{
	public:
		enum RenderViewType
		{
			RVT_RENDERTARGET, 
			RVT_DEPTHSTENCIL, 
		};

	public:
		RenderView(RenderViewType type, PixelFormat pixFmt);
		virtual ~RenderView();

		// ��ȡ��Ⱦ�ӽ�����
		virtual RenderViewType getRenderViewType() const;

		// ��ȡ��������
		virtual PixelFormat	getPixelFormat() const;

	protected:
		RenderViewType		m_type;
		PixelFormat			m_pixFmt;
	};
}
