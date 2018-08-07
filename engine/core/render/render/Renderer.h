#pragma once

#include <engine/core/util/Buffer.h>
#include "Color.h"
#include <engine/core/math/Vector3.h>
#include <engine/core/math/Matrix4.h>
#include "engine/core/thread/Threading.h"
#include "RenderInput.h"
#include "RenderState.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderable.h"
#include "RenderTarget.h"

namespace Echo
{
	class DeviceFeature
	{
	public:
		DeviceFeature();
		~DeviceFeature();

		const static String cs_pvr_format;
		const static String cs_etc1_format;
		const static String cs_atitc_format;
		const static String cs_atitc_format2;
		const static String cs_dxt1_format;
		const static String cs_s3tc_format;
		const static String cs_s3tc_format2;
		const static String cs_half_float_texture;
		const static String cs_half_float_texture_linear;
		const static String cs_depth_24;
		const static String cs_depth_32;
		const static String cs_program_binary;
		const static String cs_color_buffer_half_float;
		void initialize();

		void checkOESExtensionSupport(const String& features);

		bool supportPVR() const;
		bool supportDXT1() const;
		bool supportATITC() const;
		bool supportETC1() const;
		bool supportETC2() const;
		bool supportHFTexture() const;
		bool supportHFTextureLinear() const;
		bool supportDepth24() const;
		bool supportDepth32() const;
		bool supportBinaryProgram() const;
		bool supportGLES30() const;
		bool supportHFColorBf() const;
		bool supportHFColorBf1() const;

		String& rendererName() { return m_rendererName; }

		const String& rendererName() const { return m_rendererName; }

		String& glesVersion(){ return m_glesVersion; }

		const String& glesVersion() const { return m_glesVersion; }

		String& vendor() { return m_vendor; }

		const String& vendor() const { return m_vendor; }

		String& shadingLangVersion(){ return m_shadingLanVersion; }

		const String& shadingLangVersion() const { return m_shadingLanVersion; }

		static void SetSupportGLES3(bool value);
		static void SetSupportETC2(bool value);

	protected:
		typedef std::map<String, bool> FeaturesMap;
		FeaturesMap m_features;
		String		m_rendererName;
		String		m_glesVersion;
		String		m_vendor;
		String		m_shadingLanVersion;
		bool		m_supportDXT1;
		bool		m_supportPVR;
		bool		m_supportATITC;
		bool		m_supportETC1;
		bool		m_supportBinaryProgram;

		bool		m_supportHalfFloatTexture;
		bool		m_supportHalfFloatTextureLinear;

		bool		m_supportDepth24;				// is device support depth 24;
		bool		m_supportDepth32;				// is device support depth 32;

		bool		m_supportHalfFloatColorBuffer;

		static bool s_supportGLES3;
		static bool s_supportETC2;
	};


	class Renderer
	{
		typedef RasterizerState::PolygonMode PolygonFillMode;
	public:
		// ��Ⱦ����
		struct RenderCfg
		{
			ui32	screenWidth;
			ui32	screenHeight;
			float	nearClip;
			float	farClip;
			bool	bFullscreen;
			bool	bVSync;
			bool	bSeparatedAlpha;
			bool	enableThreadedRendering;

			// for Windows Editor.
			unsigned int windowHandle;

			RenderCfg()
				: screenWidth(800)
				, screenHeight(600)
				, nearClip(0.1f)
				, farClip(10000.0f)
				, bFullscreen(false)
				, bVSync(false)
				, bSeparatedAlpha(false)
				, enableThreadedRendering(true)
                , windowHandle(0)
			{}
		};

		// ��ǰ֡��Ⱦ״̬
		class FrameState
		{
		public:
			FrameState()
				: m_uploadedTextureSizeInBytes(0)
				, m_uploadedGeometricSize(0)
				, m_rendertargetSize(0)
				, m_drawCallTimes(0)
				, m_lockTimes(0)
				, m_fps(0)
                , m_triangleNum(0)
			{}

