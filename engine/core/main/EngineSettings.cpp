#include "EngineSettings.h"
#include "engine/core/util/PathUtil.h"
#include <engine/core/render/render/RenderThread.h>
#include "Root.h"
#include <thirdparty/pugixml/pugixml.hpp>
#include "EngineTimeController.h"

namespace Echo
{ 
	// ��ȡʵ��
	static EngineSettingsMgr* g_engineSettings = nullptr;

	// ���캯��
	EngineSettingsMgr::EngineSettingsMgr()
		: m_isShowOcclusionOccluder(true)
		, m_bEnableHighEffectActor(false)
		, m_bEnableBloom(false)
		, m_bEnableToneMapping(true)
		, m_bEnableFXAA(false)
		, m_bEnableFilterAdditional(false)
		, m_bEnableDistortion(false)
		, m_bEnableGlow(false)
		, m_bEnableLensFlare(false)
		, m_bPhysicsRender(false)
		, m_bEnableCalcThread(true)
		, m_bEnableStreamThread(true)
		, m_bEnableCoverage(false)
		, m_maxEffectBatchNum(-1)
		, m_defaultEffectParticleSizePerBatch(384)
		, m_maxEffectParticlesSizePerBatch(384)
		, m_effectQuality(EQ_High)
		, m_bInitVrMode(false)
		, m_bUseVRMode(false)
		, m_bSlowDownExclusiveUI(true)
		, m_guassScaleSetting(0)
		, m_waterQuality(WQ_High)
		, m_bAutoOptMatInt(true)
		, m_waterRefectRadius(40.0f)
		, m_bEnableSmallObjectCull(false)
		, m_shadowMapSize(1024)
		, m_bEnableRenderLayer(true)
		, m_bEnableFocusBlur(false)
		, m_bEnableAnimOptimize(false)
		, m_bEnableMainActorShadow(false)
		, m_bEnableBatchMerge(true)
	{
		EchoAssert(!g_engineSettings);
		g_engineSettings = this;
	}

	// ��������
	EngineSettingsMgr::~EngineSettingsMgr()
	{

	}

	// ��ȡʵ��
	EngineSettingsMgr* EngineSettingsMgr::instance()
	{
		EchoAssert(g_engineSettings);
		return g_engineSettings;
	}

	// Ӧ������
	void EngineSettingsMgr::Apply(const String& fileName)
	{
		//Echo::String lstrFile = Root::instance()->getResPath() + fileName;
		//if (!PathUtil::IsFileExist(lstrFile))
		//{
		//	EchoLogInfo("Config file Engine.xml not exits");
		//	return;
		//}

		//Echo::FileHandleDataStream memory(lstrFile.c_str());
		//xml_document<> doc;        // character type defaults to cha
		//vector<char>::type buffer;
		//buffer.resize(memory.size() + 1);
		//memory.readAll(&buffer[0]);
		//doc.parse<0>(&buffer[0]);

		//xml_node<>* pEngineConfigRoot = doc.first_node();
		//if (!pEngineConfigRoot)
		//{
		//	EchoLogInfo("invalid engine config file [%s].", fileName.c_str());
		//	return;
		//}
		//xml_node<>* pValueNode = pEngineConfigRoot->first_node();
		//while (pValueNode)
		//{
		//	String strNodeName = pValueNode->name();
		//	xml_attribute<>* pValue = pValueNode->first_attribute();
		//	if (pValue)
		//	{
		//		String strValue = pValue->value();
		//		if (strNodeName == "EnableHighEffectActor")
		//		{
		//			m_bEnableHighEffectActor = StringUtil::ParseBool(strValue);
		//		}
		//		else if (strNodeName == "EnableBloom")
		//		{
		//			m_bEnableBloom = StringUtil::ParseBool(strValue);
		//		}
		//		else if (strNodeName == "EnableToneMapping")
		//		{
		//			m_bEnableToneMapping = StringUtil::ParseBool(strValue);
		//		}
		//		else if (strNodeName == "EnableFXAA")
		//		{
		//			m_bEnableFXAA = StringUtil::ParseBool(strValue);
		//		}
		//		else if (strNodeName == "EnableCoverage")
		//		{
		//			m_bEnableCoverage = StringUtil::ParseBool(strValue);
		//		}
		//		else if (strNodeName == "EnableFilterAdditional")
		//		{
		//			m_bEnableFilterAdditional = StringUtil::ParseBool(strValue);
		//		}
		//		else if (strNodeName == "EnableDistortion")
		//		{
		//			m_bEnableDistortion = StringUtil::ParseBool(strValue);
		//		}
		//		else if (strNodeName == "EnableLensFlare")
		//		{
		//			m_bEnableLensFlare = StringUtil::ParseBool(strValue);
		//		}
		//		else if (strNodeName == "EnableCalcThread")
		//		{
		//			m_bEnableCalcThread = StringUtil::ParseBool(strValue);
		//		}
		//		else if (strNodeName == "EnableStreamThread")
		//		{
		//			m_bEnableStreamThread = StringUtil::ParseBool(strValue);
		//		}
		//		else if (strNodeName == "GuassScaleSetting")
		//		{
		//			m_guassScaleSetting = StringUtil::ParseFloat(strValue);
		//		}
		//		else if (strNodeName == "EnableSmallObjectCull")
		//		{
		//			setEnableSmallObjectCull(StringUtil::ParseBool(strValue));
		//		}
		//		else if (strNodeName == "ShadowMapSize")
		//		{
		//			m_shadowMapSize = StringUtil::ParseUI32(strValue);
		//		}
		//	}
		//	pValueNode = pValueNode->next_sibling();
		//}
		////config all
		//Root::instance()->setEnableFilterAdditional(m_bEnableFilterAdditional);
		//setEnableDistortion(m_bEnableDistortion);
		//EffectSystemManager::instance()->setEnableDistortionRender(m_bEnableDistortion);
		//Root::instance()->enableStreamThread(m_bEnableStreamThread);
	}

