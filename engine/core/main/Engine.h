#pragma once

#include "engine/core/render/render/Renderer.h"
#include "engine/core/Scene/NodeTree.h"
#include "FrameState.h"
#include "ProjectSettings.h"

namespace Echo
{
	class IO;
	class Engine
	{	
	public:
		// ����
		struct Config
		{
			String				m_projectFile;
			int					m_AudiomaxVoribsCodecs;
			bool				m_AudioLoadDecompresse;
			bool				m_isEnableProfiler;			// �Ƿ������ܷ�������
			unsigned int		m_windowHandle;
			bool				m_isGame;

			Config()
				: m_projectFile("")
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

		// ��ȡ����
		const Config& getConfig() const { return m_cfg; }

		// ��ȡ��Դ��·��
		const String& getResPath() const;

		// ��ȡ�û���Դ·��
		const String& getUserPath() const;

		// �����û���Դ·��
		void setUserPath(const String& strPath);

		bool isRendererInited() const;
		const ui32&		getCurrentTime() const;

		FrameState&		frameState() { return m_frameState; }

		const FrameState& frameState() const { return m_frameState; }

		void resetFrameState() { m_frameState.reset(); }

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
		ProjectSettings* getProjectFile() { return m_projectSettings.ptr(); }

	protected:
		bool render();
		void loadLaunchScene();

	private:
		bool				m_isInited;				// �Ƿ��ѳ�ʼ��
		Config				m_cfg;					// �ͻ��˴���
		String				m_resPath;				// ��Դ·��
		String				m_userPath;				// �û���Դ·��
		bool				m_bRendererInited;
		float				m_frameTime;
		ui32				m_currentTime;
		Renderer*			m_renderer;				// ��Ⱦ��
		FrameState			m_frameState;
		ProjectSettingsPtr	m_projectSettings;		// ��Ŀ��Ϣ
	};
}
