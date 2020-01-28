#pragma once

#include <string>
#include <engine/core/log/Log.h>
#include <engine/core/main/Engine.h>
#include <engine/core/scene/node_tree.h>
#include <engine/core/render/base/mesh/Mesh.h>

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

	public:
		// on open node tree
		bool onOpenNodeTree(const Echo::String& resPath);

		// current edit node
		void setCurrentEditNode(Echo::Node* node);
		Echo::Node* getCurrentEditNode() { return m_currentEditNode; }

		// save path
		void setCurrentEditNodeSavePath(const Echo::String& savePath);
		const Echo::String& getCurrentEditNodeSavePath() { return m_currentEditNodeSavePath; }

	public:
		// Ԥ������
		virtual void previewAudioEvent( const char* audioEvent);

		// ֹͣ����Ԥ������Դ
		virtual void stopCurPreviewAudioEvent();

		//������ʾ�����ر�������
		virtual void setBackGridVisibleOrNot(bool showFlag);

		// resize grid
		void resizeBackGrid3d();
		void resizeBackGrid2d();

		// ��ȡģ�Ͱ뾶
		float GetMeshRadius();

		// ��������ͼ
		void SaveSceneThumbnail(bool setCam = true);

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

		// init back grid
		void InitializeBackGrid();

	protected:
		Echo::LogOutput*		m_log;
		Echo::Node*				m_currentEditNode;
		Echo::String			m_currentEditNodeSavePath;
		Echo::Node*				m_invisibleNodeForEditor;
		Echo::Gizmos*			m_gizmosNodeGrid3d;
		Echo::Gizmos*			m_gizmosNodeGrid2d;
		static std::string		m_projectFile;
		static RenderWindow*	m_renderWindow;
	};
}
