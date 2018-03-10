#pragma once

#include <QDockWidget>
#include "ui_ScenePanel.h"
#include "QProperty.hpp"
#include "NewNodeDialog.h"

namespace Studio
{
	class ScenePanel : public QDockWidget, public Ui_ScenePanel
	{
		Q_OBJECT

	public:
		ScenePanel( QWidget* parent=0);
		~ScenePanel();

		// ������ʾ����
		void setDisplayConfig(const char* config);

		// ��������ֵ
		void setPropertyValue(const char* propertyName, QVariant& value);

	private slots:
		void showNewNodeDialog();

	private:
		QT_UI::QProperty*	m_property;				// ����
		NewNodeDialog*		m_newNodeDialog;		// �½����
	};
}