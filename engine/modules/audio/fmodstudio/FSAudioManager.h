#pragma once

#include "FSAudioSource.h"
#include "engine/core/Util/Singleton.h"

namespace Echo
{
	/**
	* ����FMOD Studioʵ�ֵ���Ƶ������
	*/
	struct BankUserData
	{
		std::string filename;
	};

	struct AudioBank;
	class FSAudioManager
	{
		__DeclareSingleton(FSAudioManager);

	public:
		typedef EchoHashMap<String, AudioBank*> BankMap;
		typedef EchoHashMap<AudioSourceID, AudioSource*> AudioSourceMap;
		typedef EchoHashSet<FMOD::Studio::EventDescription*> AudioSampleSet;
		typedef vector<AudioSourceID>::type	AudioSourceIdArray;
	public:
		FSAudioManager();
		virtual ~FSAudioManager();

		// ��ʼ��
		virtual void init(int, bool);

		// �߼���ѭ����
		virtual void tick(ui32 elapsedTime);

		// �ͷ�
		virtual void release();

		// ��������bank�ļ�
		void loadAllBankFile();

		// ��ȡ��Դ
		AudioSource* getAudioSource(AudioSourceID asID);

		// ��ȡ������Դ
		const AudioSourceMap& getAllAduioSources() { return m_audioSources; }

		// ������Դ
		AudioSourceID createAudioSource(const AudioSource::Cinfo& cinfo);

		// ������Դ
		void destroyAudioSources(AudioSourceID* audioSourceIDs, int num);

		// ����������Դ
		void destroyAllAudioSources();

		// ������������
		void setListenerAttributes(const Audio3DAttributes& attributes);

		// ��ȡ��������
		const Audio3DAttributes& getListenerAttributes() { return m_listenerAttributes; }

		// ���ݱ�ǩ��ȡ��Դ
		void listAudioSourcesByGroup(AudioSourceIdArray& oIds, int groupFilter);

		// �������Ƿ����
		void setGroupEnabled(int group, bool isEnabled) { m_isGroupEnabled[group] = isEnabled; }

		// �ж����Ƿ����
		bool isGroupEnabled(int group) const;

		// ������������������
		void setGroupVolumeFactor(int group, float volueFactor);

		// ��ȡ��������������
		float getGroupVolumeFactor(int group) const;

		// ��ȡϵͳ
		FMOD::Studio::System* getStudioSystem() { return m_studioSystem; }

		// ����bank�ļ�����ȡ�¼�
		void getAudioEvents(StringArray& oEvents, const char* bankName);

		// ��ȡ�����¼�
		void getAllAudioEvent(StringArray& oEvents);

		//����Fmod����ϵͳ
		void suspendFmodSystem();

		//�ӹ����лָ�Fmod����ϵͳ
		void resumeFmodSystem();

		//�����ٶȵ����Ƿ����
		void setSpeedAdjustEnable(bool isEnable){ m_SpeedAdjustEnable = isEnable; }

		// �����¼�
		void cacheEvent(FMOD::Studio::EventDescription* desc);

		// �ͷ�ȫ���¼�����
		void clearEventCaches();

		// ����ʱ�Ƿ��ѹ��
		bool NeedLoadDecompress(){ return m_loadDecompress; }

		// ע��ص�����
		void setAudioEventCb(AudioSourceEventCB cb) { m_audioEventCallback = cb; }

		// ��ȡ�ص�����
		AudioSourceEventCB getAudioEventCb() { return m_audioEventCallback; }

	private:
		// ����bank�ļ�
		bool loadBankFile(const char* bankFile);

		// ж��bank�ļ�
		bool unloadBankFile(const char* bankFile);

	private:
		FMOD::Studio::System*	m_studioSystem;			// �߼�FMOD Studioϵͳ
		FMOD::System*			m_lowLevelSystem;		// �ͼ�FMODϵͳ
		BankMap					m_loadedBanks;			// �Ѽ��ص�bank��Դ
		AudioSourceMap			m_audioSources;			// ������Դ
		Audio3DAttributes		m_listenerAttributes;	// ��������
		map<int, bool>::type	m_isGroupEnabled;		// �ж����Ƿ����
		map<int, float>::type	m_groupVolumeFactor;	// ��������С����
		bool					m_SpeedAdjustEnable;	// �Ƿ���ʱ�����
		AudioSampleSet			m_audioSampleSet;		// �¼�����
		bool					m_loadDecompress;		// �Ƿ��ڼ�����������ǰ���룬����ʵʱ�����ĳЩ���͵�CPU����
		AudioSourceEventCB		m_audioEventCallback;	// �����¼��ص�
	};

	struct AudioExtraInfo
	{
		int lastTime;	//��һ֡ͣ����ʱ��
		bool firstTime; //�Ƿ��һ�ν�������߼�
	};
}