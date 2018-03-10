#include "GLES2.h"
#include "GLES2Render.h"

namespace LORD
{
	// �½�GLES��Ⱦ��
	void LoadGLESRenderer(Renderer*& render)
	{
		render = LORD_NEW(GLES2Renderer);
	}

	// ж��GLES��Ⱦ��
	void UnLoadGLESRenderer(Renderer* render)
	{
		LordSafeDelete(render);
	}
}