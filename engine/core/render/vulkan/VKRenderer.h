#pragma once

#include "Render/Renderer.h"
#include "GLES2RenderState.h"

namespace LORD
{
	// �������Ϣ
	struct TextureSlotInfo
	{
		GLenum						m_target;
		const Texture*				m_texture;
		const GLES2SamplerState*	m_samplerState;	// ����״̬
		

		// ���캯��
		TextureSlotInfo()
			: m_target(-1), m_texture(NULL), m_samplerState( NULL)
		{}

		// ����
		void reset()
		{
			m_target = -1;
			m_texture= NULL;
			m_samplerState = NULL;
		}
	};

	class GLES2Texture;
	class GLES2ShaderProgram;

	/**
	 * OpenGLES2.0��Ⱦ��
	 */
	class LORD_GLES2RENDER_API GLES2Renderer: public Renderer
	{
		typedef vector<GLuint>::type			TexUintList;
		typedef vector<SamplerState*>::type		SamplerList;
		typedef array<bool, 9>					NineBoolArray;

	public:
		GLES2Renderer();
		~GLES2Renderer();
	
		void checkOpenGLExtensions();

		void setViewport(Viewport* pViewport);

		uint getMaxStageNum() const;

		void scissor(uint left, uint top, uint width, uint height);
		void endScissor();

		// �����������������
		void setTexture(uint index, const TextureSampler& sampler);

		// ִ����Ⱦ
		void render(RenderInput* pInput, ShaderProgram* shaderProgram);

		void getDepthRange(Vector2& vec);
		void convertMatOrho(Matrix4& mat, const Matrix4& matOrth, Real zn, Real zf);
		void convertMatProj(Matrix4& mat, const Matrix4& matProj);

		void enableAttribLocation(ui32 attribLocation);
		void disableAttribLocation(ui32 attribLocation);

		GPUBuffer*				createVertexBuffer(Dword usage, const Buffer& buff);
		GPUBuffer*				createIndexBuffer(Dword usage, const Buffer& buff);

		// ��������
		Texture*  createTexture(const String& name);
		Texture*				createTexture2D(PixelFormat pixFmt, Dword usage, uint width, uint height, uint numMipmaps, const Buffer& buff);
		ShaderProgram*			createShaderProgram(  Material* material);
		Shader*					createShader(Shader::ShaderType type, const Shader::ShaderDesc& desc, const String& filename);
		Shader*					createShader(Shader::ShaderType type, const Shader::ShaderDesc& desc, const Buffer& buff);
		RenderInput*			createRenderInput( ShaderProgram* pProgram);
		RenderTargetView*		createRenderTargetView(PixelFormat fmt, uint width, uint height);
		DepthStencilView*		createDepthStencilView(PixelFormat fmt, uint width, uint height);
		RasterizerState*		createRasterizerState(const RasterizerState::RasterizerDesc& desc);
		DepthStencilState*		createDepthStencilState(const DepthStencilState::DepthStencilDesc& desc);
		BlendState*				createBlendState(const BlendState::BlendDesc& desc);
		const SamplerState*		getSamplerState(const SamplerState::SamplerDesc& desc);
	
		// ������ȾĿ��
		virtual RenderTarget* createRenderTarget(ui32 _id, ui32 _width, ui32 _height, PixelFormat _pixelFormat, bool _hasDepth /* = false */, bool _msaa /* = false */, bool _multi_resolution, bool _is_cubemap);

		virtual ui32 getScreenWidth() { return m_screenWidth; }
		virtual ui32 getScreenHeight() { return m_screenHeight; }

		bool bindShaderProgram(GLES2ShaderProgram* program);

		// ��ȡ��ʵ�ӿڴ�С
		virtual void getViewportReal( Viewport& pViewport);
		
		// ����Size
		virtual void onSize(int width, int height); 

		virtual void onRenderThreadStart();

	protected:
		// ��Ⱦ����Ļ
		virtual bool doPresent();

		// ����ָ��������
		void bindTexture(GLenum slot, GLenum target, GLES2Texture* texture, const GLES2SamplerState* samplerState, const GLES2SamplerState* pPreSamplerState = NULL);

		bool initializeImpl(const RenderCfg& config);
		void destroyImpl();
		virtual void createSystemResource();
		virtual void cleanSystemResource();

	protected:
		GLES2ShaderProgram*			m_pre_shader_program;
		array<TextureSlotInfo, 8>	m_preTextures;		 // ����������	

	private:
		String				m_gpuDesc;
		ui32				m_screenWidth;
		ui32				m_screenHeight;
		std::set<GLES2SamplerState*> m_vecSamlerStates;
		NineBoolArray		m_isVertexAttribArrayEnable;	// ��¼ָ��AttribArray�Ƿ����

#ifdef LORD_PLATFORM_WINDOWS

	public:
		void* getDisplay() const { return m_eglDisplay; }

		void* getSurface() const { return m_eglSurface; }

		bool createRenderContext( const RenderCfg& config );

		void destroyRenderContext();

		bool makeContextCurrent();

		typedef void (GL_APIENTRY * PFNGLDISCARDFRAMEBUFFEREXT)(GLenum target, GLsizei numAttachments, const GLenum *attachments);
		PFNGLDISCARDFRAMEBUFFEREXT			glDiscardFramebufferEXT;

	protected:
		EGLConfig	m_eglConfig;
		EGLDisplay	m_eglDisplay;
		EGLContext	m_eglContext;
		EGLSurface	m_eglSurface;
		EGLint		m_iConfig;
		HDC			m_hDC;
		HWND		m_hWnd;
#endif

	};
}
