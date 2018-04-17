#include "FSAudioManager.h"
#include "FSAudioBase.h"
#include "Engine/Core.h"

namespace Echo
{
	// ΪloadBankCustom�ṩ��(��������)�ص�����
	FMOD_RESULT F_CALLBACK customFileOpen(const char* name, unsigned int *filesize, void **handle, void *userdata)
	{
		Echo::BankUserData* data = (Echo::BankUserData*)userdata;
		Echo::DataStream* stream = IO::instance()->open(data->filename.c_str());
		if (stream)
		{
			stream->seek(0, SEEK_END);
			size_t length = stream->tell();
			stream->seek(0, SEEK_SET);
			*filesize = (unsigned int)length;
			*handle = stream;
			return FMOD_OK;
		}
		EchoSafeDelete(stream, DataStream);
		return FMOD_ERR_FILE_BAD;
	}

	// ΪloadBankCustom�ṩ��(�������ر�)�ص�����
	FMOD_RESULT F_CALLBACK customFileClose(void *handle, void *userdata)
	{
		Echo::DataStream* stream = (Echo::DataStream*)handle;
		if (stream)
		{
			stream->close();
			EchoSafeDelete(stream, DataStream);
		}
		return FMOD_OK;
	}

	// ΪloadBankCustom�ṩ��(��������ȡ)�ص�����
	FMOD_RESULT F_CALLBACK customFileRead(void *handle, void* buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata)
	{
		*bytesread = 0;
		Echo::DataStream* stream = (Echo::DataStream*)handle;
		size_t read = stream->read(buffer, sizebytes);
		*bytesread = (unsigned int)read;

		if (read < sizebytes)
		{
			return FMOD_ERR_FILE_EOF;
		}

		return FMOD_OK;
	}

	// ΪloadBankCustom�ṩ��(��������λ)�ص�����
	FMOD_RESULT F_CALLBACK customFileSeek(void *handle, unsigned int pos, void *userdata)
	{
		Echo::DataStream* stream = (Echo::DataStream*)handle;
		stream->seek(pos, SEEK_SET);
		return FMOD_OK;
	}

	/**
	*���Androidƽ̨���޷�ֱ�Ӵ�APKͨ���ļ�·������ȡbank�ļ���������ʹ��loadbankMemory�ᵼ���ڴ�ռ�õ�����,���Ծ���ʹ��loadbankCustom
	**/
	struct AudioBank
	{
		FMOD::Studio::Bank* m_bank;		// FMODBank�ļ�
		BankUserData userdata;
		bool decompress = FSAudioManager::instance()->NeedLoadDecompress();
		AudioBank(const String& bankFile)
		{
			bool isStream = true;
#ifdef ECHO_PLATFORM_ANDROID
			isStream = false;
#endif // ECHO_PLATFORM_ANDROID

			if (isStream)
			{
				IO::instance()->getFullPath(bankFile);
				userdata.filename = bankFile;

				FMOD_STUDIO_BANK_INFO bankinfo;
				memset(&bankinfo, 0, sizeof(bankinfo));
				bankinfo.size = sizeof(bankinfo);
				bankinfo.openCallback = customFileOpen;
				bankinfo.closeCallback = customFileClose;
				bankinfo.readCallback = customFileRead;
				bankinfo.seekCallback = customFileSeek;
				bankinfo.userData = (void*)&userdata;

				FS_ERRORCHECK(FSAudioManager::instance()->getStudioSystem()->loadBankCustom(&bankinfo, decompress ? FMOD_STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES : FMOD_STUDIO_LOAD_BANK_NORMAL, &m_bank), nullptr);
			}
			else
			{
				MemoryReader memory = MemoryReader(bankFile);
				FS_ERRORCHECK(FSAudioManager::instance()->getStudioSystem()->loadBankMemory(memory.getData<const char*>(), memory.getSize(), FMOD_STUDIO_LOAD_MEMORY, decompress ? FMOD_STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES : FMOD_STUDIO_LOAD_BANK_NORMAL, &m_bank), nullptr);
			}
		}

		~AudioBank()
		{
		}
	};

