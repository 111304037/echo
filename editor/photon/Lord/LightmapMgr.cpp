#include "LightmapMgr.h"
#include "TextureSet.h"
#include <ostream>
#include <fstream>
#include "DataStream.h"

#define INVALID -1

namespace Lightmass
{
	// ���캯��
	LightmapMgr::LightmapMgr()
		: m_tsWidth( 512)
		, m_tsHeight( 512)
	{}

	// ���캯��
	LightmapMgr::LightmapMgr( int width, int height)
		: m_tsWidth( width)
		, m_tsHeight( height)
	{}

	// ��ӹ���ͼ(��ʵ������mesh��������)
	bool LightmapMgr::AddLightmap( const char* uniqueName, Color* texture, int width, int height, const Vector3& scale)
	{
		// 1.���Ѵ���,��д����
		TileInfoMap::iterator it = m_tileInfos.find( uniqueName);
		if( it != m_tileInfos.end())
		{
			TileInfo& tInfo = it->second;
			tInfo.m_scale = scale;

			return OverWriteLightmapToTextureSet( m_texSets[tInfo.m_setIdx], tInfo.m_tileId, texture, width, height);
		}

		// 2.��������ʣ��ռ�
		for ( size_t i=0; i<m_texSets.size(); i++)
		{
			if( AddLightmapToTextureSet( m_texSets[i], i, uniqueName, texture, width, height, scale))
				return true;
		}

		// 3.�½�TextureSet ������
		int tsWidth = width > m_tsWidth ? calcTexSetLen(width) : m_tsWidth;
		int tsHeight = height > m_tsHeight ? calcTexSetLen(height) : m_tsHeight;
		TextureSet* texSet = AllocateTextureSet( tsWidth, tsHeight);
		LordAssert( texSet);
		m_texSets.push_back( texSet);

		return AddLightmapToTextureSet( m_texSets.back(), m_texSets.size()-1, uniqueName, texture, width, height, scale);
	}

	// ��ѯ
	//bool LightmapMgr::Query( const char* uniqueName, const Texture*& lightmap, Vector4& lightmapViewPort)
	//{
		//TileInfoMap::const_iterator it = m_tileInfos.find( uniqueName);
		//if( it != m_tileInfos.end())
		//{
		//	int	texIdx = it->second.m_setIdx;
		//	int	tileId = it->second.m_tileId;

		//	lightmap		 = m_texSets[texIdx]->GetTexture();
		//	lightmapViewPort = m_texSets[texIdx]->GetViewport(tileId);

		//	return true;
		//}

	//	return false;
	//}

	// ��ӵ�ָ������
	bool LightmapMgr::AddLightmapToTextureSet( TextureSet* texSet, int setIdx, const char* uniqueName, Color* texture, int width, int height, const Vector3& scale)
	{
		if( !texSet)
			return false;

		int tileID = texSet->Insert(texture, width, height);
		if( tileID != INVALID)
		{
			TileInfo tInfo;
			tInfo.m_uniqueName = uniqueName;
			tInfo.m_scale  = scale;
			tInfo.m_setIdx = setIdx;
			tInfo.m_tileId = tileID;

			m_tileInfos[uniqueName] = tInfo;

			return true;
		}
		
		return false;
	}

	// ��д����
	bool LightmapMgr::OverWriteLightmapToTextureSet( TextureSet* texSet, int tileId, Color* texture, int width, int height)
	{
		int tileIdx = texSet->OverWrite( tileId, texture, width, height);
		if( tileIdx!=INVALID)
			return true;

		return false;
	}

	// �������
	void LightmapMgr::Clear()
	{
		m_tileInfos.clear();

		for( size_t i=0; i<m_texSets.size(); i++)
			SAFE_DELETE( m_texSets[i]);

		m_texSets.clear();
	}

	// �½�����
	TextureSet* LightmapMgr::AllocateTextureSet( int width, int height)
	{
		TextureSet* texSet = NULL;

		texSet  = ::new TextureSet(width, height);

		return texSet;
	}

	// ���ر��渨�� TileInfo
	struct TileInfoLS
	{
		char	m_uniqueName[64];	// Ψһ��ʶ
		int		m_setIdx;			// ��������
		int		m_tileId;			// �����ID
		Vector3 m_scale;		// ������������ֵ

