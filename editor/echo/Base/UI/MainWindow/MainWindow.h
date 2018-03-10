#pragma once

#include "ui_MainWindow.h"

namespace Studio
{
	class ResPanel;
	class ScenePanel;
	class TimelinePanel;
	class DebuggerPanel;
	class MainWindow : public QMainWindow, public Ui_MainWindow
	{
		Q_OBJECT

	public:
		MainWindow(QMainWindow* parent=0);
		~MainWindow();

		// ����Ŀʱ����
		void onOpenProject();

		// ���ļ�
		void OpenProject(const char* projectName);

	protected:
		void closeEvent(QCloseEvent *event);

	private slots:
		// �����ļ�
		void SaveFile();

	protected:
		ResPanel*		m_resPanel;
		ScenePanel*		m_scenePanel;
		TimelinePanel*	m_timelinePanel;
		DebuggerPanel*	m_debuggerPanel;
	};
}