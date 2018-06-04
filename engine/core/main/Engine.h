#pragma once

#include "engine/core/render/render/Renderer.h"
#include "engine/core/main/EngineSettings.h"
#include "engine/core/main/EngineConsole.h"
#include "engine/core/Scene/NodeTree.h"
#include "FrameState.h"

namespace Echo
{
	class ProjectSettings;
	class ArchiveManager;
	class ArchiveFactory;
	class IO;
	class ImageCodecMgr;
	class RenderTargetManager;
	class ProfilerServer;
	class Engine
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

	public:
		// instance
		static Engine* instance();

		void tick(i32 elapsedTime);

		float getFrameTime() { return m_frameTime; }

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

		inline void		setEnableFrameProfile( bool _enable ){ m_enableFrameProfile = _enable; }

		inline bool		getEnableFrameProfile() const { return m_enableFrameProfile; }

		FrameState&		frameState() { return m_frameState; }

		const FrameState& frameState() const { return m_frameState; }

		void resetFrameState() { m_frameState.reset(); }
		void SetPhoneinformation(int max,int free,String cpu);

		void setEnableFilterAdditional( bool _val) { m_enableFilterAdditional = _val; }

		bool getEnableFilterAdditional() const { return m_enableFilterAdditional; }

		// ����֡�������ű�
		void setFrameBufferScale( float scale) { m_framebufferScale =  scale; }
		
		// ��ȡ֡�������ű�
		float getFramebufferScale() const { return m_framebufferScale; }
		
		void changeFilterAdditionalMap(const String& mapName);

		// ������Ŀ,�����ʼ��ʱ���Զ����ã�Ҳ�ɵ�������(ȫ·��)
		void loadProject( const char* projectFile);
        
        void loadAllBankFile();

		// ����Ϸ����ʱ��������Ҫ���еĴ���
		void onPlatformSuspend();

		// ����Ϸ�ӹ����лָ�ʱ������Ҫ���еĴ���
		void onPlatformResume();

	private:
		Engine();
		~Engine();

		// register all class types
		void registerClassTypes();

	public:
		ProjectSettings* getProjectFile() { return m_projectFile; }
		EngineSettingsMgr& getSettingsMgr() { return m_settingsMgr; }
		EngineConsole& getConsole() { return m_console; }

	protected:
		bool render();
		void loadLaunchScene();

	private:
		bool				m_isInited;				// �Ƿ��ѳ�ʼ��
		Config				m_cfg;					// �ͻ��˴���
		EngineSettingsMgr	m_settingsMgr;			// ���ù�����
		EngineConsole		m_console;				// �����п���̨
		String				m_resPath;				// ��Դ·��
		String				m_userPath;				// �û���Դ·��
		void*				m_pAssetMgr;			// for android
		bool				m_bRendererInited;
		float				m_frameTime;
		ui32				m_currentTime;
		Renderer*			m_renderer;						// ��Ⱦ��
		bool				m_enableFrameProfile;
		FrameState			m_frameState;
		int					Maxmemory;
		int					Freememory;
		String				cputex;
		bool				m_enableBloom;
		bool				m_enableFilterAdditional;
		float				m_framebufferScale;             // ֡����������
		ProjectSettings*		m_projectFile;					// ��Ŀ��Ϣ

#ifdef ECHO_PROFILER
		ProfilerServer*		m_profilerSev;					// ���ܷ���������
#endif
	};
}
