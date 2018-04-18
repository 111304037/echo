#include "EchoEngine.h"
#include <QFileInfo>
#include <QString>
#include <array>
#include <Engine/core/main/Root.h>
#include <Engine/Core.h>
#include <Engine/modules/Navigation/Navigation.h>
#include <RenderTargetManager.h>
#include <render/RenderTarget.h>
#include "engine/core/Thread/Threading.h"
#include "Studio.h"
#include "RenderWindow.h"
#include <Render/Renderer.h>
#include "Render/RenderThread.h"
#include <Engine/modules/Audio/FMODStudio/FSAudioManager.h>
#include <Engine/core/main/EngineTimeController.h>
#include <string>
#include <Psapi.h>
#include "studio.h"
#include "gles/GLES2RenderStd.h"
#include "gles/GLES2Renderer.h"
#include "Render/RenderState.h"
#include "Engine/modules/Audio/FMODStudio/FSAudioSource.h"
#include <engine/core/util/PathUtil.h>


// �¼��ص�
void MyAudioEventCallBack(Echo::AudioSouceEventCBType type, Echo::AudioSource *event)
{
}

namespace Studio
{

#define lineNum 10
	std::string	EchoEngine::m_projectFile;		// ��Ŀ����
	RenderWindow* EchoEngine::m_renderWindow = NULL;

	// ���캯��
	EchoEngine::EchoEngine()
		: m_curPlayAudio(0)
		//, m_backGrid(NULL)
		//, m_backGridNode(NULL)		 
		//, m_gridNum(lineNum)
		//, m_gridGap(1)
		, m_log(NULL)
		//, m_FontRenderManager(NULL)
		, m_isShowFps(false)
		, m_isManualUpdateEngine(false)
		, m_currentEditNode(nullptr)
	{
	}

	// ����
	EchoEngine::~EchoEngine()
	{
		//EchoSceneManager->getRootNode()->destroyChild(m_pCameraAxis);

		//EchoSafeDelete(m_FontRenderManager, FontRenderManager);
	}

	// ��ʼ��
	bool EchoEngine::Initialize(HWND hwnd)
	{
		TIME_PROFILE
		(
			Echo::Root::Config rootcfg;

			// �Ƿ�Ԥ����Ŀ�ļ�
			if (!m_projectFile.empty())
			{
				rootcfg.projectFile = m_projectFile.c_str();
			}
			rootcfg.m_isEnableProfiler = true;
			rootcfg.m_isGame = false;
			rootcfg.m_windowHandle = (unsigned int)hwnd;

			EchoRoot->initialize(rootcfg);
		)

		TIME_PROFILE
		(
			// ��������
			InitializeBackGrid();

			// ��ʼ��������Ⱦ������
			//m_FontRenderManager = EchoNew(FontRenderManager);

			InitFPSShow();
		);

		//Echo::PostProcessRenderStage* postProcessStage = static_cast<Echo::PostProcessRenderStage*>(Echo::RenderStageManager::instance()->getRenderStageByID(Echo::RSI_PostProcess));
		//if (postProcessStage)
		{
		//	postProcessStage->setImageEffectEnable("FXAA", Echo::Root::instance()->getEnableFXAA());
		}

		EchoAudioManager->setAudioEventCb(&MyAudioEventCallBack);

		m_currentEditNode = nullptr;

		return true;
	}

	// ж��
	void EchoEngine::Release()
	{
		delete this;
	}

	// ÿ֡��Ⱦ
	void EchoEngine::Render(unsigned int elapsedTime, bool isRenderWindowVisible)
	{
		// ʱ�����
		Echo::EngineTimeController::instance()->update(elapsedTime * 0.001f);
		elapsedTime = static_cast<Echo::ui32>(elapsedTime * Echo::EngineTimeController::instance()->getSpeed());

		// ����
		if (EchoEngineSettings.isInitVRMode() && EchoEngineSettings.isUseVRMode()) // �����۸���
			Echo::NodeTree::instance()->get3dCamera()->setVRModeForEye(true);

		if (!m_isManualUpdateEngine)
		{
			if (m_currentEditNode)
			{
				m_currentEditNode->update( elapsedTime, true);
			}


			if (m_isShowFps)
			{
				//����FPS
				UpdateFPS();

				// ��Ⱦ����
				//FontRenderManager::Instance()->Render();
				EchoRoot->resetFrameState();
				Echo::Renderer::instance()->getFrameState().reset();
			}

			EchoRoot->tick(elapsedTime);
		}
	}

	// �޸Ĵ��ڴ�С
	void EchoEngine::Resize(int cx, int cy)
	{
		EchoRoot->onSize(cx, cy);

		m_renderWindow->getInputController()->onSizeCamera(Echo::Renderer::instance()->getScreenWidth(), Echo::Renderer::instance()->getScreenHeight());
	}

	// ����project
	bool EchoEngine::SetProject(const char* projectFile)
	{
		m_projectFile = projectFile;

		// ��ʼ����Ⱦ����
		m_renderWindow = static_cast<RenderWindow*>(AStudio::Instance()->getRenderWindow());

		return true;
	}

