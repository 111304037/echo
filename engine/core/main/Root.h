#pragma once

#include "engine/core/render/render/Renderer.h"
#include "engine/core/Resource/EchoThread.h"
#include "engine/core/main/EngineSettings.h"
#include "engine/core/main/EngineConsole.h"
#include "engine/core/Util/Singleton.h"
#include "engine/core/Scene/NodeTree.h"
#include "FrameState.h"

namespace Echo
{
	class ProjectFile;
	class ArchiveManager;
	class ArchiveFactory;
	class IO;
	class ImageCodecMgr;
	class RenderTargetManager;
	class ProfilerServer;
	class Root
	{	
	public:
		// �ⲿģ��ӿ�
		struct ExternalMgr
		{
			typedef std::function<void(int)> TickFun;
			typedef std::function<void()> ReleaseFun;
			typedef std::function<void()> RenderFun;

			String		m_name;			// �������
			TickFun		m_tick;			// ���º��� 
			RenderFun	m_render;		// ��Ⱦ����
			ReleaseFun	m_release;		// �ͷź���
			bool		m_isReleased;	// �Ƿ��Ѿ����ͷ�
		};

		// ����
		struct Config
		{
			typedef vector<ArchiveFactory*>::type ArchiveFactoryTypes;
			typedef vector<ExternalMgr>::type ExternalMgrs;

			String				projectFile;
			String				engineCfgFile;
			void*				pAssetMgr;					// for Android platfrom
			ArchiveFactoryTypes	externalArchiveFactorys;	// external archive factory
			int					m_AudiomaxVoribsCodecs;
			bool				m_AudioLoadDecompresse;
			bool				m_isEnableProfiler;			// �Ƿ������ܷ�������
			ExternalMgrs		m_externalMgrs;				// �ⲿģ��

			unsigned int		m_windowHandle;
			bool				m_isGame;

			Config()
				: projectFile("")
				, engineCfgFile("engine.xml")
				, pAssetMgr(NULL)
				, m_AudiomaxVoribsCodecs(32)
				, m_AudioLoadDecompresse(false)
				, m_isEnableProfiler(false)
				, m_isGame(true)
			{}
		};

		__DeclareSingleton(Root);

	public:
		Root();
		~Root();

		void tick(i32 elapsedTime);

		// �Ƿ��ѳ�ʼ��
		bool isInited() const { return m_isInited; }

		// װ����־ϵͳ
		bool initLogSystem();
		bool initialize(const Config& cfg);
		bool initRenderer(Renderer* pRenderer, const Renderer::RenderCfg& config);
		bool onRendererInited();

		// screen size changed
		bool onSize(ui32 width, ui32 height);
		void destroy();
		void releasePlugins();

		// ��ȡ����
		const Config& getConfig() const { return m_cfg; }

		// ��ȡ��Դ��·��
		const String& getResPath() const;

		// ��ȡ�û���Դ·��
		const String& getUserPath() const;

		// �����û���Դ·��
		void setUserPath(const String& strPath);

		void* getAssetManager() const;
		bool isRendererInited() const;
		const ui32&		getCurrentTime() const;
		inline ui32		getFrameTime() const { return m_frameTime; }
		inline ui32		getFrameRealTime() const { return m_frameRealTime; }
		ui32			getFrameCount() const;
		ui32			getFPS() const;
		ui32			getMaxFrameTime() const;

		void			enableStreamThread(bool enable);
		StreamThread*	getThreadThread() const;

		inline void		setEnableFrameProfile( bool _enable ){ m_enableFrameProfile = _enable; }

		inline bool		getEnableFrameProfile() const { return m_enableFrameProfile; }

		inline FrameState&		frameState() { return m_frameState; }

		inline const FrameState& frameState() const { return m_frameState; }

		void outputFrameState() { m_frameState.output(); }
		void resetFrameState() { m_frameState.reset(); }
		void SetPhoneinformation(int max,int free,String cpu);

		// ��ȡ��ǰ֡����
		ui32 getCurFrameCount() const { return m_curFameCount; }

		void setEnableFilterAdditional( bool _val) { m_enableFilterAdditional = _val; }

		bool getEnableFilterAdditional() const { return m_enableFilterAdditional; }

		// ����֡�������ű�
		void setFrameBufferScale( float scale) { m_framebufferScale =  scale; }
		
		// ��ȡ֡�������ű�
		float getFramebufferScale() const { return m_framebufferScale; }
		
		void changeFilterAdditionalMap(const String& mapName);

		// �����Ƿ���Ⱦ��ǰ����
		void setRenderSceneEnable( bool isRenderScene) { m_isRenderScene = isRenderScene; }

		// �Ƿ���Ⱦ����
		bool isRenderScene() { return m_isRenderScene; }

		// ������Ŀ,�����ʼ��ʱ���Զ����ã�Ҳ�ɵ�������(ȫ·��)
		void loadProject( const char* projectFile);
        
        void loadAllBankFile();

		// ����Ϸ����ʱ��������Ҫ���еĴ���
		void onPlatformSuspend();

		// ����Ϸ�ӹ����лָ�ʱ������Ҫ���еĴ���
		void onPlatformResume();

