#pragma once

#include "FSAudioBase.h"

namespace Echo
{
	class AudioSource;

	// �¼��ص�����
	enum AudioSouceEventCBType
	{
		AS_CreateFinished,			// �������
		AS_Unknown,					// δ֪
	};
	typedef void(*AudioSourceEventCB)(AudioSouceEventCBType type, AudioSource* audioSource);

	// ��Ƶ3D����
	struct Audio3DAttributes
	{
		Vector3		m_position;		// λ��
		Vector3		m_velocity;		// λ���ٶ�
		Vector3		m_forward;		// ����(����Ϊ��λ����,���봹ֱ���Ϸ���)
		Vector3		m_up;			// �Ϸ���

		Audio3DAttributes()
			: m_position( Vector3::ZERO)
			, m_velocity( Vector3::ZERO)
			, m_forward( Vector3::UNIT_Z)
			, m_up( Vector3::UNIT_Y)
		{}
	};

	/**
	 * ��Դ
	 */
	class AudioSource
	{
		friend class FSAudioManager;
	public:
		// ��������
		enum LoadType
		{
			LT_Normal,		// ��������
			LT_NoBlocking,	// ��ʽ����,�߼��ر߲���
		};

		// ��������
		enum SoundProperty
		{
			SP_MINI_DISTANCE = 1,
			SP_MAX_DISTANCE,
			SP_FINAL_PROPERTY,  //��β���ԣ����κ�����
		};
		
		// ��������
		struct ParamValue
		{
			String	m_paramName;		// ������
			float	m_value;			// ����ֵ
		};
		typedef vector<ParamValue>::type ParamValues;

		// ������Ϣ
		struct Cinfo
		{
			String		m_name;				// �¼�����
			float		m_volume;			// ��ʼ������С
			LoadType	m_loadType;			// ��Դ���ط�ʽ(����������)
			bool		m_is3DMode;			// �Ƿ�ǿ����2Dģʽ����
			float		m_minDistane;		// ������С˥����ʼ����
			float		m_maxDistance;		// ������С˥����������
			Vector3		m_pos;				// λ��
			ui32		m_group;			// �㼶(������������)
			bool		m_isOneShoot;		// �Ƿ񵥴β���(�����one shoot)
			ParamValues m_initParamValues;	// ������ʼ����ֵ
			bool		m_isUseCB;			// �Ƿ�ִ��ע��ʹ�����ص�����
			bool		m_pausable;			// �Ƿ����ͣ
			Cinfo() 
				: m_volume( 1.f)
				, m_is3DMode( true)
				, m_pos( Vector3::ZERO)
				, m_minDistane( 0.f)
				, m_maxDistance( 20.f)
				, m_loadType( LT_Normal)
				, m_group( 0)
				, m_isOneShoot( true)
				, m_isUseCB(false)
				, m_pausable(false)
			{}
		};

	public:
		// ��ȡΨһ��ʶ��
		ui32 getIdentifier() const { return m_identifier; }

		// ��ͣ
		void pause();

		// ��������
		void resume();

		// ֹͣ
		void stop();

		// �Ƿ���3d��Ч
		bool is3D() const { return m_is3D; }

		// �Ƿ���2Dģʽ����
		bool is3DMode() const { return m_info.m_is3DMode; }

		// �Ƿ���ֹͣ
		bool isStopped();

		bool isPausable() const { return m_info.m_pausable; }

		// ����������С
		void setVolume( float volume);

		// ����3D����			
		void set3dAttributes( const Audio3DAttributes& attributes);

		// ��ȡ3D����
		const Audio3DAttributes& get3dAttributes();

		// ��ȡ��ʼ������Ϣ
		const Cinfo& getCinfo() const { return m_info; }

		// ��ȡ����ֵ
		void getParameterValue( const char* name, float& oValue);

		// ��ȡ������Χ
		void getParameterRange( const char* name, float& oRangeMin, float& oRangeMax);

		// ���ò���ֵ
		void setParameterValue( const char* name, float value);

		// ��������ֵ
		void setProperty(SoundProperty sp_type, float value);

		// �������ֵ
		void getProperty(SoundProperty sp_type, float& oValue);

		// ��ó���
		int getLenth();

	private:
		AudioSource( const Cinfo& cinfo);
		~AudioSource();

	private:
		AudioSourceID					m_identifier;	// Ψһ��ʶ��
		FMOD::Studio::EventInstance*	m_eventInstance;// ��Դʵ��
		Cinfo							m_info;			// ������Ϣ
		bool							m_is3D;			// �Ƿ���3D��Դ
		Audio3DAttributes				m_3dAttributes;	// 3D����
	};
}