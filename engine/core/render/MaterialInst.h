#pragma once

#include "engine/core/render/render/ShaderProgram.h"
#include "engine/core/render/render/RenderState.h"
#include <engine/core/render/render/RenderQueue.h>
#include "engine/core/render/TextureRes.h"
#include "engine/core/Util/Singleton.h"

namespace Echo
{
	struct PBRLight;
	class Material;
	class MaterialController;

	/**
	* ����ʵ��
	*/
	class MaterialInst
	{
		friend class Model;
		friend class MaterialManager;
		friend class MaterialController;
		typedef vector<String>::type StringVec;
		typedef vector<String>::type::iterator StringVecItor;
		typedef map<int, TextureRes*>::type TextureMap;
		typedef map<int, TextureRes*>::type::iterator TextureMapItor;
		typedef map<int, String>::type TextureNameMap;
		typedef map<int, String>::type::iterator TextureNameMapItor;

	public:
		struct uniform
		{
			String				name;		// ����
			ShaderParamType		type;		// ����
			i32					count;		// ����
			void*				value;		// ֵ

			// ��¡
			uniform* clone();
		};

		typedef map<String, uniform* >::type ParamMap;

		typedef map<String, MaterialController*>::type MaterialControllerMap;
		typedef MaterialControllerMap::iterator MaterialControllerItor;

	public:
		MaterialInst();
		~MaterialInst();

		// ���� --> �����첽��
		bool loadByFile(const String& name, const String& macros);

		// Ӧ�����ݵ��ڴ� && GPU --> ���������̡߳�
		bool applyLoadedData();

		// ����
		void saveToFile(const String& name);

		// ��¡
		void cloneFromTemplate(MaterialInst* _template);

		// �����̳�
		void deriveUniforms( MaterialInst* from);

		// ��Դ�����߳�׼������
		void prepareTexture();

		// ��������
		void loadTexture();

		// ж������
		void unloadTexture();

		// ��ȡ����
		TextureRes* getTexture(const int& index);

		// ���º���
		void update(ui32 delta);

		// ��ȡ����ʵ����
		const String& getName() const { return m_name; }

		// ���ò���ʵ��ģ����
		void setName(const String& name) { m_name = name; }

		// ����Ĭ����Ⱦ������
		void setMaterialTemplate(const String& name) { m_materialTemplate = name; }

		// �׶���غ���
		const String& getStage() { return m_stage; }
		void setStage(const String& stage) { m_stage = stage; }

		// ���ú궨��
		void setMacros(const String& macros);

		// ��ȡĬ����Ⱦ������
		const String& getMaterialTemplate() const { return m_materialTemplate; }

		// ��ȡ��Ⱦ����
		RenderQueue* getRenderQueue() { return m_renderQueue; }

		// ���uniform����
		void AddUniformParam(uniform* param);

		// �жϱ����Ƿ����
		bool isUniformExist(const String& name);

		// �޸�uinifrom����
		void ModifyUniformParam(const String& name, const ShaderParamType& type, void* value);

		// ��ȡuniform������ֵ
		void* GetuniformValue(const String& name, ShaderParamType type);

		// ��ȡuniform����
		uniform* GetUniform(const String& name);

		// ��ȡ��������(������ȫ������)
		int getTextureNum() { return static_cast<int>(m_TexturesName.size()); }

		// �����ͼ�ļ���
		void AddTextureName(int idex, const String& name);

		// ������ͼ
		TextureRes* SetTexture(int idex, const String& name);

		// ͨ��������ȡ��ͼ����
		const String& GetTextureName(int index) { return m_TexturesName[index]; }

		// ��ȡ���Զ���
		ParamMap& GetUniformSet() { return m_unifromParamSet; }

		// ��Ӳ���������
		MaterialController* AddController(const String& name, ui32 type);

		// ʹ��ģ����Ӳ���������
		MaterialController* AddControllerFromTemplate(const String& name, MaterialController* controllerTemplate);

		// ɾ������������
		void DelController(const String& name);

		void LoadBlendState(void* pNode);
		void LoadRasterizerState(void* pNode);
		void LoadDepthStencilState(void* pNode);

		// ��ȡ������
		MaterialController* GetController(const String& name);

		ShaderParamType S2ShaderParamType(const String& value);
		String			ShaderParamType2S(const ShaderParamType& type) const;
		static void		delVoid2Value(const ShaderParamType& type, void* value, const int count = 1);
		static void*	createValue2Void(const ShaderParamType& type, const int count = 1);
		static void*	cloneVoid2Value(const ShaderParamType& type, void* value, const int count = 1);
		static void		CopyUniformValue(void* dstValue, const ShaderParamType& type, void* srcValue);

		// �Ƿ���ȫ�ֱ���
		static bool isGlobalUniform(const String& name);

