#pragma once

#include <engine/core/Util/Array.hpp>
#include "engine/core/Util/PtrMonitor.h"
#include <engine/modules/Light/LightArray.h>
#include <engine/core/Resource/EchoThread.h>
#include <engine/core/main/EngineSettings.h>
#include "engine/core/render/mesh/Mesh.h"

namespace Echo
{
	/**
	 * ģ����Դ
	 */
	class Camera;
	class ModelManager;
	class MaterialInst;
	class RenderInput;
	class Renderable;
	class TextureRes;
	class Texture;
	class Skeleton;
	class AnimBlender;
	struct TextureSampler;
	class Model : public PtrMonitor<Model>
	{
		friend class ModelManager;
		friend class ModelLoadEvent; 
		friend class ModelUnLoadEvent; 

	public: 
		static void ReleaseMesh(Mesh* p); 
		static void DeleteLightArray(LightArray* p); 

	public:
		// ����������
		enum GeometryType
		{
			GT_StaticMesh,		// ��̬ģ��
			GT_Skin,			// ����ģ��
			GT_Terrain,			// ����
		};

		// ��Ⱦ�׶�ö��
		enum RenderPhaseType
		{
			RP_Normal0 = 0,
			RP_Normal1,
			RP_Normal2,
			RP_Normal3,
			RP_ShadowMap,		// �ٷ�
			RP_Total,
		};

		enum DynamicMtlIst
		{
			RECEIVE_SHADOW = 1,
			DOUBLE_LMP = 2,
		};

		// ��̬�決����
		struct LightmassConfig
		{
			std::string		m_textureDiffuse;		// ��������ͼ
			std::string		m_textureNormal;		// ������ͼ
			std::string		m_textureEmissive;		// �Է�����ͼ
			std::string		m_textureSpecular;		// �߹���ͼ
		};
		typedef vector<LightmassConfig>::type LightmassConfigArray;

		// ģ����Ϣ
		struct Info
		{
			String					m_name;				// ģ������
			vector<String>::type	m_originNames;		// inst Model����ԭmodel����
			bool					m_isCastShadow;		// �Ƿ�Ͷ����Ӱ
			bool					m_isReceiveShadow;	// �Ƿ������Ӱ
			bool					m_isActor;			// �Ƿ���Actor
			bool					m_isMainActor;		// �Ƿ���MainActor
			bool					m_isForUI;
			GeometryType			m_geometryType;		// ����������
			String					m_meshName;			// mesh��
			array<String, RP_Total>	m_materialInsts;	// ����ʵ��(ģ�Ϳ����ж�����ʣ�������˿����ж���·�)
			String					m_materialInstsAll;	// ���п�ʹ�õĲ���ʵ��
			String					m_lightArray;		// ��Դ����
			LightmassConfigArray	m_lightmassConfig;	// �決����

			// ���캯��
			Info() 
			: m_geometryType(GT_StaticMesh)
			, m_isCastShadow(false)
			, m_isReceiveShadow(false)
			, m_isActor(false)
			, m_isMainActor(false)
			, m_isForUI(false)
			{}

			// �Ƿ�Ϊ��Ƥ������
			bool isSkinModel() const { return m_geometryType == GT_Skin ? true : false; }
		};

		// ��Ⱦ�׶�
		struct RenderPhase
		{
			String							m_tag;									// �׶α�ʶ;
			MaterialInst*					m_materialInst;							// ����ʵ��
			RenderInput*					m_renderInput;							// ����������
			Renderable*						m_renderable;							// ����Ⱦ����

			RenderPhase(){}
			~RenderPhase();

			// ����
			void clean();

			// ����
			void update( ui32 delta);
		};
		typedef array<RenderPhase*, RP_Total> RenderPhaseArray;
		typedef map<String, RenderPhase*>::type RenderPhaseMap;
		typedef vector<PBRLight*>::type PBRLights;

	public:
		// for async: Load Model Config From Xml.
		void loadFromFile(); 

		// for async: Apply Model Config.
		void applyLoadedData(); 

		//ж�� --> must same as load thread.
		void unload();

		// Model���¼���,��ʱҲ�������¼���Mesh ���� ������;
		void reload();

		// ������Ⱦ��Ԫ
		void createRenderable( bool isForUI=false);

		// ������Ⱦ��Ԫ
		void createRenderable(RenderPhase& phase, bool isForUI = false);

		// ����
		void update(bool isForUI = false, ui32 delta = 0);

		// �ύ����Ⱦ����
		void submitToRenderQueue();

		// �ύ����Ӱ��Ⱦ����
		void submitToSMRenderQueue();

		//�Ƿ������
		bool isSkinModel() const { return m_info.isSkinModel(); }

