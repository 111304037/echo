#pragma once

#include <Foundation/Math/EchoMath.h>
#include <Engine/Scene/node.h>
#include "spine/spine.h"

namespace Spine
{
	/**
	 * SpineActor
	 */
	class Actor
	{
	public:
		Actor(const char* resName);
		~Actor();

		// ����(second)
		void update(float delta, bool isForUI=false);

		// ��ȡλ��
		ECHO::node& getSceneNode() { return *m_sceneNode; }

		// ���Ŷ���
		void playAnimation( const char* animName);

		// ��������
		void setScale(float scale) { m_sceneNode->setLocalScalingXYZ(scale, scale, scale); }

		// ����λ��
		void setPosition(const ECHO::Vector3& pos) { m_sceneNode->setLocalPosition(pos); }

		// ������ת
		void setOrientation(const ECHO::Quaternion& ort) { m_sceneNode->setLocalOrientation(ort); }

	public:
		// �ύ����Ⱦ����
		void submitToRenderQueue();

	private:
		ECHO::node*		m_sceneNode;		// ���
		spAtlas*				m_spAtlas;			// ͼ��
		spSkeleton*				m_spSkeleton;		// �Ǽ�
		spAnimationState*		m_spAnimState;		// ����״̬
		spAttachmentLoader*		m_attachmendLoader;	// ������
		float*					m_worldVertices;	// ��������
		ECHO::Matrix4			m_matWVP;			// ����۲�ͶӰ����
	};
}