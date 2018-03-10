#pragma once

#include <Rapidxml/rapidxml.hpp>
#include <engine/core/Geom/Box.h>
#include <engine/core/Math/Vector3.h>
#include "SubMesh.h"
#include <engine/core/Memory/MemAllocDef.h>

namespace Echo
{
	/**
	* ��̬ģ����ģ��
	*/
	class DynamicMesh;
	class DynamicSubMesh : public SubMesh
	{
	public:
		struct VertexDefine
		{
			bool	m_isUseNormal;
			bool	m_isUseVertexColor;
			bool	m_isUseDiffuseUV;

			VertexDefine()
				: m_isUseNormal(false), m_isUseVertexColor(false), m_isUseDiffuseUV(false)
			{}
		};

	public:
		DynamicSubMesh(DynamicMesh* mesh);
		virtual ~DynamicSubMesh() {}

		// ���ö����ʽ
		void setVertDefine(const VertexDefine& format);

		// ���ò���
		virtual void set(const String& params){}

		// ����ֵ
		void set(ui32 vertCount, const Byte* vertices, int vertexStride, ui32 indicesCount, const ui16* indices, const Box& box);

	protected:
		// ����
		virtual void update(ui32 delta);

		// ����
		virtual bool load();

	protected:
		bool			m_isNeedUpdateDataToGPU;		// �����Ƿ���Ҫ�ϴ���GPU
		DynamicMesh*	m_dynamicMesh;					// ����ģ��
	};

	/**
	* ����������ģ��
	*/
	class DynamicSubMeshLineStrip : public DynamicSubMesh
	{
	public:
		DynamicSubMeshLineStrip(DynamicMesh* mesh);
		virtual ~DynamicSubMeshLineStrip() {}

		// ���ݲ���
		virtual void set(const String& params);

		// ���ò���
		void set(const vector<Vector3>::type& positions, const vector<Vector2>::type* uvs, const vector<Vector2>::type* colors);
	};

	/**
	 * ���ζ�̬��ģ��
	 */
	class DynamicSubMeshSphere : public DynamicSubMesh
	{
	public:
		// �����ʽ
		struct Vertex
		{
			Vector3 position;		// λ��
			Vector3 normal;			// ����
			Vector2 uv;				// UV

			// ���캯��
			Vertex(const Vector3& p, const Vector3& n, const Vector2& t)
				: position(p), normal(n), uv(t)
			{}
		};

	public:
		DynamicSubMeshSphere(DynamicMesh* mesh);
		virtual ~DynamicSubMeshSphere();

		// ���ݲ���
		virtual void set(const String& params);

		// ���ݲ���
		void set(const Vector3& center, float radius, int latitude, int longitude);

	private:
		// ��������
		void fghCircleTable(Real **sint, Real **cost, const int n, const bool halfCircle);
		void fghGenerateSphere(Real radius, int slices, int stacks, vector<Vertex>::type& buffer);
	};
}