		// ��ȡmesh��Χ��
		Box getLocalMeshAABB();

		//��ȡ���ذ�Χ��
		Box getLocalAABB() const;

		// ��ȡ�����Χ��
		const Box& getWorldAABB() const { return m_worldBox; }

		//��ȡģ����Ϣ
		const Model::Info& getModelInfo() const { return m_info; }

		// ��ȡģ����Ϣ(ָ��)
		Model::Info* getModelInfoPtr() { return &m_info; }

		//����ģ����Ϣ
		void setInfo(const Model::Info& info) { m_info = info; }

		// ��ȡ����ʵ��
		MaterialInst* getMaterialInstance(RenderPhaseType type) { return m_phases[type]->m_materialInst; }

		// ���ò���ʵ��
		void setMaterialInstance(RenderPhaseType type, MaterialInst* mate);

		// ����Mesh
		void setMesh(Mesh* mesh);

		// ����ԭģ������û��Ϊ��
		void setOriginNames(vector<String>::type& originNames){ m_info.m_originNames = originNames;}

		// ���ù�����
		void setLightArray(LightArray* lightArray); 

		// ��ȡmesh
		Mesh* getMesh() { return m_mesh; }

		// ��ȡģ�ͽ׶�
		RenderPhase* getPhase(RenderPhaseType type) { return m_phases[type]; }

		// ��������
		void setTexture(RenderPhaseType type, i32 index, const String& name);

		//���ù���
		void  setSkeleton(Skeleton* pSkeleton);

		// ���ö������
		void  setAnimBlender(AnimBlender* pAnimation);

		// �ɼ�������
		void  setVisible(bool visibel) { m_IsVisible = visibel; }

		// �Ƿ�ɼ�
		bool  isVisible() { return m_IsVisible; }

		// ��ȡȫ�ֱ���ֵ
		void* getGlobalUniformValue(const String& name, i32 subMesh, bool isUIModel = false);
		
		// ����ļ���
		void* extraCalcUniform(const String& name, void* value, i32 subMeshId);

		bool modifyUniformValue( const String& name, void* value, RenderPhaseType phase = RP_Normal0 );
		void modifySubMeshUniformValue(i32 subId, const String &name, void* value, RenderPhaseType phase = RP_Normal0);

		bool saveMesh( bool useNornalMap = true );

		// ��ȡ������
		LightArray* getLightArray() { return m_lightArray.get(); }
		
		// ��ȡ�Ƿ����.
		bool isEnable() const { return m_isEnable; }

		// ���ü������״̬.
		void setEnable(bool isEnable) { m_isEnable = isEnable; }

		// ��ȡ�決����
		LightmassConfigArray& getLightmassConfig() { return m_info.m_lightmassConfig; }

		void updateCameraPos();

		bool isSkyBox(){ return m_isNeedUpdateMatSky; }

		void execute_render();

	public:
		void updateRenderablesPosition();

		void resetRenderInput();

	protected:
		// ��ȡ
		Texture* getGlobalTexture(const String& name, const int subMesh = 0);
		const TextureSampler* getGlobalTextureSample(const String& name, const int subMesh = 0);

		// ���ز���ʵ��
		void loadMaterialInstanceFromFile();

		// ����ģ��
		void loadMeshFromFile();

		// ���ع�����
		void loadLightArrayFromFile();

		// 
		void applyLoadedMaterialInstanceData();

		//
		void applyLoadedMeshData();

		//
		void applyLoadedLightArrayData();

	public:
		Model(const Model::Info& modelInfo);
		~Model();

	private:
		// ������ģ���ù�����������
		void refreshSubMeshBoneMatRows();

		// ����Ͷ����Ӱ��Ⱦ����
		void createRenderabeSM(bool isForUI);

		// ��������ʵ��
		MaterialInst* createMaterialInst(const String& materialName);

		// Ӧ�ò�������
		void applyLoadedMaterialInstanceData( RenderPhase& phase);

	private:
		Info						m_info;				// ģ����Ϣ(���ش洢)
		RenderPhaseArray			m_phases;			// ��Ⱦ�׶�
		Mesh*						m_mesh;				// ����ģ��

		typedef std::unique_ptr<LightArray, decltype(&DeleteLightArray)> LightArrayPtr; 
		LightArrayPtr				m_lightArray;		// ������
		PBRLights					m_pbrlights;
		bool						m_IsVisible;        // �Ƿ�ɼ�

		//�������������
		Skeleton*					m_pSkeleton;
		AnimBlender*				m_pAnimBlender;
		ui32						m_boneCount;
		Vector4*					m_boneMatRows;			// �Ǽܶ�Ӧ�����������
		vector<Vector4*>::type		m_subMeshBoneMatRows;	// ��ģ�Ͷ�Ӧ��������

