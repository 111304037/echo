#pragma once

#include "engine/core/render/render/ShaderProgram.h"
#include "engine/core/render/render/RenderState.h"
#include "engine/core/render/TextureRes.h"

namespace Echo
{
	class ShaderProgramRes;

	/**
	* ����ʵ��
	*/
	class Material
	{
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
			i32					m_count;				// ����
			Byte*				m_value = nullptr;	// ֵ

			// destructor
			~Uniform();

			// get value bytes
			i32 getValueBytes();

			// set value
			void setValue(const void* value);

			// alloc Value
			void allocValue();

			// ��¡
			Uniform* clone();
		};

		typedef map<String, Uniform* >::type ParamMap;

	public:
		// create a material instance
		static Material* create();

		// release
		void release();

		// Ӧ�����ݵ��ڴ� && GPU --> ���������̡߳�
		bool applyLoadedData();

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

		// ��ȡ����ʵ����
		const String& getName() const { return m_name; }

		// ���ò���ʵ��ģ����
		void setName(const String& name) { m_name = name; }

		// ����Ĭ����Ⱦ������
		void setMaterialTemplate(const String& name) { m_materialTemplate = name; }
		const String& getMaterialTemplate() const { return m_materialTemplate; }

		// ����ʹ�ùٷ�����
		void setOfficialMaterialContent(const char* content) { m_officialMaterialContent = content; }

		// �׶���غ���
		const String& getRenderStage() { return m_renderStage; }
		void setRenderStage(const String& stage) { m_renderStage = stage; }

		// ���ú궨��
		void setMacros(const String& macros);

		// ��ȡ��Ⱦ����
		ShaderProgramRes* getMaterial() { return m_material; }

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
		ParamMap& GetUniformSet() { return m_unifroms; }

		// get uniform value
		void* getUniformValue(const String& name);

		// �Ƿ�ʹ���˺궨��
		bool isMacroUsed(const String& macro);

		// ���ú궨��
		void setMacro(const String& macro, bool enabled);

		// ������Ⱦ����
		void buildRenderQueue();

	private:
		// �����ͼ�ļ���
		void addTexture(int idx, const String& name);

	private:
		Material();
		~Material();

		// ����ƥ��
		void matchUniforms();

		// ׼����ԴIO
		TextureRes* prepareTextureImp(const String& texName);

	private:
		String				m_name;						// ����ʵ������
		String				m_materialTemplate;			// ��ʹ�õĲ���ģ��
		const char*			m_officialMaterialContent;	// �ٷ�����
		String				m_renderStage;				// ������Ⱦ�׶�
		StringArray			m_macros;					// �궨��
		ShaderProgramRes*			m_material;					// ��Ӧ����
		ParamMap			m_unifroms;
		TextureInfoMap 		m_textures;
	};
}