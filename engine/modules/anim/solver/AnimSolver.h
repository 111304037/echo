#pragma once

namespace Echo
{
	class Bone;
	class Skeleton;
	struct SkeletonPose;

	class ISkeletonController
	{
	public:
		virtual ~ISkeletonController() {}

		// ����
		virtual void frameMove( i32 deltaTime)=0;

		// ִ��
		virtual void solver( SkeletonPose& skeletonPose)=0;

		// ��ȡ���ù�����
		virtual const Bone* getBone() = 0;
	};

	/**
	 * ������ת
	 */
	class SkeletonControllerBoneRotation : public ISkeletonController
	{
	public:
		SkeletonControllerBoneRotation();
		~SkeletonControllerBoneRotation();

		// �������ù�����
		void attachBone( Skeleton* skeleton, const char* boneName);

		// ������תֵ
		void setRotation( const Quaternion& rotation);

		// ����
		virtual void frameMove( i32 deltaTime);

		// ִ��
		virtual void solver( SkeletonPose& skeletonPose);

		// ��ȡ���ù�����
		virtual const Bone* getBone() { return m_bone; }

	protected:
		Skeleton*		m_skeleton;		// ��Ӧ�Ǽ�
		Bone*			m_bone;			// ��������������
		Quaternion		m_rotation;		// ��ת��
	};
}