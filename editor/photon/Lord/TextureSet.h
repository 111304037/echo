#ifndef _TEXTURESET_H_
#define _TEXTURESET_H_

#include <vector>
#include <string>
#include "Color.h"
#include "Vector4.h"

using namespace std;

namespace Lightmass
{
	// Rectangle
	template< typename T> 
	struct TRect
	{
		T left,	top, width, height;

		// ���캯��
		TRect() 
			: left(0), top(0), width(0), height(0)
		{}

		// ���캯��
		TRect( T const& _left, T const& _top, T const& _width, T const& _height)
			: left(_left), top(_top), width(_width), height(_height) 
		{}

		// ��ȡ���
		T getArea() const { return width * height; }
	};

	//------------------------------------------------------
	// �����-��С������ɵĴ����� 2012-7-20 ����
	// װ���㷨:
	// http://www.blackpawn.com/texts/lightmaps/default.html
	//-------------------------------------------------------
	class TextureSet
	{
		typedef TRect<int> IRect;
	public:
		// �ӵ�
		struct Node
		{
			int			m_id;				// ID>=0��ʶ�ý����ʹ��
			int			m_child[2];			// ��¼�ӽ��ID
			IRect		m_rc;

			// ���캯��
			Node();

			// �Ƿ�ΪҶ���
			bool IsLeaf() const;
		};

	public:
		TextureSet();
		TextureSet(int width, int height);
		~TextureSet();

		// �������� INVALID ��Ч����
		int Insert(Color* data, int width, int height);

		// ��д����
		int OverWrite( int nodeIdx, Color* data, int width, int height);

		// ��ȡ��ǰ��������
		//const Texture* GetTexture() const { return m_texture; }

		// ��ȡ������Viewport
		const Vector4 GetViewport( int nodeIdx) const;  

		// ��ȡ�����Ϣ
		const Node& GetNode( int nodeIdx) const { return m_nodes[nodeIdx]; }

		// ��ȡ��
		int GetWidth() const { return m_width; }

		// ��ȡ��
		int GetHeight() const { return m_height; }
		
		// ����
		void Load( const char* filePath);

		// ����
		void Save( const char* filePath);

	private:
		// ��������
		int Insert(  int nodeIdx, Color* data, int width, int height);

	private:
		static const string	s_TextureName;
		static ui32			s_TextureSetID;
		int					m_width;			// �����
		int					m_height;			// �����
		//Texture*			m_texture;			// �����������
		vector<Node>		m_nodes;			// ���ڵ�(����0Ϊ���ڵ�)
		void*				m_TextureData;					
	};
}

#endif