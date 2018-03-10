#ifndef __ECHO_MESH_H__
#define __ECHO_MESH_H__

#include "engine/core/render/render/RenderInput.h"
#include "Engine/core/Resource/Resource.h"
#include "Engine/core/Geom/Box.h"
#include "engine/core/Util/Array.hpp"
//#include "scl/bitset.h"

namespace Echo
{
	/**
	 * ģ��
	 */
	class SubMesh;
	class DataStream;
	class Mesh: public Resource
	{
		friend class MeshManager;
		friend class Model;
	public:
		Mesh(const String& name);
		virtual ~Mesh();

		// ��ȡ��Χ��
		const Box& getBox() const;

		// �ؼ����Χ��
		void recalcBox();

		void reBuildVertexBuffer();

		// �Ƿ��Ƕ���ģ��
		bool isSkinned() const { return m_isSkinned; }

		// some function to re build
		virtual void rebuildVertexFomat_Insert(const RenderInput::VertexElement& element, void* templateData);
		virtual void rebuildVertexFomat_Remove(RenderInput::VertexSemantic semantic);

		// ��ȡ�汾��
		bool getUseSubMeshBoneMatrices() const { return m_useSubMeshBoneMatrices; }

		// ��ȡ��������
		int getVertexCount();

		// ��ȡ��ģ������
		int getSubMeshNum() const { return static_cast<int>( m_subMeshs.size()); }

		// ��ȡ��ģ��
		SubMesh* getSubMesh( int idx);

		// �Ż���ģ��
		void redundancySubMeshs();

		// �����Ƿ�����������ͼCPU��������
		void setRetainPreparedData(bool setting){ m_isRetainPreparedData = setting;}

		// ����
		virtual bool save( const char* filePath);
		virtual bool saveToFile( const char* filePath );

		void generateTangentData( bool useNormalMap );

		bool isValid();

		// ����
		virtual void update( ui32 delta, const Echo::Vector3& vpos, const Echo::Quaternion& qrotate){};

		// �Ƿ���Ҫ����
		virtual bool isNeedUpdate(){ return false; }

		virtual bool			needCloneNew(){ return false; }
		virtual Mesh*			cloneNew(){ return NULL; }

		virtual const String&	getCloneName(){ return m_clone_name; }

		const array<bool, 256>&	getEffectiveBones();

	protected:
		virtual size_t	calculateSize() const;
		virtual bool prepareImpl( DataStream* stream);
		virtual bool prepareImplInteral(DataStream* stream, bool isSkinned);
		virtual void unprepareImpl();
		virtual bool loadImpl();
		virtual void unloadImpl();

		// �ļ�����
		virtual bool parseFromFileVersion_old(DataStream* stream, bool useHalfFloat);
		void disableDefaultTexLoad();

	protected:
		bool					m_useSubMeshBoneMatrices;
		bool					m_bNeedBackUp;
		Box						m_box;
		vector<SubMesh*>::type	m_subMeshs;				// ��ģ���б�
		bool					m_isSkinned;			// �Ƿ�Ϊ������ģ��
		bool					m_isRetainPreparedData; // �Ƿ�����������ͼCPU��������
		bool					m_isValid;
		Echo::String			m_clone_name;			//��ǰ��clone������
		bool					m_isDisableDefaultTex;
		array<bool, 256>		m_effectiveBones;
	};

	/**
	 * ʵ�������� 
	 * ��ӡ��
	 */
	class InstanceMesh : public Mesh
	{
	public:
		InstanceMesh(const String& name) :Mesh(name){}
		~InstanceMesh() {}

		// ��ȡģ��ģ������
		Echo::String GetTemplateMeshName() const { return m_templateName; }

		// ��ȡʵ������
		int	GetInstanceCount() const { return m_instanceCount; }

		// ����������ȡʵ��λ��
		Echo::Vector3 GetInstancePos( size_t id) const { EchoAssert(id>=0&&id<m_instancePos.size()); return m_instancePos[id]; }

		// ����������ȡʵ����ת
		Echo::Quaternion GetInstanceRot( size_t id) const { EchoAssert(id>=0&&id<m_instanceRot.size()); return m_instanceRot[id]; }

		// ����������ȡʵ������
		Echo::Vector3 GetInstanceScale( size_t id) const { EchoAssert(id>=0&&id<m_instanceScale.size()); return m_instanceScale[id]; }

	protected:
		// �ڲ�����ʵ��
		bool prepareImplInteral(DataStream* stream, bool isSkinned);
		virtual void unloadImpl();

		// �ļ�����
		bool parseFromFileOneSumesh(DataStream* stream, bool useHalfFloat);
	private:
		// ʵ������������
		void InstanceVertices(SubMesh* subMesh, const vector<Echo::Vector3>::type& instancePos, const vector<Echo::Quaternion>::type& instanceRot, const vector<Echo::Vector3>::type& instanceScale);

		// ʵ������������
		void InstanceIndices(SubMesh* subMesh,size_t instanceVertexCount,size_t instanceCount);

		// ʵ������ͬsubmesh�Ķ�������
		void InstanceSubMeshVertices(const vector<Echo::Vector3>::type& instancePos, const vector<Echo::Quaternion>::type& instanceRot, const vector<Echo::Vector3>::type& instanceScale);

		// ʵ������ͬsubmesh����������
		void InstanceSubMeshIndices(size_t firstSubMeshVertexCount, size_t instanceCount);

	protected:
		Echo::String					m_templateName;			// ģ����������֡�
		int								m_instanceCount;		// ʵ��������
		int								m_subMeshCount;
		bool							m_isSameModel;
		vector<Echo::Vector3>::type		m_instancePos;
		vector<Echo::Quaternion>::type	m_instanceRot;
		vector<Echo::Vector3>::type		m_instanceScale;
	};
}

#endif