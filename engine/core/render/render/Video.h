#pragma once
#include "Texture.h"
#include <Engine/core/Render/TextureRes.h>
#include "engine/core/Util/Singleton.h"

class TheoraVideoManager;
class TheoraVideoClip;
class OpenAL_AudioInterfaceFactory;

namespace Echo
{
	class VideoPlay : public Singleton<VideoPlay>
	{
	public:
		VideoPlay();
		virtual ~VideoPlay();

		// ��ʼ��
		virtual bool parseVideoFile(const String& name, ui32& width, ui32& height);

		// ����
		virtual void tick(Echo::ui32 elapsedTime);

		// ����
		virtual void destroy();

		virtual TextureSampler* getTextureSampler()
		{
			return &m_texSampler;
		}

		virtual TextureRes* getTextureRes()
		{
			return m_pTex;
		}

		/////
		void play();
		void pause();
		void stop();
		void restart();

		void setSilent(bool silent);
		bool isSilent(){ return m_bSilent; }

		void finishCallback(std::function<void()> videoEndCallback)
		{
			m_videoEndCallback = videoEndCallback;
		}

		void releaseTexture();

	private:

		TextureRes	*m_pTex;
		TextureSampler m_texSampler;

		String		m_fileName;

		unsigned int		m_width;
		unsigned int		m_height;

		TheoraVideoManager* m_mgr;
		TheoraVideoClip* m_clip;
		OpenAL_AudioInterfaceFactory* m_iface_factory;
		double				m_elapsed;

		std::function<void()>	m_videoEndCallback;

		// flag
		bool				m_bStarted;
		bool				m_bSilent;
	};
}

