#pragma once

#include <engine/core/math/Math.h>
#include "engine/core/geom/AABB.h"

namespace Echo
{
	class CameraShadow
	{
	public:
		CameraShadow();
		~CameraShadow();

		// ����
		void update(const AABB* visibleActorsAABB);

		// �ֶ�����ͶӰ�ƹ⳯��
		void setLightDir(const Vector3& dir);

		// ��ȡ�۲�ͶӰ����
		const Matrix4& getViewProjMatrix() const { return m_viewProj; }

		const Matrix4& getViewMatrix() const { return m_view; }

		// ��ǰ֡�Ƿ����
		bool isEnable() { return m_enable; }

		// ��ȡActorsAABB
		AABB& getBox() { return m_Box; }

		const AABB& getCalcBox() { return m_CalcBox; }

	private:
		// ���ݰ�Χ����۲���������������
		void calcOrthoRH(Matrix4& oOrth, const AABB& box, const Matrix4& viewMat);

	private:
		bool		m_enable;			// ��ǰ�Ƿ����
		Matrix4		m_viewProj;			// �۲�ͶӰ����
		Matrix4		m_view;
		Vector3     m_dir;				// ͶӰ����
		AABB 		m_Box;              // ActorsAABB
		AABB		m_CalcBox;			// ���ڼ����AABB(����ʵ��AABB�����Ͻ�������)
	};
}