			~FrameState()
			{}

			// ����
			void reset()
			{
				m_triangleNum = 0;
				m_drawCallTimes = 0;
				m_lockTimes = 0;
				m_fps = 0;
				m_maxFrameTimePerSecond = 0;
			}

			// ��������ռ�ô�С(��λ���ֽ�)
			void incrUploadTextureSizeInBytes(ui32 _sizeInBytes) { m_uploadedTextureSizeInBytes += _sizeInBytes; }

			// ��������ռ�ô�С(��λ���ֽ�)
			void decrUploadTextureSizeInBytes(ui32 _sizeInBytes) { m_uploadedTextureSizeInBytes -= _sizeInBytes; }

			// ���Ӽ����������δ�С
			void incrUploadGeometricSize(ui32 _sizeInBytes) { m_uploadedGeometricSize += _sizeInBytes; }

			// ���ټ������ڼ�¼��С
			void decrUploadGeometricSize(ui32 _sizeInBytes) { m_uploadedGeometricSize -= _sizeInBytes; }

			// ������Ⱦ����
			void incrDrawCallTimes(ui32 _times) { m_drawCallTimes += _times;}

			// ����������
			void incrLockTimes(ui32 _time) { m_lockTimes += _time; }
			
			// ��������������
			void incrTriangleNum(ui32 _triangles) { m_triangleNum += _triangles; }

			// ��ȡ��Ⱦ����
			ui32 getDrawCalls() const { return m_drawCallTimes; }

			// ��ȡ����ߴ�
			ui32 getTextureSizeInBytes() const { return m_uploadedTextureSizeInBytes; }

			// ��ȡ�����С
			ui32 getVertexSize() const { return m_uploadedGeometricSize; }

			// ��ȡ����������
			ui32 getTriangleNum() const { return m_triangleNum; }

			// ����rendertarget�ڴ��С
			void incrRendertargetSize(const ui32 size) { m_rendertargetSize += size; }
			void decrRendertargetSize(const ui32 size) { m_rendertargetSize -= size; }

			// ��ȡrendertarget�ڴ��С
			ui32 getRendertargetSize() const { return m_rendertargetSize; }

		protected:
			ui32	m_uploadedTextureSizeInBytes;
			ui32	m_uploadedGeometricSize;
			ui32	m_rendertargetSize;
			ui32	m_drawCallTimes;
			ui32	m_lockTimes;
			ui32	m_fps;
			ui32	m_maxFrameTimePerSecond;
			ui32	m_minTimeFrame;
			ui32	m_maxTimeFrame;
			ui32	m_triangleNum;
			String	m_fpsMsg;
			String	m_maxFrameTimeMsg;
			String	m_lockTimesMsg;
			String	m_drawCallTimesMsg;
			String	m_triangleNumMsg;
			String	m_uploadedTextureSizeInBytesMsg;
			String	m_uploadedGeometricSizeMsg;
			String	m_streamThreadCount;
			String	m_mathOpCount;
		};

		static Color BGCOLOR;
		typedef map<ui32, SamplerState*>::type	StageSamplerMap;

	public:
		Renderer();
		virtual ~Renderer();

		// ����ʵ��
		static Renderer* instance();

		// �滻ʵ��
		static bool replaceInstance(Renderer* inst);

		// ��ʼ��
		bool initialize(const RenderCfg& config);
		
		// Ӳ��֧���Լ��
		virtual void checkOpenGLExtensions() = 0;

		// �����ӿ�
		virtual void setViewport(Viewport* pViewport) = 0;

		// ���ù�դ��״̬
		virtual void setRasterizerState(RasterizerState* pState);

		// �������ģ��״̬
		virtual void setDepthStencilState(DepthStencilState* pState);

		// ���û��״̬
		virtual void setBlendState(BlendState* pState);

		// �����������������
		virtual void setTexture(ui32 index, const TextureSampler& sampler,bool needUpdate = false) = 0;

