#include <Foundation/FileIO/ResourceGroupManager.h>
#include <Engine/Render/TextureResManager.h>
#include "spine/extension.h"

// ��������ӿ�ʵ��
void _spAtlasPage_createTexture(spAtlasPage* self, const char* path)
{
	ECHO::TextureRes* texRes = ECHO::TextureResManager::instance()->createTexture( path, ECHO::Texture::TU_STATIC);
	texRes->prepareLoad();

	self->rendererObject = texRes;
}

// ��������
void _spAtlasPage_disposeTexture(spAtlasPage* self)
{
	ECHO::TextureRes* texRes = static_cast<ECHO::TextureRes*>(self->rendererObject);
	ECHO::TextureResManager::instance()->releaseResource(texRes);
}

// ���ļ��ӿ�
char* _spUtil_readFile(const char* path, int* length)
{
	ECHO::DataStream* stream = ECHO::ResourceGroupManager::instance()->openResource(path);
	if (stream)
	{
		char* data = MALLOC(char, stream->size()+1);
		*length = stream->size();

		stream->read(data, stream->size());
		data[stream->size()] = '\0';

		return data;
	}
	else
	{
		*length = 0;
		return NULL;
	}
}