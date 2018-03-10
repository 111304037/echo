#ifndef __ECHO_ANIMBLENDER_H__
#define __ECHO_ANIMBLENDER_H__

#include "engine/core/Resource/EchoThread.h"
#include "engine/core/Util/PtrMonitor.h"
#include "engine/core/Util/Array.hpp"
//#include "scl/bitset.h"

namespace Echo
{
	class Skeleton;
	class AnimState;
	class BlenderEvent;
	class ISkeletonController;
	struct SkeletonPose;
	class AnimBlender
	{
		friend class BlenderEvent;
		friend class AnimManager;
		typedef vector<ISkeletonController*>::type SkeletonControllerList;
	public:
		// ���ģʽ
		enum BlendMode
		{
			BM_REPLACE,		// stop src anim and start dest anim
			BM_CROSSFADE,	// blend src animation out while blending dest animation in
		};

	public:	
		// ��ȡΨһ��ʶ��
		ui32 getIdentifier() const { return m_identifier; }

		// ���Ŷ���
		virtual bool blend(AnimState* pAnimState, ui32 blendTime);

		virtual void					setBlendMode(BlendMode mode);

		virtual Skeleton*				getSkeleton() const;
		virtual bool					isBlending() const;
		virtual AnimState*				getAnimState() const;
		virtual const Matrix4&			getAnimMatrix(ui32 idx) const;
		const Matrix4*					getAnimMatrixPtr() const;
		virtual const Quaternion&		getAnimOrientation(ui32 idx) const;
		const Quaternion*				getAnimOrientationPtr() const;
		virtual const Vector3&			getAnimPosition(ui32 idx) const;
		const Vector3*					getAnimPositionPtr() const;
		float							getAnimScale(ui32 idx) const;
		virtual void					setBlendAnimState(AnimState* state){ /*m_pBlendAnimState = state;*/ };

		// ÿ֡����
		virtual void					frameMove(i32 deltaTime, bool isSync=false);
		virtual void					setIsLoopSkill(bool isLoopSkill);

		// ��Ӷ���������
		void addSkelController(ISkeletonController* skelController);

		// ɾ������������
		void removeSkelController(ISkeletonController* skelController);

		// �h�����ж���������
		void removeSkelController();

		// ֹͣ���в��ֶ���
		void stopAllPartitionAinm();

	public:
		// ����
		void frameMoveInteral(i32 deltaTime);

		// �ں϶���
		void crossFade(SkeletonPose& oSkeletonPose, AnimState* firstAnimState, AnimState* secondAnimState, float dstWeight, i32 deltaTime) const;

		// ����Ǽ�λ��
		bool calcSkeletonPose(SkeletonPose& oSkeletonPos, AnimState* animState, i32 deltaTime);

		// ���㲿�ֶ����ԹǼ�Ӱ��
		bool calcSkeletonPoseByPartitionAnimState(SkeletonPose& skeletonPose, i32 deltaTime);

		// ����Ǽܿ������Ǽ�Ӱ��
		bool calcSkeletonControllers(SkeletonPose& skeletonPose, i32 deltaTime);

		void calcObjectSpacePose(SkeletonPose& skeletonPose);

		void setEffectiveBones(const array<bool,256>& bones);

	private:
		AnimBlender(Skeleton* pSkel);
		virtual ~AnimBlender();

		bool boneAnimOptimized(size_t i);

	protected:
		ui32						m_identifier;			// Ψһ��ʶ��
		Skeleton*					m_skeleton;
		BlendMode					m_mode;
		AnimState*					m_animState;
		AnimState*					m_targetAnimState;
		map<int, AnimState*>::type	m_partitionAnimStates;	// ���ֶ���
		SkeletonControllerList      m_skelControllers;		// �Ǽܿ����� 
		ui32						m_blendTime;
		int							m_leftBlendTime;
		vector<Matrix4>::type		m_AnimMatrix;
		vector<Quaternion>::type	m_KeyOrientation;
		vector<Vector3>::type		m_KeyPosition;
		vector<float>::type			m_KeyScale;
		bool						mbNeedMerge;
		bool						mbIsLoopSkill;
		bool						m_enableAnimOptimize;
		array<bool,256>				m_effectiveBones;
		array<bool,256>				m_skipOptimizeBones;
	};
}

#endif