		void*	getUniformValue(const String& name);
		void	modifyUniformValue(const String& name, void* value);
		void	void2s(uniform* param, String& value);
		void	S2Void(const ShaderParamType& type, const String& value, void* dstValue, const int count = 1);

		// �Ƿ�ʹ���˺궨��
		bool isMacroUsed(const String& macro);

		// �Ƿ��ύ������Ⱦ����
		bool isSubmitToStageRenderQueue() { return m_isSubmitToStageRenderQueue; }

		// �����Ƿ��ύ������Ⱦ����
		void setSubmitToStageRenderQueue(bool isSubmitToStageRenderQueue) { m_isSubmitToStageRenderQueue = isSubmitToStageRenderQueue; }

#ifdef ECHO_EDITOR_MODE
		// ������Ⱦ״̬
		void setBlendState(BlendState* state) { m_blendState = state; }
		void setRasterizerState(RasterizerState* state) { m_rasterizerState = state; }
		void setDepthStencil(DepthStencilState* state) { m_depthStencil = state; }

		void setPBRLight(const vector<PBRLight*>::type& lights);

		bool isUsingSceneEnvMap() const;

		// ���ú궨��
		void setMacro(const String& macro, bool enabled);

		void refresh();
#endif

		// ��ȡ��Ⱦ״̬
		BlendState* getBlendState() { return m_blendState; }
		RasterizerState* getRasterizerState() { return m_rasterizerState; }
		DepthStencilState* getDepthStencilState() { return m_depthStencil; }

		const BlendState::BlendDesc& getBlendDesc() { return m_blendDesc; }
		const RasterizerState::RasterizerDesc& getRasterizerStateDesc() { return m_rasterizerStateDesc; }
		const DepthStencilState::DepthStencilDesc& getDepthStencilDesc() { return m_depthStencilDesc; }

		// ������Ⱦ����
		void buildRenderQueue();

	private:	
		// ����ƥ��
		void matchUniforms();

		// ׼����ԴIO
		TextureRes* prepareTextureImp(const String& texName);

	private:
		String				m_name;					// ����ʵ������
		String				m_materialTemplate;		// ��ʹ�õĲ���ģ��
		String				m_stage;				// ������Ⱦ�׶�
		StringArray			m_macros;				// �궨��
		StringArray			m_macrosEx;				// �ⲿ�궨��
		RenderQueue*		m_renderQueue;			// ��Ⱦ����
		ParamMap			m_unifromParamSet;
		ParamMap			m_unifromParamSetFromFile;
		TextureMap			m_textures;
		TextureNameMap 		m_TexturesName;
		int					m_TextureCount;
		MaterialControllerMap m_ControllerList;   //�����������б�

		bool					 m_isHaveCustomBlend;
		BlendState*				 m_blendState;
		BlendState::BlendDesc	 m_blendDesc;

		bool							m_isHaveCustomRasterizer;
		RasterizerState*				m_rasterizerState;
		RasterizerState::RasterizerDesc m_rasterizerStateDesc;

		bool								m_isHaveCustomDepthStencil;
		DepthStencilState*					m_depthStencil;
		DepthStencilState::DepthStencilDesc m_depthStencilDesc;

		bool				m_isTemplate;
		bool				m_isSubmitToStageRenderQueue;
	};

	/**
	* ����ʵ��������
	*/
	class MaterialManager
	{
		typedef map<String, MaterialInst* >::type MaterialIstTemplateMap;
		typedef map<String, MaterialInst* >::type::iterator MaterialIstTemplateItor;
		typedef set<MaterialInst* >::type MaterialIstSet;
		typedef set<MaterialInst* >::type::iterator MaterialIstItor;
		typedef list<Material* >::type MaterialList;

		__DeclareSingleton(MaterialManager);

	public:
		MaterialManager();
		~MaterialManager();

		MaterialInst* createMaterialIst(const String& materialName, const String& macros);
		void   destroyMaterialIst(MaterialInst* material);

		// ɾ�����в���ʵ��
		void  destroyAllMaterialIst();

		bool AddMaterialIstTemplate(const String& materialName);

		// ��ȡ����ģ��
		MaterialInst* getMaterialTemplate(const String& materialName, const String& macros);

		// ɾ������ģ��
		bool DelMaterialIstTemplate(const String& materialName);

		// ɾ�����в���ģ��
		void DelAllMaterIstialTemplate();

		// ��������ز���
		void GetControllerTypes(StringArray& stringArry);
		ui32 GetControllerType(const String& typeName);
		String GetControllerTypeString(ui32 type);

		void setIsEditorModel(bool b) { m_isEditModel = b; }

		Material* createMaterial();
		void destroyMaterial(Material* mtrl);

	private:
		MaterialIstTemplateMap	m_MaterialIstTemplates;
		std::mutex				m_materialIstTemplatesMutex;
		MaterialIstSet			m_MaterialIsts;
		std::mutex				m_materialIstsMutex;
		bool					m_isEditModel;
		MaterialList			m_materialList;
	};
}