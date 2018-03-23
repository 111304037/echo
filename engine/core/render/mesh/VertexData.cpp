#include "VertexData.h"

namespace Echo
{
	// ���캯��
	VertexData::VertexData()
		: m_count(0)
		, m_isUseNormal(false)
		, m_isUseVertexColor(false)
		, m_isUseDiffuseUV(false)
		, m_isUseLightmapUV(false)
		, m_isUseBoneData(false)
		, m_isUseTangentBinormal(false)
		, m_stride(0)
		, m_posOffset(0)
		, m_normalOffset(0)
		, m_colorOffset(0)
		, m_uv0Offset(0)
		, m_uv1Offset(0)
		, m_boneIndicesOffset(0)
		, m_boneWeightsOffset(0)
		, m_vertices(nullptr)
	{}

	// ����ƫ����,�����ʽ
	void VertexData::build()
	{
		m_posOffset = 0;
		m_normalOffset = m_posOffset + sizeof(Vector3);
		m_colorOffset = m_normalOffset + (m_isUseNormal ? sizeof(Vector3) : 0);
		m_uv0Offset = m_colorOffset + (m_isUseVertexColor ? sizeof(Dword) : 0);
		m_uv1Offset = m_uv0Offset + (m_isUseDiffuseUV ? sizeof(Vector2) : 0);
		m_boneIndicesOffset = m_uv1Offset + (m_isUseLightmapUV ? sizeof(Vector2) : 0);
		m_boneWeightsOffset = m_boneIndicesOffset + (m_isUseBoneData ? sizeof(Dword) : 0);
		m_tangentOffset = m_boneWeightsOffset + (m_isUseBoneData ? sizeof(Vector3) : 0);
		m_stride = m_tangentOffset + (m_isUseTangentBinormal ? sizeof(Vector3) * 2 : 0);

		m_vertexElements.clear();

		// ����λ�õĿռ�
		m_vertexElements.push_back(RenderInput::VertexElement(RenderInput::VS_POSITION, PF_RGB32_FLOAT));

		// ʹ�÷���
		if (m_isUseNormal)
			m_vertexElements.push_back(RenderInput::VertexElement(RenderInput::VS_NORMAL, PF_RGB32_FLOAT));

		// ���Ӷ���ɫ
		if (m_isUseVertexColor)
			m_vertexElements.push_back(RenderInput::VertexElement(RenderInput::VS_COLOR, PF_RGBA8_UNORM));

		// ��������
		if (m_isUseDiffuseUV)
			m_vertexElements.push_back(RenderInput::VertexElement(RenderInput::VS_TEXCOORD0, PF_RG32_FLOAT));

		// �ƹ�ͼ
		if (m_isUseLightmapUV)
			m_vertexElements.push_back(RenderInput::VertexElement(RenderInput::VS_TEXCOORD1, PF_RG32_FLOAT));

		// ��������(����Ȩ��������)
		if (m_isUseBoneData)
		{
			m_vertexElements.push_back(RenderInput::VertexElement(RenderInput::VS_BLENDINDICES, PF_RGBA8_UINT));
			m_vertexElements.push_back(RenderInput::VertexElement(RenderInput::VS_BLENDWEIGHTS, PF_RGB32_FLOAT));
		}

		// �пռ�����
		if (m_isUseTangentBinormal)
		{
			m_vertexElements.push_back(RenderInput::VertexElement(RenderInput::VS_TANGENT, PF_RGB32_FLOAT));
			m_vertexElements.push_back(RenderInput::VertexElement(RenderInput::VS_BINORMAL, PF_RGB32_FLOAT));
		}
	}

	// ��ȡ�����ʽ��С
	ui32 VertexData::getVertexStride() const
	{
		return m_stride;
	}

	// ��ȡ��������
	ui32 VertexData::getVertexCount() const
	{
		return m_count;
	}

	// ��ȡ��������
	Byte* VertexData::getVertices() const
	{
		return m_vertices;
	}

	// �ж϶����ʽ���Ƿ���ָ�����͵�����
	bool VertexData::isVertexUsage(RenderInput::VertexSemantic semantic) const
	{
		size_t num = m_vertexElements.size();
		for (size_t i = 0; i < num; ++i)
		{
			if (m_vertexElements[i].m_semantic == semantic)
				return true;
		}

		return false;
	}

	// ��ȡ����λ������
	Vector3& VertexData::getPosition(Word index)
	{
		EchoAssert(index < m_count && isVertexUsage(RenderInput::VS_POSITION));

		return *(Vector3*)((Byte*)m_vertices + m_stride*index + m_posOffset);
	}

	// ��ȡ���㷨������
	const Vector3& VertexData::getNormal(Word index)
	{
		EchoAssert(index < m_count && isVertexUsage(RenderInput::VS_NORMAL));

		return *(Vector3*)((Byte*)m_vertices + m_stride*index + m_normalOffset);
	}

	// ��ȡ������ɫ����
	Dword& VertexData::getColor(Word index)
	{
		EchoAssert(index < m_count && RenderInput::VS_COLOR);

		return *(Dword*)((Byte*)m_vertices + m_stride*index + m_colorOffset);
	}

	// ��ȡ����UV����0
	const Vector2& VertexData::getUV0(Word index)
	{
		EchoAssert(index < m_count && isVertexUsage(RenderInput::VS_TEXCOORD0));

		return *(Vector2*)((Byte*)m_vertices + m_stride*index + m_uv0Offset);
	}

	// ��ȡ����UV����1
	const Vector2& VertexData::getUV1(Word index)
	{
		EchoAssert(index < m_count && isVertexUsage(RenderInput::VS_TEXCOORD1));

		return *(Vector2*)((Byte*)m_vertices + m_stride*index + m_uv1Offset);
	}

	// ��ȡ����
	Vector3& VertexData::getTangent(Word index)
	{
		EchoAssert(index < m_count && isVertexUsage(RenderInput::VS_TANGENT));

		return *(Vector3*)((Byte*)m_vertices + m_stride*index + m_tangentOffset);
	}

	void VertexData::reset()
	{
		m_count = 0;
		m_isUseNormal = false;
		m_isUseVertexColor = false;
		m_isUseDiffuseUV = false;
		m_isUseLightmapUV = false;
		m_isUseBoneData = false;
		m_isUseTangentBinormal = false;
		m_stride = 0;
		m_posOffset = 0;
		m_normalOffset = 0;
		m_colorOffset = 0;
		m_uv0Offset = 0;
		m_uv1Offset = 0;
		m_boneIndicesOffset = 0;
		m_boneWeightsOffset = 0;
		m_vertices = NULL;
	}
}