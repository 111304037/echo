 #include "Engine/modules/Model/SkinnedMesh.h"
 #include "engine/core/Util/LogManager.h"
 #include "engine/core/Util/PathUtil.h"
 #include "Engine/core/Render/TextureResManager.h"
 #include "Render/Renderer.h"
 #include "engine/core/resource/DataStream.h"
 #include "Render/PixelFormat.h"
 #include "Render/GPUBuffer.h"
 
 namespace Echo
 {
 	// ���캯��
 	SkinnedMesh::SkinnedMesh(const String& name)
 		: Mesh(name)
 	{
 	}
 
	// ��������
 	SkinnedMesh::~SkinnedMesh()
 	{
 	}
 
	// ����
	bool SkinnedMesh::prepareImpl(DataStream* stream)
 	{
		return prepareImplInteral(stream, true);
 	}
 }