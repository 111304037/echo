#ifndef __ECHO_ANIMSTATE_H__
#define __ECHO_ANIMSTATE_H__

#include "Animation.h"

namespace Echo
{
	class Animation;
	class Quaternion;
	class Vector4;
	class AnimKeyFrame;


	class AnimState
	{
	public:
		AnimState(Animation* pAnim);
		virtual ~AnimState();

		// ����
		void reset();

		virtual void enable(bool bEnable);
		virtual void enableLoop(bool bEnable);

		// ����ѭ������
		void setLoopTime(int time) { m_loopTime = time; }

		virtual void setLength(ui32 length);

		// ���ö�������λ��
		void setTimePosition(ui32 timePos);

		// ��ȡ��������λ��
		ui32 getTimePosition() const;

		virtual Animation*		getAnim() const;
		virtual bool			isEnable() const;
		virtual bool			isLoop() const;
		virtual ui32			getLength() const;
		virtual ui32			getCurrentFrameIndex() const;
		virtual ui32			getNextFrameIndex() const;
		virtual ui32			getPrevFrameIndex() const;
		virtual AnimKeyFrame*	getCurrentAnimKeyFrame() const;
		virtual AnimKeyFrame*	getAnimKeyFrame(ui32 index) const;

		// ����
		virtual void frameMove(i32 deltaTime);

		// ����ָ��������ǰλ����ת
		bool calcBonePos(Quaternion& oRotation, Vector4& oTranslate, const Bone* bone, i32 deltaTime);

		// ���ò����ٶ�
		void setSpeed(float speed) { m_speed = speed; }

		// ��ȡӰ�����Ȩ��
		virtual real32 getWeight(ui32 index) const;

		// ��ȡ���Ȩ��
		float getBlendWeight() const { return m_blendWeight; }

		// ��ȡ��������������
		int getRootBone() { return m_anim ? m_anim->getPartionRootBone() : -1; }

		// �Ƿ�Ӱ��ָ������
		bool isEffectBone(const Bone* bone);

		// �Ƿ��Ǿֲ�����
		bool isPartition() const;

		// ���ö����ں�ʱ��(����)
		void setBlendTime(const ui32 time) { m_blendTime = time; }

		// ��ǰ�����Ƿ��Ѿ������
		bool isEnd() { return m_bIsEnd; }

	protected:
		Animation*			m_anim;		// �����ƶ���
		bool				m_bEnable;
		ui32				m_loopTime;		// ѭ������
		ui32				m_length;		// ��������
		i32					m_timePos;		// ��ǰʱ��̶�
		ui32				m_nFrame;
		float				m_speed;		// ���ƶ��������ٶ�
		int					m_layer;		// ���������㼶
		ui32				m_blendTime;	// ���ʱ��
		float				m_blendWeight;	// ��ǰ���Ȩ��
		bool				m_bIsEnd;		// ���ζ����Ƿ���
	};
}

#endif