#include "Engine/modules/Anim/Animation.h"
#include "Engine/modules/Anim/Bone.h"
#include "engine/core/Util/LogManager.h"
#include "Engine/modules/Anim/AABBKeyFrame.h"
#include "Engine/modules/Anim/AnimState.h"

namespace Echo
{
	// ���캯��
	AnimState::AnimState(Animation *pAnim)
		: m_anim(pAnim)
		, m_bEnable(false)
		, m_length(0)
		, m_timePos(0)
		, m_loopTime(1)
		, m_nFrame(0)
		, m_speed(1.f)
		, m_layer(0)
		, m_blendTime(300)
		, m_blendWeight(1.f)
		, m_bIsEnd(false)
	{
		EchoAssert(pAnim);
		m_length = pAnim->getLength();
	}

	// ��������
	AnimState::~AnimState()
	{
	}

	// ����
	void AnimState::reset()
	{
		enable(true);
		setTimePosition(0);
		enableLoop(false);
		setSpeed(1.f);
		m_blendWeight = 1.f;
	}

	void AnimState::enable(bool bEnable)
	{
		m_bEnable = bEnable;
	}

	void AnimState::enableLoop(bool bEnable)
	{
		m_loopTime = -1;
	}

	void AnimState::setLength(ui32 length)
	{
		m_length = length;
	}

	void AnimState::setTimePosition(ui32 timePos)
	{
		m_timePos = timePos;
		m_nFrame = m_anim->getFrameIndex(m_timePos, 1);
	}

	Animation* AnimState::getAnim() const
	{
		return m_anim;
	}

	bool AnimState::isEnable() const
	{
		return m_bEnable;
	}

	ui32 AnimState::getLength() const
	{
		return m_length;
	}

	bool AnimState::isLoop() const
	{
		return m_loopTime < 0 ? true : false;
	}

	ui32 AnimState::getTimePosition() const
	{
		return m_timePos;
	}

	ui32 AnimState::getCurrentFrameIndex() const
	{
		return m_nFrame;
	}

	Echo::ui32 AnimState::getNextFrameIndex() const
	{
		return std::min<ui32>(m_nFrame + 1, m_anim->getFrameCount() - 1);
	}

	Echo::ui32 AnimState::getPrevFrameIndex() const
	{
		return std::min<ui32>(m_nFrame - 1, m_anim->getFrameCount() - 1);
	}

	AnimKeyFrame* AnimState::getCurrentAnimKeyFrame() const
	{
		return m_anim->getAnimKeyFrameByIdx(m_nFrame);
	}

	AnimKeyFrame* AnimState::getAnimKeyFrame(ui32 index) const
	{
		return m_anim->getAnimKeyFrameByIdx(index);
	}

	// ÿ֡����
	void AnimState::frameMove(i32 deltaTime)
	{
		if (!m_bEnable)
		{
			m_bIsEnd = false;
			return;
		}

		// calculate current time
		m_timePos += static_cast<i32>(deltaTime * m_speed);
		if (m_timePos < 0) m_timePos = 0;

		m_bIsEnd = false;

		if (m_loopTime)
		{
			if (m_loopTime > 0 && m_timePos > static_cast<i32>(m_length))
			{
				if (m_timePos > static_cast<i32>(m_length))
				{
					m_bIsEnd = true;
				}

				m_loopTime--;
				if (!m_loopTime)
				{
					m_timePos = m_length;
					m_bEnable = false;
				}
				else
				{
					// ����ѭ��
					m_timePos = (m_timePos) % (m_length + 1);
				}
			}
			else
			{
				if (m_timePos > static_cast<i32>(m_length))
				{
					m_bIsEnd = true;
				}

				// ����ѭ��
				m_timePos = (m_timePos) % (m_length + 1);
			}
		}

		m_nFrame = m_anim->getFrameIndex(m_timePos, deltaTime);

		// ���»��Ȩ��
		if (m_blendTime)
		{
			bool isFadein = m_timePos < static_cast<i32>(m_blendTime);
			bool isFadeOut = (m_length - m_timePos) < m_blendTime;

			if (isFadein)
				m_blendWeight = static_cast<float>(m_timePos) / static_cast<float>(m_blendTime);
			else if (isFadeOut)
				m_blendWeight = static_cast<float>(m_length - m_timePos) / static_cast<float>(m_blendTime);
			else if (isFadein && isFadeOut)
				m_blendWeight = std::min<float>(static_cast<float>(m_timePos) / static_cast<float>(m_blendTime), static_cast<float>(m_length - m_timePos) / static_cast<float>(m_blendTime));
			else
				m_blendWeight = 1.f;
		}
	}