		// ������Դ�ӳ��ͷ�ʱ��
		void setReleaseDelayTime(ui32 t);

	private:
		// register all class types
		void registerClassTypes();

	public:
		ProjectFile* getProjectFile() { return m_projectFile; }
		MemoryManager* getMemoryManager() { EchoAssert(m_memoryManager);  return m_memoryManager; }
		LogManager* getLogManager() { EchoAssert(m_logManager);  return m_logManager; }
		SkeletonManager* getSkeletonManager() { EchoAssert(m_skeletonManager);  return m_skeletonManager; }
		AnimManager* getAnimManager() { EchoAssert(m_animManager);  return m_animManager; }
		AnimSystemManager* getAnimSysManager() { EchoAssert(m_animSysManager);  return m_animSysManager; }
		ModelManager* getModelManager() { EchoAssert(m_modelManager);  return m_modelManager; }
		EffectSystemManager* getEffectSystemManager() { EchoAssert(m_EffectSystemManager);  return m_EffectSystemManager; }
		IO* getResourceGroupManager() { EchoAssert(m_io); return m_io; }
		TextureResManager* getTextureResManager() { EchoAssert(m_textureResManager);  return m_textureResManager; }
		OpenMPTaskMgr* getOpenMPTaskMgr() { EchoAssert(m_openMPTaskMgr);  return m_openMPTaskMgr; }
		EngineSettingsMgr& getSettingsMgr() { return m_settingsMgr; }
		EngineConsole& getConsole() { return m_console; }
		ImageCodecMgr* getImageCodecManager() { EchoAssert(m_imageCodecManager);  return m_imageCodecManager; }
		FSAudioManager* getAudioManager() { EchoAssert(m_audioManager);  return m_audioManager; }

	protected:
		void updateAllManagerDelayResource();
		bool render();
		void loadFirstScene();

	private:
		bool				m_isInited;				// �Ƿ��ѳ�ʼ��
		Config				m_cfg;					// �ͻ��˴���
		EngineSettingsMgr	m_settingsMgr;			// ���ù�����
		EngineConsole		m_console;				// �����п���̨
		String				m_resPath;				// ��Դ·��
		String				m_userPath;				// �û���Դ·��
		void*				m_pAssetMgr;			// for android
		bool				m_bRendererInited;

		bool				m_bSupportGPUSkin;

		ui32				m_lastTime;
		ui32				m_frameTime;
		ui32				m_frameRealTime;
		ui32				m_currentTime;
		ui32				m_timeCount;
		ui32				m_frameCount;
		ui32				m_curFameCount;
		ui32				m_fps;
		ui32				m_maxFrameTime;
		bool				m_isRenderScene;				// �Ƿ���Ⱦ����

		MemoryManager*		m_memoryManager;				// �ڴ������
		LogManager*			m_logManager;
		IO*					m_io;
		TextureResManager*	m_textureResManager;			// ������Դ������
		SkeletonManager*	m_skeletonManager;				// ������Դ������
		AnimManager*		m_animManager;					// ����������
		AnimSystemManager*	m_animSysManager;				// ����ϵͳ������
		ImageCodecMgr*		m_imageCodecManager;			// ������������
		ModelManager*		m_modelManager;					// ģ�͹�����
		Time*				m_Timer;
		EffectSystemManager*m_EffectSystemManager;			// ��Чϵͳ������
		Renderer*			m_renderer;						// ��Ⱦ��
		NodeTree*		m_sceneManager;					// ����������
		FSAudioManager*		m_audioManager;					// ��Ƶ������
		StreamThread*		m_StreamThreading;				// �������߳�
		bool				m_enableFrameProfile;
		FrameState			m_frameState;
		int					Maxmemory;
		int					Freememory;
		String				cputex;
		bool				m_enableBloom;
		bool				m_enableFilterAdditional;
		float				m_framebufferScale;             // ֡����������
		OpenMPTaskMgr*		m_openMPTaskMgr;				// OpenMP��������
		RenderTargetManager*m_renderTargetManager;			// ��ȾĿ�������
		ProjectFile*		m_projectFile;					// ��Ŀ��Ϣ

#ifdef ECHO_PROFILER
		ProfilerServer*		m_profilerSev;					// ���ܷ���������
#endif
	};
}

#define EchoRoot					Echo::Root::instance()
#define EchoMemoryManager			EchoRoot->getMemoryManager()
#define EchoLogManager				EchoRoot->getLogManager()
#define EchoSkeletonManager			EchoRoot->getSkeletonManager()
#define EchoAnimManager				EchoRoot->getAnimManager()
#define EchoAnimSystemManager		EchoRoot->getAnimSysManager()
#define EchoEffectSystemManager		EchoRoot->getEffectSystemManager()
#define EchoTextureResManager		EchoRoot->getTextureResManager()
#define EchoOpenMPTaskMgr			EchoRoot->getOpenMPTaskMgr()
#define EchoEngineSettings			EchoRoot->getSettingsMgr()
#define EchoEngineConsole			EchoRoot->getConsole()
#define EchoModelManager			EchoRoot->getModelManager()
#define EchoImageCodecManager		EchoRoot->getImageCodecManager()
#define EchoAudioManager			EchoRoot->getAudioManager()