	__ImplementSingleton(FSAudioManager);

	// ���캯��
	FSAudioManager::FSAudioManager()
		: m_studioSystem(NULL)
		, m_lowLevelSystem(NULL)
		, m_SpeedAdjustEnable(false)
		, m_loadDecompress(false)
		, m_audioEventCallback(NULL)
	{
		__ConstructSingleton;
	}

	// ��������
	FSAudioManager::~FSAudioManager()
	{
		__DestructSingleton;
	}

	// ��ʼ��
	void FSAudioManager::init(int maxVoribsCodecs, bool LoadDecompressed)
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		m_loadDecompress = LoadDecompressed;

		// 1.�½�FMODE Studio System
		FS_ERRORCHECK(FMOD::Studio::System::create(&m_studioSystem), nullptr);

		// 2.lowlevelSystem
		FS_ERRORCHECK(m_studioSystem->getLowLevelSystem(&m_lowLevelSystem), nullptr);
		FS_ERRORCHECK(m_lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_STEREO, NULL), nullptr);

		FMOD_ADVANCEDSETTINGS fmodSet = { sizeof(FMOD_ADVANCEDSETTINGS) };
		fmodSet.maxVorbisCodecs = maxVoribsCodecs;
		FS_ERRORCHECK(m_lowLevelSystem->setAdvancedSettings(&fmodSet), nullptr);
		// 3.channel
		FS_ERRORCHECK(m_studioSystem->initialize(maxVoribsCodecs, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_3D_RIGHTHANDED, NULL), nullptr);

		// 4.����������ص�����
		// ���ö�ȡbank��metadata��buffersizeΪ128K,Ĭ��Ϊ2k�ᵼ�¶�ȡ��ȥƵ��
		FS_ERRORCHECK(m_lowLevelSystem->setFileSystem(NULL, NULL, NULL, NULL, NULL, NULL, 1024 * 128), nullptr);
		// ���ö�ȡbank��Streamingdata��buffersizeΪ64K,Ĭ��Ϊ16K�����ٶ�ȡ����
		FS_ERRORCHECK(m_lowLevelSystem->setStreamBufferSize(64 * 1024, FMOD_TIMEUNIT_RAWBYTES), nullptr);
#endif
	}

	// �߼���ѭ����
	void FSAudioManager::tick(ui32 elapsedTime)
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		// ϵͳ����
		FS_ERRORCHECK(m_studioSystem->update(), nullptr);

		// �Ƴ���ѭ��������Դ
		for (AudioSourceMap::iterator it = m_audioSources.begin(); it != m_audioSources.end();)
		{
			AudioSource* audioSource = it->second;
			if (!elapsedTime && audioSource->isPausable())
			{
				audioSource->pause();
			}
			if (elapsedTime && audioSource->isPausable())
			{
				audioSource->resume();
			}
			if (audioSource->isStopped() && audioSource->getCinfo().m_isOneShoot)
			{
				EchoSafeDelete(it->second, AudioSource);
				m_audioSources.erase(it++);
			}
			else
			{
				it++;
			}
		}
#endif
	}

	// �ͷ�
	void FSAudioManager::release()
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		for (BankMap::iterator it = m_loadedBanks.begin(); it != m_loadedBanks.end(); it++)
		{
			EchoSafeDelete(it->second, AudioBank);
		}
		destroyAllAudioSources();

		m_loadedBanks.clear();

		FS_ERRORCHECK(m_studioSystem->release(), nullptr);
#endif
	}

	// ��������bank�ļ�
	void FSAudioManager::loadAllBankFile()
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else

		// ��ȡ����bank�ļ�
		StringArray bankFiles;
		IO::instance()->listFilesWithExt(bankFiles, ".bank");
		for (size_t i = 0; i<bankFiles.size(); i++)
		{
			loadBankFile(bankFiles[i].c_str());
		}