	// save current node tree
	void EchoEngine::saveCurrentEditNodeTree()
	{
		if (m_currentEditNode)
		{
			m_currentEditNode->save("Res://main.scene");
		}
	}

	// �л�����
	void EchoEngine::ChangeScene(const char* sceneName, bool isGameMode)
	{
		using namespace  Echo;
		//m_sceneMgr->loadScene(sceneName, Vector3::ZERO, 3, false, isGameMode);
		InitializeBackGrid();
		m_renderWindow->getInputController()->onAdaptCamera();
	}

	// ��ʼ����������
	void EchoEngine::InitializeBackGrid()
	{
		ResizeBackGrid(lineNum, 1);
	}

	//���ɱ�������(�����������)
	void EchoEngine::ResizeBackGrid(int linenums, float lineGap)
	{/*
		if (m_backGrid)
		{
			m_backGrid->attachTo(NULL);
			EchoDebugDisplayManager->destroyQueryObject(m_backGrid);
		}
		m_backGrid = EchoDebugDisplayManager->createQueryObject("BackGrid");
		m_backGrid->setQueryFlag(0x00000000);
		m_backGrid->setTestDepth(true);
		m_backGrid->setWriteDepth(false);
		m_backGrid->setVisible(true);

		m_backGrid->beginPolygon(Echo::QueryObject::QO_WIREFRAME);
		{
			// ʮ�ֺ���
			std::array<Echo::Vector3, 4> vertexData;

			vertexData[0] = Echo::Vector3(-linenums*lineGap, 0, 0);
			vertexData[1] = Echo::Vector3(linenums*lineGap, 0, 0);
			vertexData[2] = Echo::Vector3(0, 0, -linenums*lineGap);
			vertexData[3] = Echo::Vector3(0, 0, linenums*lineGap);

			for (size_t i = 0; i < vertexData.size(); i++)
			{
				m_backGrid->setPosition(vertexData[i]);
				m_backGrid->setColor(Echo::Color(0.7f, 0.0f, 0.0f));
			}
		}
		{
			// ����
			std::vector<Echo::Vector3> vertexData(linenums * 8, Echo::Vector3(0, 0, 0));
			//for(int i =0;i<linenums*8;i++)
			//{
			//	vertexData[i] = Echo::Vector3(0,0,0);
			//}

			for (int i = 0; i < linenums; i++)
			{
				int j = i + 1;
				vertexData[i * 8 + 0] = Echo::Vector3(linenums*lineGap, 0, j*lineGap);
				vertexData[i * 8 + 1] = Echo::Vector3(-linenums*lineGap, 0, j*lineGap);
				vertexData[i * 8 + 2] = Echo::Vector3(j*lineGap, 0, linenums*lineGap);
				vertexData[i * 8 + 3] = Echo::Vector3(j*lineGap, 0, -linenums*lineGap);
				vertexData[i * 8 + 4] = Echo::Vector3(linenums*lineGap, 0, -j*lineGap);
				vertexData[i * 8 + 5] = Echo::Vector3(-linenums*lineGap, 0, -j*lineGap);
				vertexData[i * 8 + 6] = Echo::Vector3(-j*lineGap, 0, linenums*lineGap);
				vertexData[i * 8 + 7] = Echo::Vector3(-j*lineGap, 0, -linenums*lineGap);
			}

			for (size_t i = 0; i < vertexData.size(); i++)
			{
				m_backGrid->setPosition(vertexData[i]);
				m_backGrid->setColor(Echo::Color(0.6f, 0.6f, 0.6f));
			}
		}
		m_backGrid->endPolygon();
		if (!m_backGridNode)
		{
			m_backGridNode = EchoSceneManager->getRootNode()->createChild();
		}

		m_backGridNode->setWorldPosition(Echo::Vector3::ZERO);
		m_backGrid->attachTo(m_backGridNode);
		m_gridNum = linenums;
		m_gridGap = lineGap;

		*/
	}

	//������ʾ�����ر�������
	void EchoEngine::setBackGridVisibleOrNot(bool showFlag)
	{
		/*
		if (m_backGrid && m_backGridNode)
		{
			if (!showFlag)
			{
				m_backGrid->attachTo(NULL);
			}
			else
			{
				m_backGrid->attachTo(m_backGridNode);
			}
		}
		*/
	}

	//��ñ����������ز���
	void EchoEngine::GetBackGridParameters(int* linenums, float* lineGap)
	{
		//if (linenums)
		//{
		//	*linenums = m_gridNum;
		//}
		//if (lineGap)
		//{
		//	*lineGap = m_gridGap;
		//}
	}

	// Ԥ������
	void EchoEngine::previewAudioEvent(const char* audioEvent)
	{
		EchoAudioManager->destroyAudioSources(&m_curPlayAudio, 1);

		bool isAudioEvnet = Echo::StringUtil::StartWith(audioEvent, "event:", true);
		if (isAudioEvnet)
		{
			Echo::AudioSource::Cinfo cinfo;
			cinfo.m_name = audioEvent;
			cinfo.m_is3DMode = false;
			m_curPlayAudio = EchoAudioManager->createAudioSource(cinfo);
		}
	}

