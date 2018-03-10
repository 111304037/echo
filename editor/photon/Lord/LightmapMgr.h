#ifndef _LIGHTMAPMGR_H_
#define _LIGHTMAPMGR_H_

#include <map>
#include <string>
#include <vector>
#include "TextureSet.h"

using namespace std;

namespace Lightmass
{
	//---------------------------------
	// ��������ͼ������ 2012-7-7
	//     ���,ɾ��,�ϲ�,����
	//---------------------------------
	class LightmapMgr
	{
	public:
		// TextureTile
		struct TileInfo
		{
			string  m_uniqueName;	// Ψһ��ʶ
			int		m_setIdx;		// ��������
			int		m_tileId;		// �����ID
			Vector3 m_scale;		// ������������ֵ
		};
		typedef map<string, TileInfo> TileInfoMap;

	public:
		LightmapMgr();
		LightmapMgr( int width, int height);

		// ��ӹ���ͼ(��ʵ������mesh��������)
		bool AddLightmap( const char* uniqueName, Color* texture, int width, int height, const Vector3& scale);

		// ��ѯ
		//bool Query( const char* uniqueName, const Texture*& lightmap, Vector4& lightmapViewPort);

		// �������
		void Clear();

		// ����
		bool Load();

		// ����
		bool Save(const string& locationPath, const string& sceneName, int num);

		ui32 getTextureSetNum() const;
		//// ����xml��ʽ
		//bool LoadXml( const char* locationPath);

		//// ����xml��ʽ
		//bool SaveXml( const char* locationPath);

	private:
		// �½�����
		TextureSet* AllocateTextureSet( int width, int height);

		// ��ӵ�ָ������
		bool AddLightmapToTextureSet( TextureSet* texSet, int setIdx, const char* uniqueName, Color* texture, int width, int height, const Vector3& scale);

		// ��д����
		bool OverWriteLightmapToTextureSet( TextureSet* texSet, int tileId, Color* texture, int width, int height);

		// ��ȡ���ʵ�������
		int calcTexSetLen( int texLen);

	private:
		int							m_tsWidth;		// TextureSet��
		int							m_tsHeight;		// TextureSet��
		TileInfoMap					m_tileInfos;	// ����Ϣ
		vector<TextureSet*>			m_texSets;		// ����
	};
}

#endif