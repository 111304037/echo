#include "FSAudioSource.h"
#include "FSAudioManager.h"
#include "engine/core/log/Log.h"

namespace Echo
{
	// ���캯��
	AudioSource::AudioSource( const Cinfo& cinfo)
		: m_info( cinfo)
		, m_eventInstance( NULL)
	{
		// ��1��ʼ����
		static int identifier=1;
		m_identifier = identifier++;

		// ��ȡ����
		FMOD::Studio::EventDescription* eventDecription = NULL;
		FMOD::Studio::System* studioSystem = FSAudioManager::instance()->getStudioSystem();
		if (FMOD_OK != studioSystem->getEvent(cinfo.m_name.c_str(), &eventDecription))
		{
			EchoLogError("FMOD Studio Error The requested event, bus or vca could not be found., eventName: [%s]", cinfo.m_name.c_str());
		}
		else
		{
			// �����¼�����
			FSAudioManager::instance()->cacheEvent(eventDecription);

			// ����ʵ��
			FS_ERRORCHECK(eventDecription->createInstance(&m_eventInstance), nullptr);

			AudioExtraInfo* info = EchoNew( AudioExtraInfo);
			info->lastTime = 0;
			info->firstTime = true;
			m_eventInstance->setUserData(info);

			// ��ȡ�¼���3d����
			FS_ERRORCHECK(eventDecription->is3D(&m_is3D), nullptr);

			// 3D����
			if (m_is3D)
			{
				Audio3DAttributes attributes = get3dAttributes();
				attributes.m_position = cinfo.m_pos;
				set3dAttributes(attributes);
			}

			// ��������
			setVolume(m_info.m_volume);

			// ���ó�ʼ����
			for (const ParamValue& param : cinfo.m_initParamValues)
			{
				setParameterValue(param.m_paramName.c_str(), param.m_value);
			}

			// ��Ϣ�ص�
			if (cinfo.m_isUseCB)
			{
				AudioSourceEventCB cb = FSAudioManager::instance()->getAudioEventCb();
				if (cb)
				{
					(*cb)(AS_CreateFinished, this);
				}		
			}

			// ����
			FS_ERRORCHECK(m_eventInstance->start(), nullptr);
		}
	}
	
	// ��������
	AudioSource::~AudioSource()
	{
		AudioExtraInfo* info = NULL;
		m_eventInstance->getUserData(reinterpret_cast<void**>(&info));
		EchoSafeDelete( info, AudioExtraInfo);
		FS_ERRORCHECK( m_eventInstance->release(), nullptr);
	}

	// ����������С
	void AudioSource::setVolume( float volume)
	{
		m_info.m_volume    = Math::Clamp( volume, 0.f, 10.f);
		float volumeFactor = FSAudioManager::instance()->getGroupVolumeFactor(m_info.m_group);
		FS_ERRORCHECK( m_eventInstance->setVolume( m_info.m_volume * volumeFactor), nullptr);
	}

	// ��ͣ
	void AudioSource::pause()
	{
		FS_ERRORCHECK(m_eventInstance->setPaused(true), nullptr);
	}

	// ��������
	void AudioSource::resume()
	{
		FS_ERRORCHECK(m_eventInstance->setPaused(false), nullptr);
	}

	// ֹͣ
	void AudioSource::stop()
	{
		FS_ERRORCHECK( m_eventInstance->stop( FMOD_STUDIO_STOP_IMMEDIATE), nullptr);
	}

	// ����3D����			
	void AudioSource::set3dAttributes( const Audio3DAttributes& attributes)
	{
		if( m_is3D)
		{
			m_3dAttributes = attributes;

			EchoAssert( sizeof(Audio3DAttributes)==sizeof(FMOD_3D_ATTRIBUTES));
			FS_ERRORCHECK( m_eventInstance->set3DAttributes( (FMOD_3D_ATTRIBUTES*)&m_3dAttributes), nullptr);
		}
	}

	// ��ȡ3D����
	const Audio3DAttributes& AudioSource::get3dAttributes()
	{
		if ( m_is3D)
		{
			EchoAssert(sizeof(Audio3DAttributes) == sizeof(FMOD_3D_ATTRIBUTES));
			FS_ERRORCHECK(m_eventInstance->get3DAttributes((FMOD_3D_ATTRIBUTES*)&m_3dAttributes), nullptr);
		}
	
		return m_3dAttributes;
	}

