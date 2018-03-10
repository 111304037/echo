#pragma once

#include "Engine/Core.h"
#include "engine/core/render/render//RenderInput.h"

namespace Echo
{
	/** 
	 * SubMesh 2013-11-6
	 */
	class Mesh;
	class SubMesh
	{
		friend class Mesh;
		friend class DynamicMesh;
		friend class InstanceMesh;
		typedef RenderInput::VertexElementList RIVEL;

	public:
		// ������Ϣ
		struct VertexInfo
		{
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
			vector<Vector3>::type	m_positions;// λ������(����ʹ��,����ɾ��,��̬ģ��only)

			// ���캯��
			VertexInfo();

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

			// ��¼λ������
			void copyPositions();

			//״̬�ÿ�
			void reset();
		};

		// �пռ�����
		struct Tangent
		{
			Vector3		m_tangent;		// ����
			Vector3		m_binormal;		// ����

			Tangent()
				: m_tangent( Vector3::ZERO)
				, m_binormal( Vector3::ZERO)
			{}
		};

		struct SinglelVertexBuff
		{
			GPUBuffer* m_gpubuff;
			RenderInput::VertexElementList m_vertex_element_list;
			SinglelVertexBuff()
				:m_gpubuff(NULL)
			{

			}
		};

		typedef Echo::vector<SinglelVertexBuff>::type MutiVertexBuff;
		typedef Echo::vector<SinglelVertexBuff>::const_iterator itrMutiVertexBuff;

	public:
		// ��ȡ����
		const String& getName() const { return m_name; }

		// ��ȡ����������
		TextureRes* getDiffuseTexture() const;

		// get mask texture
		TextureRes* getMasktexture() const;

		// ��ȡ������Ϣ
		VertexInfo& getVertexInfo() { return m_vertInfo; }

		// ��ȡ�����ʽ��С
		ui32 getVertexStride() const { return m_vertInfo.m_stride; }

		// ��ȡ��������
		ui32 getVertexCount() const { return m_vertInfo.m_count; }

		// ��ȡ��������
		Byte* getVertices() const { return m_vertInfo.m_vertices; }

		// ��ȡ���㻺��
		GPUBuffer* getVertexBuffer() const;

		// ��ȡ��������
		GPUBuffer* getIndexBuffer() const;

		// ��ȡ������
		ui32 getFaceCount() const;

		// ��ȡ��������
		ui32 getIndexCount() const;

		// ��ȡ������ʽ��С
		ui32 getIndexStride() const;

		// ��ȡ��������
		Word* getIndices() const;

		// �ж϶����ʽ���Ƿ���ָ�����͵�����
		bool isVertexUsage(RenderInput::VertexSemantic semantic) const { return m_vertInfo.isVertexUsage(semantic);}

		// ��ȡ�����ʽ
		const RenderInput::VertexElementList& getVertexElements() const;

		// ��ȡ���ذ�Χ��
		const Box& getLocalBox() const;

		// ��ȡ��Ӱ��Ĺ�������
		ui32 getBoneNum() const { return static_cast<ui32>(m_boneIdxs.size()); }

		// ��ȡ��Ӱ��Ĺ�������
		ui32 getBoneIdx( int idx) { return m_boneIdxs[idx]; }

		// ��ȡ����ͼ��С
		int getLightmapResolution() const { return m_lightmapResolution; }

		// ���ù���ͼ��С
		void setLightmapResolution( int resolution) { m_lightmapResolution = resolution; }

		// �Ƿ�ʹ�ð뾫�ȸ�����
		bool getUseHalfFloat(){ return m_bHalfFloat; }

		void generateTangentData( bool useNormalMap );

		void setVisible(bool state) { m_visible = state; }
		bool* isVisible() { return &m_visible; }

		bool isMultiVertexBuff(){ return m_is_muti_vertex_buff; }

		const MutiVertexBuff& getMultiVertexBuff(){ return m_muti_vertex_buff; }

		// ����
		virtual void update(ui32 delta){};

		void removeRedundancyVertex();

	protected:
		SubMesh();
		virtual ~SubMesh();

		// ����
		virtual bool load();

		// ж��
		virtual void unloadImpl();

		// ...
		virtual void unprepareImpl();

		// ��������
		virtual void dataInsert(const RenderInput::VertexElement& element, void* templateData);

		// �Ƴ�����
		virtual void dataRemove(RenderInput::VertexSemantic semantic);

		// ����ռ���ڴ��С
		virtual size_t calculateSize() const;

		// ���ö�����Ϣ floatPos: ����0������float������С��0��float�μ���ֽ���
		virtual void setVertexStructInfo(bool isSkinned, bool useHalfFloat, std::vector<int>& floatPos);

		// ������������
		virtual void buildTangentData();

		virtual void reBuildVertexBuffer();

	protected:
		String					m_name;					// ����
		Box						m_box;					// ��Χ��
		int						m_lightmapResolution;	// ����ͼ�ֱ���
		VertexInfo				m_vertInfo;				// ������Ϣ
		ui32					m_idxCount;				// ��������
		ui32					m_idxStride;			// ������ʽ��С
		Byte*					m_indices;				// ��������
		GPUBuffer*				m_vertexBuffer;			// ���㻺��
		GPUBuffer*				m_indexBuffer;			// ��������

		TextureRes*				m_pDiffTex;				// ����������
		TextureRes*				m_pMaskTex;				// mask
		bool					m_bHalfFloat;
		vector<ui32>::type		m_boneIdxs;				// ��������

		bool					m_visible;				// �Ƿ���ʾ
		bool					m_is_muti_vertex_buff;		// �Ƿ��Ƕ���
		MutiVertexBuff			m_muti_vertex_buff;		// ��������buff
	};
}