	#ifdef ECHO_EDITOR_MODE
		// ���ö�������ģʽ(ȫ�ֽӿ�)
		virtual void setPolygonFillMode(RasterizerState::PolygonMode polyFillMode) { m_polygonFillMode = polyFillMode; }

		// ��ȡ��������ģʽ
		virtual RasterizerState::PolygonMode getPolygonFillMode() const { return m_polygonFillMode; }
	#endif

		// �Ƿ�Ϊȫ��ģʽ
		virtual bool isFullscreen() const;

		// ��ȡ����
		const RenderCfg& getCfg() const { return m_cfg; }

		virtual bool				isVSync() const;
		virtual ui32				getMaxStageNum() const = 0;
		virtual FrameBuffer*		getFrameBuffer() const;
		virtual RasterizerState*	getDefaultRasterizerState() const;
		virtual DepthStencilState*	getDefaultDepthStencilState() const;
		virtual BlendState*			getDefaultBlendState() const;
		virtual RasterizerState*	getRasterizerState() const;
		virtual DepthStencilState*	getDepthStencilState() const;
		virtual BlendState*			getBlendState() const;
		//virtual SamplerState*		getSamplerState(ui32 stage) const;

		// ���ò���ָ��
		virtual void scissor(ui32 left, ui32 top, ui32 width, ui32 height)=0;

		// ��������
		virtual void endScissor()=0;

		// ִ����Ⱦ
		virtual void render(RenderInput* pInput, ShaderProgram* program)=0;

		virtual void project(Vector3& screenPos, const Vector3& worldPos, const Matrix4& matVP, Viewport* pViewport = NULL);
		virtual void unproject(Vector3& worldPos, const Vector3& screenPos, const Matrix4& matVP, Viewport* pViewport = NULL);
		virtual void getDepthRange(Vector2& vec) = 0;
		virtual void convertMatOrho(Matrix4& mat, const Matrix4& matOrth, Real zn, Real zf) = 0;
		virtual void convertMatProj(Matrix4& mat, const Matrix4& matProj) = 0;

		// �������㻺��
		virtual GPUBuffer* createVertexBuffer(Dword usage, const Buffer& buff) = 0;
		
		// ������������
		virtual GPUBuffer* createIndexBuffer(Dword usage, const Buffer& buff) = 0;

		// ��������
		virtual Texture* createTexture(const String& name)=0;

		// ��������
		virtual Texture* createTexture2D(PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 numMipmaps, const Buffer& buff) = 0;

		// �ͷ�����
		virtual void releaseTexture(Texture* tex);

		// �������ƻ�ȡ����
		Texture* getTexture(const char* name);

		void dirtyTexSlot(){ m_dirtyTexSlot = true; }

		//virtual Texture*			createTextureFromImage(Image* pImage, Dword usage = Texture::TU_DEFAULT);
		virtual ShaderProgram*		createShaderProgram(ShaderProgramRes* material)=0;
		virtual Shader*				createShader(Shader::ShaderType type, const Shader::ShaderDesc& desc, const String& filename) = 0;
		virtual Shader*				createShader(Shader::ShaderType type, const Shader::ShaderDesc& desc, const char* srcBuffer, ui32 size) = 0;

		// ������Ⱦ����
		virtual RenderInput* createRenderInput(ShaderProgram* pProgram) = 0;
		virtual RenderTargetView*	createRenderTargetView(PixelFormat fmt, ui32 width, ui32 height) = 0;
		virtual DepthStencilView*	createDepthStencilView(PixelFormat fmt, ui32 width, ui32 height) = 0;
		virtual RasterizerState*	createRasterizerState(const RasterizerState::RasterizerDesc& desc) = 0;
		virtual DepthStencilState*	createDepthStencilState(const DepthStencilState::DepthStencilDesc& desc) = 0;
		virtual BlendState*			createBlendState(const BlendState::BlendDesc& desc) = 0;
		virtual const SamplerState*	getSamplerState(const SamplerState::SamplerDesc& desc) = 0;

