#pragma once

#include "render/RenderInput.h"

namespace Echo
{
	// ��������
	struct VertexData
	{
		typedef RenderInput::VertexElementList RIVEL;

		ui32		m_count;				// ��������
		bool		m_isUseNormal;			// �Ƿ�ʹ�÷���
		bool		m_isUseVertexColor;		// �Ƿ�ʹ�ö���ɫ
		bool		m_isUseDiffuseUV;		// �Ƿ�ʹ����������ͼUV 
		bool		m_isUseLightmapUV;		// �Ƿ�ʹ�ù���ͼUV
		bool		m_isUseBoneData;		// �Ƿ�ʹ�ù�������(����Ȩ��)
		bool		m_isUseTangentBinormal;	// �Ƿ�ʹ�������븱��
		ui32		m_stride;				// �����ʽ��С
		Byte		m_posOffset;			// ����λ������ƫ����
		Byte		m_normalOffset;			// ���㷨������ƫ����
		Byte		m_colorOffset;			// ������ɫ����ƫ����
		Byte		m_uv0Offset;			// ������������0ƫ����
		Byte		m_uv1Offset;			// ������������1ƫ����
		Byte		m_boneIndicesOffset;	// �����������ƫ����
		Byte		m_boneWeightsOffset;	// �������Ȩ��ƫ����
		Byte		m_tangentOffset;		// ��������ƫ����
		RIVEL		m_vertexElements;		// �����ʽ
		Byte*		m_vertices;				// ��������

		// ���캯��
		VertexData();

		// ����ƫ����,�����ʽ
		void build();

		// ��ȡ�����ʽ��С
		ui32 getVertexStride() const;

		// ��ȡ��������
		ui32 getVertexCount() const;

		// ��ȡ��������
		Byte* getVertices() const;

		// �ж϶����ʽ���Ƿ���ָ�����͵�����
		bool isVertexUsage(RenderInput::VertexSemantic semantic) const;

		// ��ȡ����λ������
		Vector3& getPosition(Word index);

		// ��ȡ���㷨������
		const Vector3& getNormal(Word index);

		// ��ȡ������ɫ����
		Dword& getColor(Word index);

		// ��ȡ����UV����0
		const Vector2& getUV0(Word index);

		// ��ȡ����UV����1
		const Vector2& getUV1(Word index);

		// ��ȡ����
		Vector3& getTangent(Word index);

		//״̬�ÿ�
		void reset();
	};
}