#endif
	}

	// ����bank�ļ�
	bool FSAudioManager::loadBankFile(const char* bankFile)
	{
#ifdef ECHO_PLATFORM_HTML5
		return false;
#else
		String key = bankFile;
		StringUtil::LowerCase(key);
		if (m_loadedBanks.find(key) == m_loadedBanks.end())
		{
			m_loadedBanks[key] = EchoNew(AudioBank(key));

			return true;
		}
		return false;
#endif
	}

	// ж��bank�ļ�
	bool FSAudioManager::unloadBankFile(const char* bankFile)
	{
#ifdef ECHO_PLATFORM_HTML5
		// return false
#else
		BankMap::iterator it = m_loadedBanks.find(bankFile);
		if (it != m_loadedBanks.end())
		{
			EchoSafeDelete(it->second, AudioBank);
			m_loadedBanks.erase(it);

			return true;
		}

		return false;
#endif
	}

	// ��ȡ��Դ
	AudioSource* FSAudioManager::getAudioSource(AudioSourceID asID)
	{
#ifdef ECHO_PLATFORM_HTML5
		return NULL;
#else
		AudioSourceMap::iterator it = m_audioSources.find(asID);
		if (it != m_audioSources.end())
			return it->second;

		return NULL;
#endif
	}

	// ������Դ
	AudioSourceID FSAudioManager::createAudioSource(const AudioSource::Cinfo& cinfo)
	{
#ifdef ECHO_PLATFORM_HTML5
		return 0;
#else
		if (!isGroupEnabled(cinfo.m_group))
		{
			return 0;
		}

		AudioSource* audioSource = EchoNew(AudioSource(cinfo));
		m_audioSources[audioSource->getIdentifier()] = audioSource;

		return audioSource->getIdentifier();
#endif
	}

	// ������Դ
	void FSAudioManager::destroyAudioSources(AudioSourceID* audioSourceIDs, int num)
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		for (int i = 0; i<num; i++)
		{
			AudioSourceMap::iterator it = m_audioSources.find(audioSourceIDs[i]);
			if (it != m_audioSources.end())
			{
				it->second->stop();
				EchoSafeDelete(it->second, AudioSource);
				m_audioSources.erase(it);
			}
		}
#endif
	}

	// ����������Դ
	void FSAudioManager::destroyAllAudioSources()
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		// �Ƴ���ѭ��������Դ
		for (AudioSourceMap::iterator it = m_audioSources.begin(); it != m_audioSources.end(); it++)
		{
			it->second->stop();
			EchoSafeDelete(it->second, AudioSource);
		}

		m_audioSources.clear();
#endif
	}

	// ������������
	void FSAudioManager::setListenerAttributes(const Audio3DAttributes& attributes)
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		// ������������
		m_listenerAttributes = attributes;
		m_listenerAttributes.m_forward.y = 0.f;
		m_listenerAttributes.m_forward.normalize();

		EchoAssert(sizeof(m_listenerAttributes) == sizeof(FMOD_3D_ATTRIBUTES));
		FS_ERRORCHECK(m_studioSystem->setListenerAttributes(0, (FMOD_3D_ATTRIBUTES*)&m_listenerAttributes), nullptr);

		// ��������2D��Դ(2Dģʽ���ŵ�3D��Դ)����λ��������һ��
		for (AudioSourceMap::iterator it = m_audioSources.begin(); it != m_audioSources.end(); it++)
		{
			AudioSource* audioSource = it->second;
			if (!audioSource->is3DMode() && audioSource->is3D())
			{
				audioSource->set3dAttributes(m_listenerAttributes);
			}
		}
#endif
	}

	// ���ݱ�ǩ��ȡ��Դ
	void FSAudioManager::listAudioSourcesByGroup(AudioSourceIdArray& oIds, int groupFilter)
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		oIds.clear();

		// �Ƴ���ѭ��������Դ
		for (AudioSourceMap::iterator it = m_audioSources.begin(); it != m_audioSources.end(); it++)
		{
			if (it->second->getCinfo().m_group & groupFilter)
			{
				oIds.push_back(it->first);
			}
		}
