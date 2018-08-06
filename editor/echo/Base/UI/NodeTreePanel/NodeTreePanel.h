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
		// clear
		void clear();

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

		// ���ñ༭�ڵ�
		void setNextEditObject(Echo::Object* obj) { m_nextEditObject = obj; }

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

		// on duplicate node
		void onDuplicateNode();

		// on change type
		void onChangeType();

		// on trigger rename node
		void onRenameNode();

		// on save branch as scene
		void onSaveBranchAsScene();

		// on discard instancing
		void onDiscardInstancing();

		// when modifyd item name
		void onChangedNodeName(QTreeWidgetItem* item);

		// import gltf scene
		void importGltfScene();

		// on instance child scene
		void onInstanceChildScene();

		// refresh node display
		void refreshNodeDisplay(QTreeWidgetItem* item);

	public slots:
		// ��Ⱦ
		void  update();

		// on select node
		void onSelectNode();

		// on clicked node item
		void onClickedNodeItem(QTreeWidgetItem* item, int column);

		// edit res
		void onEditObject(Echo::Object* res);

		// edit res
		void onSelectRes(const Echo::String& resPath);

	protected:
		// update item display
		void updateNodeTreeWidgetItemDisplay(QTreeWidgetItem* item);

	private:
		// �ݹ���ʾ����
		void showObjectPropertyRecursive(Echo::Object* classPtr, const Echo::String& className);

		// show property
		void showPropertyByVariant(const Echo::String& name, const class Echo::Variant& var, const Echo::PropertyInfo* propInfo);

	private:
		// remove item
		void removeItem(QTreeWidgetItem* item);

	private:
		int								m_width;
		QTimer*							m_timer;				// ��ʱ��
		QT_UI::QPropertyConfigHelper	m_propertyHelper;		// ����
		Echo::Object*					m_nextEditObject;		
		Echo::Object*					m_currentEditObject;
		QMenu*							m_nodeTreeMenu;			// ������Ҽ��˵�
	};
}