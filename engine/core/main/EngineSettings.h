#pragma once

#include <engine/core/Base/EchoDef.h>
#include <engine/core/Memory/MemManager.h>

namespace Echo
{
	// ��ЧƷ��
	enum EffectQuality
	{
		EQ_Low = 1,
		EQ_High,
		EQ_Auto,
	};

	enum WaterQuality
	{
		WQ_Low = 1,
		WQ_Mid,
		WQ_High,
	};

	/**
	 * EngineSettingsMgr 2014-12-5
	 * �������ù�����,��������������ؽӿڱ����ɴ�����ɡ�
	 */
	class EngineSettingsMgr
	{
		friend class Root;
	public:
		EngineSettingsMgr();
		~EngineSettingsMgr();

		// ��ȡʵ��
		static EngineSettingsMgr* instance();

		// Ӧ������
		void Apply(const String& fileName);

		// ������Ч�Ƿ�֧�ֿռ�Ť��
		void setEnableDistortion(bool isDistortion) { m_bEnableDistortion = isDistortion; }

		// �Ƿ�ռ�Ť��
		bool isEnableDistortion() const { return m_bEnableDistortion; }

		bool isEnableLensFlare() const { return false; }

		bool isEnableDownsampleDepth() const { return false; }

		bool isEnableToneMapping() const { return m_bEnableToneMapping; }
		void setEnableToneMapping(bool val) { m_bEnableToneMapping = val; }

		bool isEnableFXAA() const { return m_bEnableFXAA; }
		void setEnableFXAA(bool val) { m_bEnableFXAA = val; }

		bool isEnableFocusBlur() const { return m_bEnableFocusBlur; }
		void setEnableFocusBlur(bool val) { m_bEnableFocusBlur = val; }

		bool isEnableMainActorShadow() const { return m_bEnableMainActorShadow; }
		void setEnableMainActorShadow(bool val) { m_bEnableMainActorShadow = val; }
		
		// �Ƿ���ʾ�ڵ��޳���(�༭��ģʽ����)
		bool isShowOcclusionOccluder() { return m_isShowOcclusionOccluder; }

		// �����Ƿ���Ⱦ����
		void setEnablePhysicsRender(bool physicsRender);

		// �Ƿ���Ⱦ����
		bool isEnablePhysicsRender() { return m_bPhysicsRender; }

		// �����Ƿ�ʹ��������
		void setEnableStreamThread(bool isUseStreamThread);

		void setEnableSmallObjectCull(bool enable);
		bool isEnableSmallObjectCull() { return m_bEnableSmallObjectCull; }

		// �Ƿ�ʹ��VR��ʽ��Ⱦ
		bool isInitVRMode(){ return m_bInitVrMode; }
		bool isUseVRMode(){ return m_bUseVRMode; }
		void setUserVRMode(bool use = true){ m_bUseVRMode = use; }

		// ��ȡ���������Ч����
		ui32 getMaxEffectBatchNum() const { return m_maxEffectBatchNum; }

		// ��ȡĬ�����Ӵ�С(������)
		ui32 getDefaultEffectParticleSizePerBatch() const { return m_defaultEffectParticleSizePerBatch; }

		// ��ȡÿ�������������������
		ui32 getMaxEffectParticlesSizePerBatch() const { return m_maxEffectParticlesSizePerBatch; }

		// �����Ƿ������߳���Ⱦ
		void setEnableMultiThreadRendering(bool isEnable);

		// ���ø�˹ģ��Ʒ��
		void setGuassScaleSetting(float guass_setting_scale);

		// ��ø�˹ģ��Ʒ��
		float getGuassScaleSetting();

		// ����ˮ��Ʒ��
		void setWaterQuality(WaterQuality water_qulity);

		// ���ˮ��Ʒ��
		WaterQuality getWaterQuality();

		// ����ˮ�淴�䷶Χ
		void setWaterRefectRadius(float refect_radius){ m_waterRefectRadius = refect_radius; }
		float getWaterRefectRadius(){ return m_waterRefectRadius; }

		void setAutoOptMatInt(bool setting) { m_bAutoOptMatInt = setting; }
		// �Զ��Ż����ʣ�Ŀǰֻ������Ӱ��صģ�
		bool isAutoOptMatInt() const { return m_bAutoOptMatInt; }

		void setEnableRenderLayer(bool setting) { m_bEnableRenderLayer = setting; }
		bool isEnableRenderLayer() { return m_bEnableRenderLayer; }

		void setSlowDownExclusiveUI(bool value);
		bool isSlowDownExclusiveUI() const;

		void setEnableAnimOptimize(bool enable);
		bool isEnableAnimOptimize() const;

		void setEnableBatchMerge(bool enable);
		bool isEnableBatchMerge() const;
		

	protected:
		bool			m_isShowOcclusionOccluder;					// �Ƿ���ʾ�ڵ���
		bool			m_bEnableHighEffectActor;
		bool			m_bEnableBloom;
		bool			m_bEnableToneMapping;
		bool			m_bEnableFXAA;
		bool			m_bEnableCoverage;
		bool			m_bEnableFilterAdditional;
		bool			m_bEnableDistortion;
		bool			m_bEnableGlow;
		bool			m_bEnableLensFlare;
		bool			m_bPhysicsRender;
		bool			m_bEnableCalcThread;
		bool			m_bEnableStreamThread;
		bool			m_bUseVRMode;							// ���ĳһ�������Ƿ���VR��Ⱦģʽ	
		bool			m_bInitVrMode;							// �������ʼ��ʱ�����������Ƿ��п��ܿ���VR
		bool			m_bSlowDownExclusiveUI;
		ui32			m_maxEffectBatchNum;					// (��Ч)��Ч��������
		ui32			m_defaultEffectParticleSizePerBatch;	// (��Ч)ÿ���������������
		ui32			m_maxEffectParticlesSizePerBatch;		// (��Ч)ÿ���������������
		EffectQuality	m_effectQuality;						// (��Ч)��ЧƷ��
		float			m_guassScaleSetting;					// ��˹ģ��Ʒ��
		WaterQuality	m_waterQuality;							// ˮ��Ʒ��
		float			m_waterRefectRadius;					// ˮ�淢�䷶Χ
		bool			m_bAutoOptMatInt;						// �Զ��Ż����ʣ�Ŀǰֻ������Ӱ��صģ�
		bool			m_bEnableSmallObjectCull;				// �ӽǱ�ԵС�������
		ui32			m_shadowMapSize;						// ��Ӱͼ��С
		bool			m_bEnableRenderLayer;					// ���������������Ⱦ�㼶
		bool			m_bEnableFocusBlur;
		bool			m_bEnableAnimOptimize;
		bool			m_bEnableMainActorShadow;				// �Ƿ����Ⱦ����Ӱ��
		bool			m_bEnableBatchMerge;
	};
}