#pragma once

#include <QProcess>
#include "ui_MainWindow.h"
#include <engine/core/util/StringUtil.h>
#include <QComboBox>
#include <QMdiArea>

namespace Studio
{
	class ResPanel;
	class NodeTreePanel;
	class TimelinePanel;
	class DebuggerPanel;
	class DocumentPanel;
	class BottomPanel;
	class MainWindow : public QMainWindow, public Ui_MainWindow
	{
		Q_OBJECT

	public:
		MainWindow(QMainWindow* parent=0);
		~MainWindow();

		// instance
		static MainWindow* instance();

		// ����Ŀʱ����
		void onOpenProject();

		// ���ļ�
		void OpenProject(const char* projectName);

		// open node tree
		void openNodeTree(const Echo::String& resPath);

		// open lua file for edit
		void openLuaScript(const Echo::String& resPath);

	protected:
		void closeEvent(QCloseEvent *event);

	public slots:
		// scene operate
		void onNewScene();
		void onSaveScene();
		void onSaveAsScene();

		// project operate
		void onNewAnotherProject();
		void onOpenAnotherProject();
		void onSaveProject();
		void onSaveasProject();
		void onOpenRencentProject();
		void onEditSingletonSettings();

		// sub editor operate
		void onSubEditChanged(const QString& subeditName);

		// game mode
		void onPlayGame();

		// receive msg from game
		void onReadMsgFromGame();

		// game process exit
		void onGameProcessFinished(int id, QProcess::ExitStatus status);

		// open help dialog
		void onOpenHelpDialog();

	protected:
		// open another project
		void openAnotherProject(const Echo::String& fullPathName);

		// update rencent project display
		void updateRencentProjectsDisplay();

		// update setting display
		void updateSettingDisplay();

		// recover edit settings
		void recoverEditSettings();


	protected:
		ResPanel*		m_resPanel;
		NodeTreePanel*	m_scenePanel;
		BottomPanel*	m_bottomPanel;
		DocumentPanel*	m_documentPanel;
		QProcess		m_gameProcess;
		QComboBox*		m_subEditComboBox;
		QMdiArea*		m_midArea;
	};
}