	// ���ݹ���������ȡȨ��
	real32 AnimState::getWeight(ui32 index) const
	{
		return m_anim ? m_anim->getBoneWeights()[index] : 0.f;
	}

	// �Ƿ�Ӱ��ָ������
	bool AnimState::isEffectBone(const Bone* bone)
	{
		if (m_anim)
		{
			bool needCalc = m_anim->isPartition();
			if (needCalc)
			{
				return getWeight(bone->getId()) > 0.f;
			}
			else
			{
				return true;
			}
		}

		return false;
	}

	// ����ָ��������ǰλ����ת
	bool AnimState::calcBonePos(Quaternion& oRotation, Vector4& oTranslate, const Bone* bone, i32 deltaTime)
	{
		if (m_anim->getBoneWeight(bone) > 0.f)
		{
			AnimKeyFrame* keyFramePrev = nullptr;
			AnimKeyFrame* keyFrameNext = nullptr;

			// ��ȡ����ǰһ֡����һ֡����
			if (deltaTime >= 0)
			{
				keyFramePrev = getCurrentAnimKeyFrame();
				keyFrameNext = getAnimKeyFrame(getNextFrameIndex());
			}
			else
			{
				keyFramePrev = getCurrentAnimKeyFrame();
				keyFrameNext = getAnimKeyFrame(getPrevFrameIndex());
			}

			// ��֡�������
			float ratio = 0.f;
			if (keyFramePrev != keyFrameNext)
			{
				float frameTime = static_cast<float>(m_anim->getLength()) / static_cast<float>(m_anim->getFrameCount());	// ÿ֡ʱ��
				float preFrameTime = frameTime * getCurrentFrameIndex();														// ��һ֡ʱ��
				ratio = Math::Abs<float>((getTimePosition() - preFrameTime) / frameTime);												// ��֡��ֵ

				// ��ֵ��ת
				int	 index = bone->getId();
				const Quaternion& prevQuat = keyFramePrev->getAnimQuaternion(index);
				const Quaternion& nextQuat = keyFrameNext->getAnimQuaternion(index);
				Quaternion::Slerp(oRotation, prevQuat, nextQuat, ratio, true);

				// ��ֵλ��
				Vector4 prevTran = keyFramePrev->getAnimTranslate(index);
				Vector4 nextTran = keyFrameNext->getAnimTranslate(index);

				if (!m_anim->hasScaling())
				{
					const Vector3& vBone = bone->getPosition();
					prevTran = prevTran + Vector4(prevQuat * vBone, 0.0f);
					nextTran = nextTran + Vector4(nextQuat * vBone, 0.0f);
				}

				oTranslate = prevTran * (1.f - ratio) + nextTran * ratio;
			}
			else
			{
				int	 index = bone->getId();
				oRotation = keyFramePrev->getAnimQuaternion(index);
				oTranslate = keyFramePrev->getAnimTranslate(index);

				if (!m_anim->hasScaling())
				{
					oTranslate = oTranslate + Vector4(oRotation * bone->getPosition(), 0.0f);
				}
			}

			return true;
		}

		return false;
	}

	// �Ƿ��Ǿֲ�����
	bool AnimState::isPartition() const
	{
		return m_anim ? m_anim->isPartition() : false;
	}

}