#endif
	}

	// �ж����Ƿ����
	bool FSAudioManager::isGroupEnabled(int group) const
	{
#ifdef ECHO_PLATFORM_HTML5
		return false;
#else
		map<int, bool>::type::const_iterator it = m_isGroupEnabled.find(group);
		if (it != m_isGroupEnabled.end())
		{
			return it->second;
		}

		return true;
#endif
	}

	// ������������������
	void FSAudioManager::setGroupVolumeFactor(int group, float volumeFactor)
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		volumeFactor = Math::Clamp(volumeFactor, 0.f, 1.f);
		m_groupVolumeFactor[group] = volumeFactor;

		AudioSourceIdArray ids;
		listAudioSourcesByGroup(ids, group);
		for (size_t i = 0; i<ids.size(); i++)
		{
			AudioSource* source = getAudioSource(ids[i]);
			if (source)
			{
				source->setVolume(source->getCinfo().m_volume);
			}
		}
#endif
	}

	// ��ȡ��������������
	float FSAudioManager::getGroupVolumeFactor(int group) const
	{
#ifdef ECHO_PLATFORM_HTML5
		return 1.f;
#else
		map<int, float>::type::const_iterator it = m_groupVolumeFactor.find(group);
		if (it != m_groupVolumeFactor.end())
		{
			return it->second;
		}

		return 1.f;
#endif
	}

	// ����bank�ļ�����ȡ�¼�
	void FSAudioManager::getAudioEvents(StringArray& oEvents, const char* bankName)
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		BankMap::iterator it = m_loadedBanks.find(bankName);
		if (it != m_loadedBanks.end())
		{
			FMOD::Studio::Bank* bank = it->second->m_bank; EchoAssert(bank);

			// ��ȡ�¼�����
			int eventCount = 0;
			FS_ERRORCHECK(bank->getEventCount(&eventCount), nullptr);
			if (eventCount)
			{
				// ��ȡ�¼�����
				int getCount = 0;
				vector<FMOD::Studio::EventDescription*>::type eventDescriptions;
				eventDescriptions.resize(eventCount);
				FS_ERRORCHECK(bank->getEventList(eventDescriptions.data(), eventCount, &getCount), nullptr);
				EchoAssert(getCount == eventCount);

				// ��ȡ�¼�����
				char eventName[512];
				for (int i = 0; i<getCount; i++)
				{
					FS_ERRORCHECK(eventDescriptions[i]->getPath(eventName, 512, NULL), nullptr);
					oEvents.push_back(eventName);
				}
			}
		}
#endif
	}

	// ��ȡ�����¼�
	void FSAudioManager::getAllAudioEvent(StringArray& oEvents)
	{
#ifdef ECHO_PLATFORM_HTML5
		oEvents.clear();
#else
		oEvents.clear();
		for (BankMap::iterator it = m_loadedBanks.begin(); it != m_loadedBanks.end(); it++)
		{
			getAudioEvents(oEvents, it->first.c_str());
		}
#endif
	}

	//����Fmod����ϵͳ
	void FSAudioManager::suspendFmodSystem()
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		if (m_lowLevelSystem)
		{
			m_lowLevelSystem->mixerSuspend();
		}
#endif
	}

	//�ӹ����лָ�Fmod����ϵͳ
	void FSAudioManager::resumeFmodSystem()
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		if (m_lowLevelSystem)
		{
			m_lowLevelSystem->mixerResume();
		}
#endif
	}

	// �����¼�����

	void FSAudioManager::cacheEvent(FMOD::Studio::EventDescription* desc)
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		if (!desc)
		{
			EchoLogError("in function FSAudioManager::cacheEvent, FMOD::Studio::EventDescription* is nullptr.");
			return;
		}

		if (m_audioSampleSet.find(desc) == m_audioSampleSet.end())
		{
			bool isStream;
			FS_ERRORCHECK(desc->isStream(&isStream), nullptr);
			if (!isStream)
			{
				desc->loadSampleData();
				m_audioSampleSet.insert(desc);
			}
		}
#endif
	}

	// �ͷ�ȫ���¼�����
	void FSAudioManager::clearEventCaches()
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		if (!m_audioSampleSet.empty())
		{
			AudioSampleSet::iterator it;
			for (it = m_audioSampleSet.begin(); it != m_audioSampleSet.end(); it++)
			{
				(*it)->unloadSampleData();
			}
			m_audioSampleSet.clear();
		}
#endif
	}
}