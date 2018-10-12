#pragma once

#include <engine/core/math/Math.h>

namespace Echo
{
	// ��Դ����ö��
	enum LightType
	{
		LT_Unknown = 0,		// δ֪
		LT_Ambient,			// ������
		LT_Directional,		// �����
		LT_Point,			// ���Դ
		LT_Spot,			// �۹��

		LT_PBRLight,		// PBRʹ�õĻ�ϵƹ�
	};

	/**
	 * Light
	 */
	struct Light
	{
		LightType			m_type;		// ��Դ����
		float				m_intensity;// ����ǿ��

		Light(LightType type) : m_type(type), m_intensity(1.f) {}
		virtual ~Light() {}

		// ��ȡ����
		const char* getType();
		const LightType getTypeEnum() { return m_type; }
	};

	/**
	 * ������
	 */
	struct AmbientLight : public Light
	{
		Vector4				m_color;	// ��ɫ

		// ���캯��
		AmbientLight()
			: Light(LT_Ambient)
			, m_color(0.25f, 0.25f, 0.25f, 1.f)
		{}
	};

	/**
	 * �����
	 */
	struct DirectionalLight : public Light
	{
		Vector4				m_color;		// ��ɫ
		Vector4				m_direction;	// ����
		Vector3				m_position;

		// ���캯��
		DirectionalLight()
			: Light( LT_Directional)
			, m_color(0.8f, 0.8f, 0.8f, 1.f)
			, m_direction( -0.5774f, -0.5774f, -0.5774f, 1.f)
		{
		}
	};

	/**
	 * ���Դ
	 */
	struct PointLight : public Light
	{
		Vector4		m_colorAddFalloff;		// ��ɫ��˥��ֵ��
		Vector4		m_positionAndInvRadius;	// λ����뾶����
		bool		m_followModel;			// �Ƿ����ģ��λ�ñ仯

		// ���캯��
		PointLight()
			: Light(LT_Point)
            , m_colorAddFalloff(1.f, 1.f, 1.f, 1.f)
			, m_positionAndInvRadius(0.f, 0.f, 0.f, 0.2f)
			, m_followModel( true )
		{}
	};

	/**
	 * �۹��
	 */
	struct SpotLight : public Light
	{
		// ���캯��
		SpotLight()
			: Light(LT_Spot)
		{}
	};
}
