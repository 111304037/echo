#include "EchoEngine.h"
#include <QFileInfo>
#include <QString>
#include <array>
#include <Engine/core/main/Engine.h>
#include <Engine/modules/Navigation/Navigation.h>
#include <RenderTargetManager.h>
#include <render/RenderTarget.h>
#include "engine/core/Thread/Threading.h"
#include "Studio.h"
#include "RenderWindow.h"
#include <Render/Renderer.h>
#include <Engine/modules/Audio/FMODStudio/FSAudioManager.h>
#include <string>
#include <Psapi.h>
#include "studio.h"
#include "gles/GLES2RenderBase.h"
#include "gles/GLES2Renderer.h"
#include "Render/RenderState.h"
#include "Engine/modules/Audio/FMODStudio/FSAudioSource.h"
#include <engine/core/util/PathUtil.h>
#include <engine/core/util/TimeProfiler.h>


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
		, m_currentEditNode(nullptr)
		, m_invisibleNodeForEditor(nullptr)
		, m_gizmosNodeForEditor(nullptr)
	{
	}

	// ����
	EchoEngine::~EchoEngine()
	{
		//EchoSceneManager->getRootNode()->destroyChild(m_pCameraAxis);

		//EchoSafeDelete(m_FontRenderManager, FontRenderManager);
	}

	// inst
	EchoEngine* EchoEngine::instance()
	{
		static EchoEngine* inst = new EchoEngine;
		return inst;
	}

	// ��ʼ��
	bool EchoEngine::Initialize(HWND hwnd)
	{
		TIME_PROFILE
		(
			Echo::Engine::Config rootcfg;

			// �Ƿ�Ԥ����Ŀ�ļ�
			if (!m_projectFile.empty())
			{
				rootcfg.m_projectFile = m_projectFile.c_str();
			}
			rootcfg.m_isEnableProfiler = true;
			rootcfg.m_isGame = false;
			rootcfg.m_windowHandle = (unsigned int)hwnd;

			Echo::Engine::instance()->initialize(rootcfg);
		)

		TIME_PROFILE
		(
			// ��������
			InitializeBackGrid();
		);

		Echo::FSAudioManager::instance()->setAudioEventCb(&MyAudioEventCallBack);

		m_currentEditNode = nullptr;

		{
			// editor node
			m_invisibleNodeForEditor = ECHO_DOWN_CAST<Echo::Node*>(Echo::Class::create("Node"));

			// gizmos node
			m_gizmosNodeForEditor = ECHO_DOWN_CAST<Echo::Gizmos*>(Echo::Class::create("Gizmos"));
			m_gizmosNodeForEditor->setName("Gizmos");
			m_gizmosNodeForEditor->setParent(m_invisibleNodeForEditor);
		}

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
		m_gizmosNodeForEditor->drawLine(-Echo::Vector3(100.f, 0.f, 0.f), Echo::Vector3(100.f, 0.f, 0.f), Echo::Color::RED, false, 1.f);

		if (m_currentEditNode)
			m_currentEditNode->update( elapsedTime, true);

		if (m_invisibleNodeForEditor)
			m_invisibleNodeForEditor->update(elapsedTime, true);

		Echo::Engine::instance()->tick(elapsedTime);
	}

	// �޸Ĵ��ڴ�С
	void EchoEngine::Resize(int cx, int cy)
	{
		Echo::Engine::instance()->onSize(cx, cy);

		m_renderWindow->getInputController()->onSizeCamera(Echo::Renderer::instance()->getScreenWidth(), Echo::Renderer::instance()->getScreenHeight());
	}

	// ����project
	bool EchoEngine::SetProject(const char* projectFile)
	{
		m_projectFile = projectFile;

		// ��ʼ����Ⱦ����
		m_renderWindow = static_cast<RenderWindow*>(AStudio::instance()->getRenderWindow());

		return true;
	}

	// save current node tree
	void EchoEngine::saveCurrentEditNodeTree()
	{
		if (m_currentEditNode && !m_currentEditNodeSavePath.empty())
		{
			m_currentEditNode->save( m_currentEditNodeSavePath);
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

	// ��ñ����������ز���
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
		Echo::FSAudioManager::instance()->destroyAudioSources(&m_curPlayAudio, 1);

		bool isAudioEvnet = Echo::StringUtil::StartWith(audioEvent, "event:", true);
		if (isAudioEvnet)
		{
			Echo::AudioSource::Cinfo cinfo;
			cinfo.m_name = audioEvent;
			cinfo.m_is3DMode = false;
			m_curPlayAudio = Echo::FSAudioManager::instance()->createAudioSource(cinfo);
		}
	}

	// ֹͣ����Ԥ������Դ
	void EchoEngine::stopCurPreviewAudioEvent()
	{
		Echo::FSAudioManager::instance()->destroyAudioSources(&m_curPlayAudio, 1);
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
}
