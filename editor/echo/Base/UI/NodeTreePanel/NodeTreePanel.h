#pragma once

#include <QDockWidget>
#include "ui_NodeTreePanel.h"
#include "QProperty.hpp"
#include "NewNodeDialog.h"
#include <engine/core/resource/Res.h>
#include <engine/core/scene/node.h>

namespace Studio
{
	class NodeTreePanel : public QDockWidget, public Ui_NodeTreePanel
	{
		Q_OBJECT

	public:
		NodeTreePanel( QWidget* parent=0);
		~NodeTreePanel();

		// ��ȡ���ʵ��
		static NodeTreePanel* instance();

		// ���浱ǰ�༭��Դ
		void saveCurrentEditRes();

	public:
		// ˢ�½������ʾ
		void refreshNodeTreeDisplay();

		// ��ӽ��
		void addNode(Echo::Node* node);

		// ��ӽ��
		void addNode( Echo::Node* node, QTreeWidgetItem* parent, bool recursive);

		// ��ȡ��ǰ���
		Echo::Node* getCurrentSelectNode();

		// ��ȡ��ǰ�༭����
		Echo::Object* getCurrentEditObject();

	private slots:
		// ��ʾ�½��ڵ㴰��
		void showNewNodeDialog();

		// node tree widget show menu
		void showMenu(const QPoint& point);

		// ��ʾ��ǰѡ�нڵ�����
		void showSelectedObjectProperty();

		// �����޸ĺ�,���½��ֵ
		void refreshPropertyToObject(const QString& property, QVariant value);

		// on trigger delete nodes
		void onDeleteNodes();

		// on trigger rename node
		void onRenameNode();

		// when modifyd item name
		void onChangedNodeName(QTreeWidgetItem* item);

		// import gltf scene
		void importGltfScene();

	public slots:
		// on select node
		void onSelectNode();

		// edit res
		void onSelectRes(const Echo::String& resPath);

	private:
		// �ݹ���ʾ����
		void showObjectPropertyRecursive(Echo::Object* classPtr, const Echo::String& className);

		// show property
		void showPropertyByVariant(const Echo::String& name, const class Echo::Variant& var);

	private:
		NewNodeDialog*					m_newNodeDialog;		// �½����
		QT_UI::QPropertyConfigHelper	m_propertyHelper;		// ����
		Echo::ResPtr					m_currentEditRes;		// ��ǰ��Դ
		Echo::Node*						m_currentEditNode;
		QMenu*							m_nodeTreeMenu;			// ������Ҽ��˵�
	};
}