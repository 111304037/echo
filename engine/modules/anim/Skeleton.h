#pragma once

#include "engine/core/Resource/Resource.h"
#include "engine/core/math/MathLib.h"

namespace Echo
{
	class Bone;
	class Skeleton : public Resource
	{
	public:
		friend class SkeletonManager;
		typedef vector<Bone*>::type			BoneList;
		typedef map<String, Bone*>::type	BoneListByName;
	public:
		Skeleton(const String& name, bool isManual);
		virtual ~Skeleton();

		// �½�����
		virtual Bone* createBone(int parentId, const String& name, const Quaternion& rot, const Vector4& pos, bool bHelper = false);

		// ���ݹ���ID��ȡ����
		virtual Bone* getBoneById(int boneId) const;

		// ���ݹ������ƻ�ȡ����
		virtual Bone* getBone(const String& boneName);

		// ��ȡ��������
		virtual ui32 getBoneCount() const;

		// ��ȡ�ҵ�����
		virtual ui32 getDummyCount() const;

		// ��ȡ��������(��ʵ�������+�ҵ�)
		virtual ui32 getTotalCount() const;

		// ��ȡ�����б�
		virtual const BoneList&	getBones() const;

		// ��ȡ������
		Bone* getRootBone();

		// ��ȡ�����ӽڵ�(��������)
		bool getAllChildren(BoneList& oBoneList, Bone* parentBone);

		bool supportScale() const;

	protected:
		virtual size_t calculateSize() const;
		virtual bool prepareImpl(DataStream* stream);
		virtual void unprepareImpl();
		virtual bool loadImpl();
		virtual void unloadImpl();

	protected:
		BoneList		m_bones;			// ����������
		BoneListByName	m_bonesByName;		// ��������
		ui32			m_boneCount;		// �������� without dummy
		ui32			m_dummyCount;		// �ҵ�����
		Bone*			m_pRootBone;		// ������
		bool			m_supportScale;
	};

	/**
	* SkeletonPose
	*/
	struct SkeletonPose
	{
		vector<Quaternion>::type		m_rotations;		// ��ת
		vector<Vector4>::type			m_translations;		// λ��a
		map<int, Quaternion>::type		m_lookAtRotations;	// ����̶�����ת

		// ���캯��
		SkeletonPose(Skeleton* skeleton);

		// ����
		void reset(Skeleton* skeleton);
	};
}