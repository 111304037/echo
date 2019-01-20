#pragma once

#include <string>
#include <engine/core/log/Log.h>
#include <engine/core/main/Engine.h>
#include <engine/core/scene/node_tree.h>
#include <engine/core/render/interface/mesh/Mesh.h>

namespace Studio
{
	class RenderWindow; 
	class EchoEngine
	{
	public:
		virtual ~EchoEngine();

		// inst
		static EchoEngine* instance();

		// initialize
		bool Initialize( size_t hwnd);

		// ÿ֡��Ⱦ
		void Render( float elapsedTime, bool isRenderWindowVisible);

		// �޸Ĵ��ڴ�С
		void Resize(int cx, int cy);

		// ����project
		static bool SetProject( const char* projectFile);

		// ж��
		void Release();

	public:
		// on open node tree
		bool onOpenNodeTree(const Echo::String& resPath);

		// set current edit node
		void setCurrentEditNode(Echo::Node* node);

		// ��ȡ��ǰ�༭���
		Echo::Node* getCurrentEditNode() { return m_currentEditNode; }

		// ���õ�ǰ������洢·��
		void setCurrentEditNodeSavePath(const Echo::String& savePath);

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
		void resizeBackGrid3d();
		void resizeBackGrid2d();

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

		// save branch node as scene
		void saveBranchAsScene(const Echo::String& savePath, Echo::Node* node);

	private:
		EchoEngine();

		// ��ʼ����������
		void InitializeBackGrid();

	protected:
		Echo::LogOutput*		m_log;						// ��־
		Echo::Node*				m_currentEditNode;			// ��ǰ�༭����
		Echo::String			m_currentEditNodeSavePath;	// ��ǰ�����洢·��
		Echo::Node*				m_invisibleNodeForEditor;	// �༭�����
		Echo::Gizmos*			m_gizmosNodeBackGrid;
		Echo::Gizmos*			m_gizmosNodeGrid2d;			// 2d ��
		static std::string		m_projectFile;				// ��Ŀ����
		static RenderWindow*	m_renderWindow;				// ��Ⱦ����
	};
}
