#pragma once
#include "AnimSolver.h"

namespace Echo
{
	class Bone;
	class Skeleton;
	struct SkeletonPose;


	class SkeletonControllerLookAt : public ISkeletonController
	{
	public:
		SkeletonControllerLookAt();
		~SkeletonControllerLookAt();

		// �������ù�����
		void attachBone(Skeleton* skeleton, const char* boneName);

		// ������תֵ
		void setRotation(const Quaternion& rotation);

		// ����
		virtual void frameMove(i32 deltaTime);

		// ִ��
		virtual void solver(SkeletonPose& skeletonPose);

		// ��ȡ���ù�����
		virtual const Bone* getBone() { return m_bone; }

	protected:
		Skeleton*		m_skeleton;			// ��Ӧ�Ǽ�
		Bone*			m_bone;				// ��������������
		Quaternion		m_rotation;			// ������ת
	};
}