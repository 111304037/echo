#pragma once

#include "Texture.h"

namespace Echo
{
	#define MAX_GUI_RT_COUNT	 12
	#define MAX_LIGHTPROBE_COUNT 50

	/**
	 * ��ȾĿ��ӿ�
	 */
	class RenderTarget
	{
	public:
		struct Options
		{
			bool					depth;
			bool					msaa;
			bool					multiResolution;
			bool					cubemap;
			RenderTarget*			depthTarget;

			Options()
				: depth(false)
				, msaa(false)
				, multiResolution(false)
				, cubemap(false)
				, depthTarget(nullptr)
			{}
		};

		struct Tiled
		{
			float x_, y_;
			float width_, height_;

			Tiled( float x = 0.0f, float y = 0.0f, float w = 0.0f, float h = 0.0f )
				: x_( x ),y_( y ),width_( w ),height_(h)
			{
			}
		};

		enum
		{
			Invalid_Value = 0xffffffff
		};

	public:

		ui32& width(){ return m_width; }

		const ui32& width() const { return m_width; }

		ui32& height(){ return m_height; }

		const ui32& height() const { return m_height; }

		bool hasDepth() const { return m_bHasDepth; }

		bool hasMSAA() const { return m_bHasMSAA; }

		ui32 id() const { return m_id; }

		PixelFormat pixelFormat() const { return m_pixelFormat; }

		void setEnableTiled( bool _Val ) { m_tiledRender = _Val; }

		bool getEnableTiled() const { return m_tiledRender; }

		Tiled& curRenderTiled() { return m_curRenderTiled; }

		const Tiled& curRenderTiled() const { return m_curRenderTiled; }

		// ��ȡ��ȾĿ�������
		Texture* getBindTexture() { return m_bindTexture.m_texture; }
		const TextureSampler& getBindTextureSampler() { return m_bindTexture; }
		// ��ȡ��ȾĿ�����ͼ
		Texture* getDepthTexture() { return m_depthTexture.m_texture; }
		const TextureSampler& getDepthTextureSampler() { return m_depthTexture; }

		// �Ƿ�Ϊ��������ͼ
		bool isCubemap() const { return m_isCubemap; }

		// ����
		bool create();

		// ����Ϊ��ǰ��ȾĿ��
		bool beginRender(bool _clearColor, const Color& _backgroundColor, bool _clearDepth, float _depthValue, bool _clearStencil, ui8 _stencilValue);

		// ������Ⱦ����Ҫ�л�Ϊ�������ȾĿ��
		bool endRender();
        
        bool invalidateFrameBuffer(bool invalidateColor, bool invalidateDepth, bool invalidateStencil);

		// �󶨵�Target�����Ұ��
		virtual void bindTarget_left() = 0;
		virtual void bindTarget_right() = 0;

		// ���
		void clear(bool _clearColor, const Color& _backgroundColor, bool _clearDepth, float _depthValue, bool _clearStencil, ui8 _stencilValue);

		// �޸ĳߴ��С
		void onResize( ui32 _width, ui32 _height ) { return doOnResize(_width, _height); }

		// ���������
		bool saveTo( const char* file ){ return doSaveTo( file ); }

		// �˷������Ƴ�
		virtual bool doStoreDefaultRenderTarget()=0;

		// �˷������Ƴ�
		virtual bool doRestoreDefaultRenderTarget()=0;

		bool resolutionRelative() const { return m_handlerOnsize; }
		void setResolutionRelative(bool relative) { m_handlerOnsize = relative; }
		float scaleFactor() const { return m_scale; }
		void setScaleFactor(float factor) { m_scale = factor; }

		void reusageDepthTarget(RenderTarget* depthTarget);

		ui32 getMemorySize();

	public:
		RenderTarget(ui32 _id, ui32 _width, ui32 _height, PixelFormat _format, const Options& option = Options());
		virtual ~RenderTarget();
		virtual bool doCreate() = 0;
		virtual bool doCreateCubemap() = 0;
		virtual bool doBeginRender( bool _clearColor, const Color& _backgroundColor,  bool _clearDepth, float _depthValue, bool _clearStencil, ui8 stencilValue ) = 0;
		virtual bool doEndRender() = 0;
        virtual bool doInvalidateFrameBuffer(bool invalidateColor, bool invalidateDepth, bool invalidateStencil) = 0;
		virtual void doClear(bool clear_color, const Color& color, bool clear_depth, float depth_value, bool clear_stencil, ui8 stencil_value) = 0;
		virtual void doOnResize( ui32 _width, ui32 _height ) = 0;
		virtual void doSetCubeFace( Texture::CubeFace cf ) = 0;
		virtual bool doSaveTo( const char* file ) = 0;

	protected:
		bool					m_bHasMSAA;
		ui32					m_id;
        PixelFormat                m_pixelFormat;
		bool					m_bHasDepth;
		ui32					m_width;
		ui32					m_height;
		ui32					m_clearFlags;
		TextureSampler			m_bindTexture;
		TextureSampler			m_depthTexture;
		ui32					m_RenderFrameCount;
		bool					m_bLogicResolution;
		bool					m_isCubemap;
		Texture::CubeFace		m_curCubeface;
		Tiled					m_curRenderTiled;
		bool					m_tiledRender;
		bool					m_handlerOnsize;
		float					m_scale;
		RenderTarget*			m_depthTarget;
	public:
		bool					m_bFrameBufferChange;
		bool					m_bViewportChange;
	};
	typedef map<ui32, RenderTarget*>::type	RenderTargetMap;
}
