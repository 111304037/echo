#pragma once

#include "Frame/Example.h"

namespace Examples
{
	/**
	 * ���ǹ켣(�������)
	 * ѧϰĿ��:
	 *     1.ʹ������DebugDisplay��Ⱦ����������
	 *	   2.�˽Ⲣʹ��SceneNode��������ռ��ϵ
	 */
	class Planet;
	class VenusPentagram : public Example
	{
	public:
		VenusPentagram();
		virtual ~VenusPentagram() {}

		// ������Ϣ
		virtual const LORD::String getHelp();

		// ��ʼ��
		virtual bool initialize();

		// ����
		virtual void tick(LORD::ui32 elapsedTime);

		// ����
		virtual void destroy();

	private:
		Planet*			m_sun;			// ̫��
		Planet*			m_mercury;		// ˮ��
		Planet*			m_venus;		// ����
		Planet*			m_earth;		// ����
		Planet*			m_mars;			// ����
		Planet*			m_jupiter;		// ľ��
		Planet*			m_saturn;		// ����
		Planet*			m_uranus;		// ������
		Planet*			m_neptune;		// ������
	};

	/**
	 * ���Ƕ���
	 */
	class Planet
	{
		typedef LORD::list<LORD::Vector3>::type Vector3List;
	public:
		Planet( const LORD::String& name, float radius, const LORD::Color& color);
		~Planet();

		// ���ù�ת����
		void setRevolution(Planet* parent, float revolutionRadius, float revolutionSpeed);

		// �����Ƿ���ʾ�켣
		bool setShowTrack( bool isShowTrack);

		// ��ȡ�����ڵ�
		LORD::SceneNode* getSceneNode() { return m_sceneNode;  }

		// ����
		void update( LORD::ui32 elapsedTime);

		// ������ʾ�켣
		void updateTrackDisplay();

	private:
		// �����켣��ʾ��
		void createTrack();

	private:
		LORD::String			m_name;				// ��������(Ψһ)
		float					m_radius;			// �뾶
		LORD::SceneNode*		m_sceneNode;		// �������
		LORD::Color				m_color;			// ��ɫ
		LORD::DebugDisplayItem*	m_planet;			// ��������
		bool					m_isShowTrack;		// �Ƿ���ʾ�켣
		Vector3List				m_trackPositions;	// �켣(��������ϵ)	
		LORD::DebugDisplayItem* m_track;			// �켣(��������ϵ)
		LORD::SceneNode*		m_trackNode;		// �켣���
		LORD::DebugDisplayItem*	m_links;			// ��������������
		Planet*					m_parent;			// ����
		float					m_revolutionRadius;	// ��ת�뾶
		float					m_revolutionSpeed;	// ��ת�ٶ�
		float					m_revolutionAngle;	// ��ת��ǰ�Ƕ�
	};
}

