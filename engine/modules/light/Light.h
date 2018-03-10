#pragma once

#include <engine/core/Math/EchoMath.h>

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
			, m_positionAndInvRadius(0.f, 0.f, 0.f, 0.2f)
			, m_colorAddFalloff(1.f, 1.f, 1.f, 1.f)
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

	/**
	 * PBR��ϵƹ�
	 */
	struct PBRLight : public Light
	{
		PBRLight()
			: Light(LT_PBRLight)
			, m_selfType(LT_Point)
			, m_position(Vector4::ZERO)
			, m_direction(Vector3::ZERO)
			, m_color(Vector3::ONE)
			, m_lightParam(-1.f, 0.f, 1.f)
			, m_spot(Vector3(-1.f, 1.f, 0.f))
			, m_followModel(true)
			, m_castShadow(false)
#ifdef ECHO_EDITOR_MODE
			, m_logicPos(Vector4::ZERO)
#endif
		{ }

		LightType	m_selfType;
		Vector4		m_position;			// λ��
		Vector3		m_direction;		// ����
		Vector3		m_color;			// ��ɫ
		Vector3		m_lightParam;		// ˥��
		Vector3		m_spot;				// �۹�Ʋ���
		bool		m_followModel;
		bool		m_castShadow;

#ifdef ECHO_EDITOR_MODE
		Vector4		m_logicPos;			// �༭���й�Դ�߼�λ��
#endif
	};
}