		//ȫ������
		Matrix4						m_matWV;
		Matrix4						m_matWVP;				// ����۲�ͶӰ����
		Matrix4						m_matWVPSM;				// ��Ӱ��ͼ��Ӧ����۲�ͶӰ����
		Matrix4						m_matWSM;
		Real						m_alpha;				// alphaֵ
		bool						m_isNeedUpdateMatWater;	// �Ƿ����ˮ����
		Matrix4						m_matWVPWater;		    // ˮ�淴������۲�ͶӰ����
		bool						m_isNeedUpdateMatSky;	// �Ƿ������պо���
		Matrix4						m_matWVPSky;			// ��պ�ʹ������۲�ͶӰ����
		float						m_currentTime;			// ��ǰʱ��(����)
		Box							m_worldBox;				// �����Χ��
		bool						m_isEnable;				// �Ƿ������ɡ�
		bool						m_isInShowdownBox;  // �Ƿ���Ͷ����Ӱ��Χ����
		ui32						m_dymOffset;
	};

	/**
	 * ģ�͹�����
	 **/
	class ModelManager
	{
		friend class ModelTemplateLoadEvent; 
		friend class ModelUnLoadEvent; 
		friend class ModelLoadEvent;

		typedef map<String, Model::Info* >::type Model_InfoMap;
		typedef map<String, Model::Info* >::type::iterator Model_InfoItor;
		typedef set<Model* >::type ModelSet;
		typedef set<Model* >::type::iterator ModelItor;

	public:
		~ModelManager();

		// instance
		static ModelManager* instance();

		// ����ģ��
		void createModel(const String& modelName, bool isSync, std::function<bool(Model* )> onCreateModelComplete, bool isForUI = false);

		// ����ģ��
		void destroyModel(Model* model, bool isSync = false);

		// ɾ������ģ��
		void destroyAllModels();

		// ���ģ��ģ��
		bool addModelTemplate(const String& modelName);

		// ɾ��ģ��ģ��
		bool delModelTemplate(const String& modelName);

		// ��ȡģ��ģ��
		Model::Info* getModelTemplate(const String& modelName);

		// ɾ������ģ��ģ��
		void delAllModelTemplate();

		// ��ȡͶӰ��Ӱģ�Ͱ�Χ��
		void buildBoxSM(Box& oBox, const Vector3& mainPos, Camera* cam, float shadowDistance = 10.f);

		// ����ģ���ļ�
		bool loadModelByFile( const String& modelName, Model::Info* modelInfo);

		// ����ģ���ļ�
		void saveModelToFile(const String& modelName ,const Model::Info& modelInfo);

		bool refreshModelTemplate(const String& modelName);

	private:
		ModelManager();

		// ���ģ��
		void addModel(Model* model);

		// ׼��ģ��
		Model::Info* prepareModelTemplate(const String& modelName);

	private:
		ModelSet		m_Models;
		ModelSet		m_modelsCastShadow;			// Ͷ����Ӱģ��
		Model_InfoMap	m_ModelInfoMap;

		float			m_criticalValue;
	};

	class ModelLoadEvent : public StreamThread::Task
	{
	public:
		ModelLoadEvent(Model* model, std::function<bool(Model*)> onModelLoadComplete);
		virtual ~ModelLoadEvent();

		// ʧ�ܺ��Ƿ��ظ�֪ͨ
		virtual bool isRepeatNotifyIfFail()	{ return true; }

		// �Ƿ��Զ�����
		virtual bool isAutoDestroy() { return true; }

		// ����
		virtual bool process();

		// ���
		virtual bool finished();

		// ���ظ��¼������ȼ���
		virtual StreamThread::TaskLevel GetLevel() { return StreamThread::TL_Normal; }

	private:
		Model*	m_model;
		ui32	m_modelId;

		std::function<bool(Model*)> m_onModelLoadComplete; 
	};

	class ModelUnLoadEvent : public StreamThread::Task
	{
	public:
		ModelUnLoadEvent(Model* model, std::function<void(Model*)> onDestroyComplete);
		virtual ~ModelUnLoadEvent();

		// ʧ�ܺ��Ƿ��ظ�֪ͨ
		virtual bool isRepeatNotifyIfFail()	{ return true; }

		// �Ƿ��Զ�����
		virtual bool isAutoDestroy() { return true; }

		// ����
		virtual bool process();

		// ���
		virtual bool finished();

		// ���ظ��¼������ȼ���
		virtual StreamThread::TaskLevel GetLevel() { return StreamThread::TL_Normal; }

	private:
		Model* m_model;

		std::function<void(Model*)> m_onDestroyComplete;
	};
}