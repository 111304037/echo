#pragma once

#include <engine/core/render/render/Renderer.h>

namespace Echo
{
	// �½�GLES��Ⱦ��
	void LoadGLESRenderer(Renderer*& render);

	// ж��GLES��Ⱦ��
	void UnLoadGLESRenderer(Renderer* render);
}