		// ������ȾĿ��
		virtual RenderTarget* createRenderTarget(ui32 _id, ui32 _width, ui32 _height, PixelFormat _pixelFormat, const RenderTarget::Options& option) = 0;

		// �½�Renderable
		Renderable* createRenderable(const String& renderStage, ShaderProgramRes* material);

		// ��ȡRenderable
		Renderable* getRenderable(RenderableID id);

		// ����Renderable
		void destroyRenderables(Renderable** renderables, int num);

		// ����Renderable
		void destroyRenderables(vector<Renderable*>::type& renderables);

		// ��ȡ�豸����
		DeviceFeature&	getDeviceFeatures() { return m_deviceFeature; }

		// ��Ļ��
		virtual ui32 getScreenWidth() = 0;

		// ��Ļ��
		virtual ui32 getScreenHeight() = 0;

		// ��ȡ��ʵ�ӿڴ�С
		virtual void getViewportReal(Viewport& pViewport)=0;

		// ���������ʽ
		PixelFormat	getBackBufferPixelFormat() const { return m_backBufferFormat; }

		// �л���ǰ̨
		bool present();

		// �����Ƿ�ִ��֡����
		void setEnableFrameProfile(bool _enable){ m_isEnableFrameProfile = _enable; }

		// �ж���Ⱦ���Ƿ�����֡����
		bool isEnableFrameProfile() const { return m_isEnableFrameProfile; }

		// ��ȡ֡״̬
		FrameState& getFrameState() { return m_frameState; }

		// ���ó�ʼ����Mipmap����
		void setStartMipmap(ui32 mipmap) { m_startMipmap = mipmap; }

		// ��ȡ
		ui32 getStartMipmap() const { return m_startMipmap; }

		// ����RendererSize
		virtual void onSize(int width, int height) = 0; 

		void destroy();

	protected:
		virtual bool doPresent() = 0;
		virtual bool initializeImpl(const RenderCfg& config) = 0;
		virtual void destroyImpl() = 0;
		virtual void createSystemResource() = 0;
		virtual void cleanSystemResource() = 0;

	protected:
		RenderCfg			m_cfg;								// ����
		bool				m_bVSync;
		FrameState			m_frameState;						// ��һ֡״̬
		FrameBuffer*		m_pFrameBuffer;
		RasterizerState*	m_pDefaultRasterizerState;
		DepthStencilState*	m_pDefaultDepthStencilState;
		BlendState*			m_pDefaultBlendState;
		RasterizerState*	m_pRasterizerState;
		DepthStencilState*	m_pDepthStencilState;
		BlendState*			m_pBlendState;
		bool				m_bSupportsDXT;
		bool				m_bSupportsPVRTC;
		bool				m_bSupportsATITC;
		bool				m_bSupportsDepthTextures;
		bool				m_bSupportsAnisotropy;
		ui32				m_backBufferBlueBits;
		ui32				m_backBufferRedBits;
		ui32				m_backBufferGreenBits;
		ui32				m_backBufferAlphaBits;
		ui32				m_backBufferBits;
		ui32				m_depthBufferBits;
		PixelFormat			m_backBufferFormat;
		ui16				m_bBind2RGBTexture;
		ui16				m_bBind2RGBATexture;
		std::map<String, Texture*>	m_textures;				// ��Ⱦ��ʹ�õ���������
		EE_MUTEX(m_texturesMutex);

		int					m_renderableIdentifier;			// ��Ⱦ��Ԫ����
		std::map<ui32, Renderable*>	m_renderables;			// ����Ⱦ��λ
		ui32				m_startMipmap;					// ���������ʼmipmap���𣬿�����������������
		DeviceFeature		m_deviceFeature;				// �豸������Ϣ
		bool				m_isEnableFrameProfile;			// �Ƿ�ִ��֡����
		bool				m_dirtyTexSlot;
	#ifdef ECHO_EDITOR_MODE
		PolygonFillMode		m_polygonFillMode;				// ��������ģʽ(ȫ������)
	#endif
	};
}