	// ֹͣ����Ԥ������Դ
	void EchoEngine::stopCurPreviewAudioEvent()
	{
		EchoAudioManager->destroyAudioSources(&m_curPlayAudio, 1);
	}

	// ��ȡģ�Ͱ뾶
	float EchoEngine::GetMeshRadius()
	{
		return 10.f;
	}

	// ���浱ǰ�༭����
	void EchoEngine::SaveScene()
	{
		SaveSceneThumbnail();
	}

	// ��������ͼ
	void EchoEngine::SaveSceneThumbnail(bool setCam)
	{
		//Echo::RenderTarget* defaultBackBuffer = Echo::RenderTargetManager::Instance()->getRenderTargetByID(Echo::RTI_DefaultBackBuffer);
		//if (defaultBackBuffer)
		//{
		//	Echo::String sceneFullPath = EchoResourceManager->getFileLocation(EchoSceneManager->getCurrentScene()->getSceneName() + ".scene");
		//	Echo::String sceneLocation = Echo::PathUtil::GetFileDirPath(sceneFullPath);
		//	Echo::String bmpSavePath = Echo::PathUtil::GetRenameExtFile(sceneFullPath, ".bmp");
		//	if (setCam)
		//		defaultBackBuffer->saveTo((std::string(bmpSavePath.c_str())).c_str());
		//	else
		//		defaultBackBuffer->saveTo((std::string(sceneLocation.c_str()) + "/map.bmp").c_str());
		//}
	}


	void EchoEngine::InitFPSShow()
	{
		//m_FpsPrarentNode = EchoSceneManager->getRootNode()->createChild();

		//Echo::node* pSceneNode;
		//FontCNRender* pTextRender;

		//for (int j = 0; j < FPSINFONUM; ++j)
		//{
		//	pSceneNode = m_FpsPrarentNode->createChild();
		//	pTextRender = FontRenderManager::Instance()->CreateFontCNRender();
		//	pTextRender->setFontColor(Echo::Color::GREEN);
		//	pTextRender->setParentSceneNode(pSceneNode);
		//	pTextRender->setRenderText(" ");
		//	pTextRender->setScale(0.7f);
		//	m_pFpsNodes[j] = pSceneNode;
		//	m_pTextRenders[j] = pTextRender;
		//}

	}

	void EchoEngine::UpdateFPS()
	{
		//Echo::ui32	fps = EchoRoot->getFPS();
		//Echo::ui32	drawCallTimes = EchoRender->getFrameState().getDrawCalls();
		//Echo::ui32	triangleNum = EchoRender->getFrameState().getTriangleNum();
		//float		vertexSize = EchoRender->getFrameState().getVertexSize() / 1024.f / 1024.f;
		//float		textureSize = EchoRender->getFrameState().getTextureSizeInBytes() / 1024.f / 1024.f;

		//_PROCESS_MEMORY_COUNTERS pmc;
		//pmc.cb = sizeof(_PROCESS_MEMORY_COUNTERS);
		//GetProcessMemoryInfo(GetCurrentProcess(), &pmc, pmc.cb);
		//float memorySize = pmc.WorkingSetSize / (1024.f * 1024.f);

		//char format[256];
		//sprintf(format, "FPS: %d", fps);
		//m_pTextRenders[0]->setRenderText(format, false);
		//upDateFpsNode(0);

		//sprintf(format, "DrawCall: %d", drawCallTimes);
		//m_pTextRenders[1]->setRenderText(format, false);
		//upDateFpsNode(1);

		//sprintf(format, "TriangleNum: %d", triangleNum);
		//m_pTextRenders[2]->setRenderText(format, false);
		//upDateFpsNode(2);

		//sprintf(format, "VertexSize: %.3f mb", vertexSize);
		//m_pTextRenders[3]->setRenderText(format, false);
		//upDateFpsNode(3);

		//sprintf(format, "TextureSize: %.3f mb", textureSize);
		//m_pTextRenders[4]->setRenderText(format, false);
		//upDateFpsNode(4);

		//sprintf(format, "MemorySize: %.3f mb", memorySize);
		//m_pTextRenders[5]->setRenderText(format, false);
		//upDateFpsNode(5);
	}

	void EchoEngine::upDateFpsNode(int index)
	{
		//Echo::Ray _ray;
		//Echo::Vector2 point = Echo::Vector2(20.0f, 20.0f + index * 20.f);
		//EchoSceneManager->getMainCamera()->getCameraRay(_ray, point);
		//Echo::Real fov = EchoSceneManager->getMainCamera()->getFov();
		//Echo::Vector3 fpsPos = _ray.o;
		//fpsPos += _ray.dir*2.3f;
		//m_pTextRenders[index]->setScale(fov);

		//m_pFpsNodes[index]->setWorldPosition(fpsPos);
		//m_pFpsNodes[index]->update();
	}
}
