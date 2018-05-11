#pragma once

#include <QDockWidget>
#include <QString>
#include "ui_BottomPanel.h"
#include <engine/core/Util/Log.h>

namespace Studio
{
	class TimelinePanel;
	class DebuggerPanel;
	class BottomPanel : public QDockWidget, public Ui_BottomPanel
	{
		Q_OBJECT

	public:
		// ���캯��
		BottomPanel(QWidget* parent = 0);

		// ��������
		~BottomPanel();

	public slots:
		// on tab index changed
		void onTabIdxChanged(int idx);

	private:
		TimelinePanel*	m_timelinePanel;
		DebuggerPanel*	m_debuggerPanel;
	};
}