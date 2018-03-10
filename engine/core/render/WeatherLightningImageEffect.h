#pragma once

#include "PostImageEffect.h"

namespace rapidxml
{
	template<typename Ch> class xml_node;
}

namespace Echo
{
	class WeatherLightningImageEffect :public PostImageEffect
	{
	public:
		WeatherLightningImageEffect(const String& name);
		~WeatherLightningImageEffect();

		void weatherLightningReset();
		void adjustWeatherLightningStatus();
		void playWeatherSystemSound(const Echo::String& path);

		void render();
		void setLogicCalcParameter(const String& name, const String& value);
		String getLogicCalcParameter(const String& name);
	private:
		bool m_bWeatherLightingNeedReset;	//�Ƿ���Ҫ����ȫ���������
		bool m_bWeatherSingleLightiningEnable;	//���������Ƿ���
		bool m_bWeatherLIghtiningSoundEnable;	//�Ƿ���Ҫ����������Ч
		float m_weatherLightingTotalTime;	//���������ܳ���ʱ��
		float m_weatherLightingElapsedTime;	//Ŀǰ����ʱ��
		Vector4 m_weatherLightingDurationTimes;	//����������ֶεĳ���ʱ��
		float m_weatherLightingStartTime;	//����ϵͳ��ʼʱ��
		float m_weatherLightningIntervalTime;	//ȫ��������
		Echo::String m_lightningSoundPath;	//������Ч��·��

	};
	
}  // namespace Echo
