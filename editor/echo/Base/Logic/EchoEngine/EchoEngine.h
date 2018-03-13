#pragma once

#include <string>
#include <Engine/Core.h>
#include <Engine/core/main/Root.h>
#include <Engine/core/Scene/Scene_Manager.h>
#include <Engine/modules/Model/Mesh.h>
#include <Engine/modules/Audio/FMODStudio/FSAudioManager.h>

#define  FPSINFONUM 6

namespace Studio
{
	class RenderWindow; 
	class FBXManager;
	class EchoEngine : public Echo::Singleton<EchoEngine>
	{
	public:
		EchoEngine();
		~EchoEngine();

		// ��ʼ��
		bool Initialize( HWND hwnd);

		// ÿ֡��Ⱦ
		void Render( unsigned int elapsedTime, bool isRenderWindowVisible);

		// �޸Ĵ��ڴ�С
		void Resize(int cx, int cy);

		// ����project
		static bool SetProject( const char* projectFile);

		// ж��
		void Release();

		// ��ȡFBX��ʽ������
		FBXManager* GetFBXManager() { return m_fbxManager; }

	public:
		// ���õ�ǰ�༭���
		void setCurrentEditNode(Echo::Node* node) { m_currentEditNode = node; }

		// ��ȡ��ǰ�༭���
		Echo::Node* getCurrentEditNode() { return m_currentEditNode; }

	public:
		// Ԥ������
		virtual void previewAudioEvent( const char* audioEvent);

		// ֹͣ����Ԥ������Դ
		virtual void stopCurPreviewAudioEvent();

		//������ʾ�����ر�������
		virtual void setBackGridVisibleOrNot(bool showFlag);

		//������������Ĳ���
		virtual void ResizeBackGrid(int linenums,float lineGap);

		//��ñ����������ز���
		virtual void GetBackGridParameters(int* linenums,float* lineGap);

		// ��ȡģ�Ͱ뾶
		float GetMeshRadius();

		// ��������ͼ
		void SaveSceneThumbnail(bool setCam = true);

		// �������
	public:
		// �л�����
		void ChangeScene( const char* sceneName, bool isGameMode=false);

		// ���浱ǰ�༭����
		void SaveScene();

		// ����FPS�Ƿ���ʾ
		void SetIsFPSShow(bool setting){ m_isShowFps = setting; }

	private:
		// ��ʼ����������
		void InitializeBackGrid();

		// ��ʼ��FPS��ʾ
		void InitFPSShow();

		// ����FPS��ʾ
		void UpdateFPS();

		// ��������ڵ�
		void upDateFpsNode(int index);

	protected:
		Echo::Log*				m_log;				// ��־
		Echo::SceneManager*		m_sceneMgr;			// ����������
		Echo::Node*				m_currentEditNode;	// ��ǰ�༭����

		//Echo::node*			m_backGridNode;	// ���������ó������
		//Echo::DebugDisplayItem*	m_backGrid;		// ��������
		//int						m_gridNum;		// ���������
		//float					m_gridGap;		// ������

		Echo::Node *		m_pCameraAxis;

		static std::string		m_projectFile;		// ��Ŀ����
		static RenderWindow*	m_renderWindow;	// ��Ⱦ����

		FBXManager*				m_fbxManager;		// fbx��ʽ������

		Echo::AudioSourceID		m_curPlayAudio;		// ��ǰ������Ƶ

		//FontRenderManager*  m_FontRenderManager; // ������Ⱦ  
		bool					m_isShowFps;		 // �Ƿ���ʾFPS	
		Echo::Node*				m_FpsPrarentNode;	 // FPS���ڵ�
		Echo::array<Echo::Node*,FPSINFONUM>	m_pFpsNodes;	 // FPS��ʾ�����ص�
		//Echo::array<FontCNRender*, FPSINFONUM>		m_pTextRenders;	 // ���弯��
		bool				m_isManualUpdateEngine; 
	};
}
