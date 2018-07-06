#pragma once

#include <string>
#include <Engine/core/main/Engine.h>
#include <Engine/core/Scene/NodeTree.h>
#include <Engine/core/render/mesh/Mesh.h>
#include <Engine/modules/Audio/FMODStudio/FSAudioManager.h>

#define  FPSINFONUM 6

namespace Studio
{
	class RenderWindow; 
	class EchoEngine
	{
	public:
		~EchoEngine();

		// inst
		static EchoEngine* instance();

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

	public:
		// ���õ�ǰ�༭���
		void setCurrentEditNode(Echo::Node* node) { m_currentEditNode = node; }

		// ��ȡ��ǰ�༭���
		Echo::Node* getCurrentEditNode() { return m_currentEditNode; }

		// ���õ�ǰ������洢·��
		void setCurrentEditNodeSavePath(const Echo::String& savePath) { m_currentEditNodeSavePath = savePath; }

		// ��ȡ��ǰ������洢·��
		const Echo::String& getCurrentEditNodeSavePath() { return m_currentEditNodeSavePath; }

	public:
		// Ԥ������
		virtual void previewAudioEvent( const char* audioEvent);

		// ֹͣ����Ԥ������Դ
		virtual void stopCurPreviewAudioEvent();

		//������ʾ�����ر�������
		virtual void setBackGridVisibleOrNot(bool showFlag);

		//������������Ĳ���
		virtual void ResizeBackGrid();

		//��ñ����������ز���
		virtual void GetBackGridParameters(int* linenums,float* lineGap);

		// ��ȡģ�Ͱ뾶
		float GetMeshRadius();

		// ��������ͼ
		void SaveSceneThumbnail(bool setCam = true);

		// �������
	public:
		// new
		void newEditNodeTree();

		// save current node tree
		void saveCurrentEditNodeTree();
		void saveCurrentEditNodeTreeAs(const Echo::String& savePath);

		// �л�����
		void ChangeScene( const char* sceneName, bool isGameMode=false);

		// ���浱ǰ�༭����
		void SaveScene();

	private:
		EchoEngine();

		// ��ʼ����������
		void InitializeBackGrid();

	protected:
		Echo::Log*				m_log;						// ��־
		Echo::Node*				m_currentEditNode;			// ��ǰ�༭����
		Echo::String			m_currentEditNodeSavePath;	// ��ǰ�����洢·��
		Echo::Node*				m_invisibleNodeForEditor;	// �༭�����
		Echo::Gizmos*			m_gizmosNodeBackGrid;
		Echo::Node *			m_pCameraAxis;
		static std::string		m_projectFile;				// ��Ŀ����
		static RenderWindow*	m_renderWindow;				// ��Ⱦ����
		Echo::AudioSourceID		m_curPlayAudio;				// ��ǰ������Ƶ
	};
}
