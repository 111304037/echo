#include "GLES2.h"
#include "GLES2Render.h"

namespace Echo
{
	// �½�GLES��Ⱦ��
	void LoadGLESRenderer(Renderer*& render)
	{
		render = EchoNew(GLES2Renderer);
	}

	// ж��GLES��Ⱦ��
	void UnLoadGLESRenderer(Renderer* render)
	{
		EchoSafeDelete(render, Renderer);
	}
}