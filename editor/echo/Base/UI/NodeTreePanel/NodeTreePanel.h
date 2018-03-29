#pragma once

#include <QDockWidget>
#include "ui_NodeTreePanel.h"
#include "QProperty.hpp"
#include "NewNodeDialog.h"
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

	public:
		// ˢ�½������ʾ
		void refreshNodeTreeDisplay();

		// ��ӽ��
		void addNode(Echo::Node* node);

		// ��ӽ��
		void addNode( Echo::Node* node, QTreeWidgetItem* parent, bool recursive);

		// ��ȡ��ǰ���
		Echo::Node* getCurrentSelectNode();

	private slots:
		// ��ʾ�½��ڵ㴰��
		void showNewNodeDialog();

		// ��ʾ��ǰѡ�нڵ�����
		void showSelectedNodeProperty();

		// �����޸ĺ�,���½��ֵ
		void refreshPropertyToNode(const QString& property, QVariant value);

	private:
		// �ݹ���ʾ����
		void showNodePropertyRecursive(const Echo::String& className);

	private:
		NewNodeDialog*					m_newNodeDialog;		// �½����
		QT_UI::QPropertyConfigHelper	m_propertyHelper;		// ����
	};
}