	// �����Ƿ�ʹ��������
	void EngineSettingsMgr::setEnableStreamThread(bool isUseStreamThread)
	{
		m_bEnableStreamThread = isUseStreamThread;
		Root::instance()->enableStreamThread(m_bEnableStreamThread);
	}

	void EngineSettingsMgr::setEnableSmallObjectCull(bool enable)
	{
#ifndef ECHO_EDITOR_MODE
		m_bEnableSmallObjectCull = enable;
#endif
	}

	// �����Ƿ���Ⱦ����
	void EngineSettingsMgr::setEnablePhysicsRender(bool physicsRender)
	{
		m_bPhysicsRender = physicsRender;
	}

	// �����رն��߳���Ⱦ
	void EngineSettingsMgr::setEnableMultiThreadRendering(bool isEnable)
	{
		Echo::g_render_thread->enableThreadedRendering(false);
	}

	//���ø�˹ģ��Ʒ��
	void EngineSettingsMgr::setGuassScaleSetting(float guass_setting_scale)
	{
		m_guassScaleSetting = guass_setting_scale;
		if (m_guassScaleSetting > 1.0f)
		{
			m_guassScaleSetting = 1.0f;
		}
		if (m_guassScaleSetting < 0.125f)
		{
			m_guassScaleSetting = 0.125f;
		}
	}

	// ��ø�˹ģ��Ʒ��
	float EngineSettingsMgr::getGuassScaleSetting()
	{
		return m_guassScaleSetting;
	}

	// ����ˮ��Ʒ��
	void EngineSettingsMgr::setWaterQuality(WaterQuality water_qulity)
	{
		m_waterQuality = water_qulity;
	}

	// ���ˮ��Ʒ��
	WaterQuality EngineSettingsMgr::getWaterQuality()
	{
		return m_waterQuality;
	}

	void EngineSettingsMgr::setSlowDownExclusiveUI(bool value)
	{
		m_bSlowDownExclusiveUI = value;
	}

	bool EngineSettingsMgr::isSlowDownExclusiveUI() const
	{
		return m_bSlowDownExclusiveUI;
	}

	void EngineSettingsMgr::setEnableAnimOptimize(bool enable)
	{
		m_bEnableAnimOptimize = enable;
	}

	bool EngineSettingsMgr::isEnableAnimOptimize() const
	{
		return m_bEnableAnimOptimize;
	}

	void EngineSettingsMgr::setEnableBatchMerge(bool enable)
	{
		m_bEnableBatchMerge = enable;
	}

	bool EngineSettingsMgr::isEnableBatchMerge() const
	{
		return m_bEnableBatchMerge;
	}

}
