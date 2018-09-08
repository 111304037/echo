#pragma once

#include "engine/core/render/interface/ShaderProgram.h"
#include "engine/core/render/interface/RenderState.h"
#include "engine/core/resource/Res.h"

namespace Echo
{
	class ShaderProgramRes;
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
			Texture*	m_texture = nullptr;
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

		// ��������
		void loadTexture();

		// ж������
		void unloadTexture();

		// ��ȡ����
		Texture* getTexture(const int& index);
		const String& getTexturePath(const int& index);

		// ����Ĭ����Ⱦ������
		void setShaderPath(const ResourcePath& path);
		void setShaderContent(const String& virtualPath, const char* content);
		const ResourcePath& getShaderPath() const { return m_shaderPath; }

		// �׶���غ���
		const StringOption& getRenderStage() { return m_renderStage; }
		void setRenderStage(const StringOption& stage) { m_renderStage.setValue(stage.getValue()); }

		// ���ú궨��
		void setMacros(const String& macros);

		// ��ȡ��Ⱦ����
		ShaderProgramRes* getShader();

		// operate uniform
		bool isUniformExist(const String& name);
		void setUniformValue(const String& name, const ShaderParamType& type, const void* value);
		Uniform* getUniform(const String& name);

		// ��ȡ��������(������ȫ������)
		int getTextureNum() { return static_cast<int>(m_textures.size()); }

		// ������ͼ
		Texture* setTexture(const String& name, const String& uri);
		Texture* setTexture(const String& name, Texture* texture);

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

	protected:
		// propertys (script property or dynamic property)
		virtual const PropertyInfos& getPropertys();

		// get property value
		virtual bool getPropertyValue(const String& propertyName, Variant& oVar);

		// set property value
		virtual bool setPropertyValue(const String& propertyName, const Variant& propertyValue);

	private:
		// �����ͼ�ļ���
		void addTexture(int idx, const String& name);

		// ����ƥ��
		void matchUniforms();

		// is global uniform
		bool isGlobalUniform( const String& name);

	private:
		bool				m_isDirty;					// need rebuild
		ResourcePath		m_shaderPath;				// shader res path
		String				m_shaderContentVirtualPath;	// virtual path of shader content
		const char*			m_shaderContent;			// �ٷ�����
		StringOption		m_renderStage;				// ������Ⱦ�׶�
		StringArray			m_macros;					// �궨��
		ShaderProgramRes*	m_shaderProgramRes;			// ��Ӧ��ɫ��
		ParamMap			m_uniforms;
		TextureInfoMap 		m_textures;
	};
	typedef ResRef<Material> MaterialPtr;
}