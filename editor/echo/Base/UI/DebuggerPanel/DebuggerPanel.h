#pragma once

#include <QDockWidget>
#include <QString>
#include "ui_DebuggerPanel.h"

namespace Studio
{
	//----------------------------------------
	// ��� ������ 2010-12-30
	//----------------------------------------
	class DebuggerPanel : public QWidget, public Ui_DebuggerPanel
	{
		Q_OBJECT

	public:
		// ���캯��
		DebuggerPanel(QWidget* parent = 0);

		// ��������
		~DebuggerPanel();

	protected:
		// �ر�
		//void closeEvent(QCloseEvent *event);
	};
}