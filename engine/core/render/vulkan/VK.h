#pragma once

#include <engine/core/render/render/Renderer.h>

namespace Echo
{
	// �½�GLES��Ⱦ��
	void LoadVKRenderer(Renderer*& render);

	// ж��GLES��Ⱦ��
	void UnLoadVKRenderer(Renderer* render);
}