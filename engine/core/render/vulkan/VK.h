#pragma once

#include <Render/Renderer.h>

namespace LORD
{
	// �½�GLES��Ⱦ��
	void LoadGLESRenderer(Renderer*& render);

	// ж��GLES��Ⱦ��
	void UnLoadGLESRenderer(Renderer* render);
}