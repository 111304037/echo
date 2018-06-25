#pragma once

#include "engine/core/render/render/ShaderProgram.h"
#include "engine/core/render/render/RenderState.h"
#include "engine/core/render/TextureRes.h"
#include "engine/core/resource/Res.h"

namespace Echo
{
	class ShaderProgramRes;

	/**
	* ����ʵ��
	*/
	class Material : public Res
	{
		ECHO_RES(Material, Res, ".material", Res::create<Material>, Res::load)

	public:
		// texture info
		struct TextureInfo
		{
			String		m_name;
			int			m_idx = -1;
			String		m_uri;
			TextureRes* m_texture = nullptr;
		};
		typedef map<int, TextureInfo>::type TextureInfoMap;

	public:
		struct Uniform
		{
			String				m_name;				// ����
			ShaderParamType		m_type;				// ����
			i32					m_count;			// ����
			Byte*				m_value = nullptr;	// ֵ

			// destructor
			~Uniform();

			// get value bytes
			ui32 getValueBytes();

			// set value
			void setValue(const void* value);

			// alloc Value
			void allocValue();

			// ��¡
			Uniform* clone();
		};

		typedef map<String, Uniform*>::type ParamMap;

	public:
		Material();
		Material(const ResourcePath& path);
		~Material();

		// release
		void release();

		// ��¡
		void clone(Material* orig);

		// ��Դ�����߳�׼������
		void prepareTexture();

		// ��������
		void loadTexture();

		// ж������
		void unloadTexture();

		// ��ȡ����
		TextureRes* getTexture(const int& index);
		const String& getTexturePath(const int& index);

		// ����Ĭ����Ⱦ������
		void setShader(const ResourcePath& path);
		void setShaderContent(const char* content);
		const ResourcePath& getShader() const { return m_shaderPath; }


		// �׶���غ���
		const StringOption& getRenderStage() { return m_renderStage; }
		void setRenderStage(const StringOption& stage) { m_renderStage.setValue(stage.getValue()); }

		// ���ú궨��
		void setMacros(const String& macros);

		// ��ȡ��Ⱦ����
		ShaderProgramRes* getMaterial() { return m_shaderProgramRes; }

		// operate uniform
		bool isUniformExist(const String& name);
		void setUniformValue(const String& name, const ShaderParamType& type, void* value);
		Uniform* getUniform(const String& name);

		// ��ȡ��������(������ȫ������)
		int getTextureNum() { return static_cast<int>(m_textures.size()); }

		// ������ͼ
		TextureRes* setTexture(const String& name, const String& uri);
		TextureRes* setTexture(const String& name, TextureRes* textureRes);

		// ��ȡ���Զ���
		ParamMap& GetUniformSet() { return m_uniforms; }

		// get uniform value
		void* getUniformValue(const String& name);

		// �Ƿ�ʹ���˺궨��
		bool isMacroUsed(const String& macro);

		// ���ú궨��
		void setMacro(const String& macro, bool enabled);

		// ������Ⱦ����
		void buildShaderProgram();

		// on loaded
		virtual void onLoaded();

	protected:
		// get property value
		virtual bool getPropertyValue(const String& propertyName, Variant& oVar);

		// set property value
		virtual bool setPropertyValue(const String& propertyName, const Variant& propertyValue);

	private:
		// �����ͼ�ļ���
		void addTexture(int idx, const String& name);

		// ����ƥ��
		void matchUniforms();

		// ׼����ԴIO
		TextureRes* prepareTextureImp(const String& texName);

		// is global uniform
		bool isGlobalUniform( const String& name);

	private:
		ResourcePath		m_shaderPath;				// shader res path
		const char*			m_shaderContent;	// �ٷ�����
		StringOption		m_renderStage;				// ������Ⱦ�׶�
		StringArray			m_macros;					// �궨��
		ShaderProgramRes*	m_shaderProgramRes;			// ��Ӧ��ɫ��
		ParamMap			m_uniforms;
		TextureInfoMap 		m_textures;
	};
	typedef ResRef<Material> MaterialPtr;
}