		// ���캯��
		TileInfoLS()
			: m_setIdx(0), m_tileId(0)
		{
			m_uniqueName[0] = '\0';
		}
	};

	// ����
	bool LightmapMgr::Load()
	{
		/*String name = LordSceneManager->getCurrentScene()->getSceneName() + ".lmb";
		DataStream* dataStream = ResourceGroupManager::Instance()->openResource(name);

		if (dataStream)
		{
			size_t tileInfSize = 0;
			dataStream->read(&tileInfSize, sizeof(size_t));
			TileInfoLS tileInfoLS;
			for (size_t i=0; i<tileInfSize; ++i)
			{
				dataStream->read(&tileInfoLS, sizeof(TileInfoLS));
				TileInfo info;
				info.m_uniqueName = tileInfoLS.m_uniqueName;
				info.m_setIdx = tileInfoLS.m_setIdx;
				info.m_tileId = tileInfoLS.m_tileId;
				info.m_scale  = tileInfoLS.m_scale;

				m_tileInfos[info.m_uniqueName] = info;
			}
			int texSets = 0;
			dataStream->read(&texSets, sizeof(int));

			char* texsetPath = static_cast<char*>(LordMalloc(64));
			for ( int i=0; i<texSets; i++)
			{
				dataStream->read(texsetPath, sizeof(char)*64);

				TextureSet* texSet = LordNew(TextureSet);
				texSet->Load(String(texsetPath).c_str());
				m_texSets.push_back(texSet);
			}
			LordDelete(dataStream);*/
		//	return true;
		//}

		return false;
	}

	// ����
	bool LightmapMgr::Save(const string& locationPath, const string& sceneName, int num)
	{
		string rootPath = locationPath + "\\" + sceneName + ".lmb";
		std::fstream f;
		f.open(rootPath.c_str(), std::ios::out|std::ios::binary|std::ios::trunc);
		DataStream* filePtr( new FileStreamDataStream(&f, false));
		if (filePtr)
		{
			int tileInfSize = (int)m_tileInfos.size();
			filePtr->write(&tileInfSize, sizeof(int));
			TileInfoLS tileInfoLS;
			for ( TileInfoMap::iterator it=m_tileInfos.begin(); it!=m_tileInfos.end(); it++)
			{
				strcpy( tileInfoLS.m_uniqueName, it->second.m_uniqueName.c_str());
				tileInfoLS.m_setIdx = it->second.m_setIdx;
				tileInfoLS.m_tileId = it->second.m_tileId;
				tileInfoLS.m_scale  = it->second.m_scale;

				// ����TileInfo
				filePtr->write(&tileInfoLS, sizeof(tileInfoLS));
			}
			// ��������˳��
			int texSets = (int)m_texSets.size();
			filePtr->write(&texSets, sizeof(texSets));

			char texsetPath[64];
			for ( size_t i=0; i<m_texSets.size(); i++)
			{
				sprintf(texsetPath, (sceneName + "lightmap_%d_%d.ts").c_str(), num, i);
				filePtr->write(texsetPath, sizeof(char)*64);
			}

			// ����textureset
			int index =	rootPath.find_last_of("\\");
			string newScenePath = rootPath.substr(0, index);
			for ( size_t i=0; i<m_texSets.size(); i++)
			{
				string tsPath;
				sprintf(texsetPath, (sceneName + "lightmap_%d_%d.ts").c_str(), num,i);
				m_texSets[i]->Save((newScenePath + "\\" + texsetPath).c_str());
			}

			SAFE_DELETE( filePtr);

			f.close();

			return true;
		}

		return false;
	}

	ui32 LightmapMgr::getTextureSetNum() const
	{
		return m_texSets.size();
	}

	// ��ȡ���ʵ�������
	int LightmapMgr::calcTexSetLen(int texLen)
	{
		vector<int> choices = { 32, 64, 128, 256, 512, 1024, 2048};
		for (size_t i = 0; i < choices.size(); i++)
		{
			if (texLen <= choices[i])
				return choices[i];
		}

		return 4096;
	}
}