#include "engine/core/Math/Quaternion.h"
#include "Engine/modules/Anim/Bone.h"
#include "Engine/modules/Anim/Skeleton.h"
#include "LookAtIKSolver.h"

namespace Echo
{
	// ���캯��
	SkeletonControllerLookAt::SkeletonControllerLookAt()
	{

	}

	// ��������
	SkeletonControllerLookAt::~SkeletonControllerLookAt()
	{

	}

	// �������ù�����
	void SkeletonControllerLookAt::attachBone(Skeleton* skeleton, const char* boneName)
	{
		m_skeleton = skeleton;
		m_bone = skeleton->getBone(boneName);
	}

	// ������תֵ
	void SkeletonControllerLookAt::setRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	// ����
	void SkeletonControllerLookAt::frameMove(i32 deltaTime)
	{
	}

	// ִ��
	void SkeletonControllerLookAt::solver(SkeletonPose& skeletonPose)
	{
		skeletonPose.m_lookAtRotations[m_bone->getId()] = m_bone->getRotation() * m_rotation;
	}
}