	// �Ƿ���ֹͣ
	bool AudioSource::isStopped()
	{
		if( !m_eventInstance)
			return true;

		FMOD_STUDIO_PLAYBACK_STATE state;
		FS_ERRORCHECK( m_eventInstance->getPlaybackState( &state), nullptr);
		if( state == FMOD_STUDIO_PLAYBACK_STOPPED)
			return true;

		return false;
	}

	// ��ȡ����ֵ
	void AudioSource::getParameterValue( const char* name, float& oValue)
	{
		FMOD::Studio::ParameterInstance* parameter = NULL;
		FS_ERRORCHECK( m_eventInstance->getParameter( name, &parameter), nullptr);
		FS_ERRORCHECK( parameter->getValue( &oValue), nullptr);
	}

	// ��ȡ������Χ
	void AudioSource::getParameterRange( const char* name, float& oRangeMin, float& oRangeMax)
	{
		FMOD::Studio::ParameterInstance* parameter = NULL;
		FS_ERRORCHECK( m_eventInstance->getParameter( name, &parameter), nullptr);

		FMOD_STUDIO_PARAMETER_DESCRIPTION* description = NULL;
		FS_ERRORCHECK( parameter->getDescription( description), nullptr);
		if( description)
		{
			oRangeMin = description->minimum;
			oRangeMax = description->maximum;
		}
	}

	// ���ò���ֵ
	void AudioSource::setParameterValue( const char* name, float value)
	{
		FMOD::Studio::ParameterInstance* parameter = nullptr; 
		m_eventInstance->getParameter(name, &parameter); 
		if (parameter == nullptr)
		{
			// debug resource.
			int count = 0; 
			m_eventInstance->getParameterCount(&count); 
			EchoLogError("[CHECK][AudioSource::setParameterValue]::Parameter Count == [%d]", count);
			for (int i = 0; i < count; i++)
			{
				m_eventInstance->getParameterByIndex(i, &parameter); 

				FMOD_STUDIO_PARAMETER_DESCRIPTION desc = {};
				float value = 0.f; 

				parameter->getDescription(&desc); 
				parameter->getValue(&value); 
				EchoLogError("[CHECK][AudioSource::setParameterValue]::Parameter[%d]Key == [%s], Value == [%.3f]", i, desc.name, value); 
			}
		}

		FS_ERRORCHECK( m_eventInstance->getParameter( name, &parameter), "Parameter[%s] is't exist in Event[%s]", name, m_info.m_name.c_str());
		FS_ERRORCHECK( parameter->setValue( value), nullptr);
	}

	// ��������ֵ
	void AudioSource::setProperty(SoundProperty sp_type, float value)
	{
		FMOD_STUDIO_EVENT_PROPERTY fmod_property = FMOD_STUDIO_EVENT_PROPERTY_FORCEINT;
		switch (sp_type)
		{
		case Echo::AudioSource::SP_MINI_DISTANCE:
			fmod_property = FMOD_STUDIO_EVENT_PROPERTY_MINIMUM_DISTANCE;
			break;
		case Echo::AudioSource::SP_MAX_DISTANCE:
			fmod_property = FMOD_STUDIO_EVENT_PROPERTY_MAXIMUM_DISTANCE;
			break;
		default:
			return;
		}

		if (m_eventInstance)
		{
			m_eventInstance->setProperty(fmod_property, value);
		}
	}

	// �������ֵ
	void AudioSource::getProperty(SoundProperty sp_type, float& oValue)
	{
		FMOD_STUDIO_EVENT_PROPERTY fmod_property = FMOD_STUDIO_EVENT_PROPERTY_FORCEINT;
		switch (sp_type)
		{
		case Echo::AudioSource::SP_MINI_DISTANCE:
			fmod_property = FMOD_STUDIO_EVENT_PROPERTY_MINIMUM_DISTANCE;
			break;
		case Echo::AudioSource::SP_MAX_DISTANCE:
			fmod_property = FMOD_STUDIO_EVENT_PROPERTY_MAXIMUM_DISTANCE;
			break;
		default:
			return;
		}

		if (m_eventInstance)
		{
			m_eventInstance->getProperty(fmod_property, &oValue);
		}
	}

	// ��ó���
	int AudioSource::getLenth()
	{
		FMOD::Studio::EventDescription* description = NULL;
		m_eventInstance->getDescription(&description);
		if (description)
		{
			int lenth = 0;
			description->getLength(&lenth);
			return lenth;
		}
		return 0;
	}
}