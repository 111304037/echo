#include "MainWindow.h"
#include "Studio.h"
#include "LogPanel.h"
#include "ResPanel.h"
#include "NodeTreePanel.h"
#include <QFileDialog>
#include <QDesktopservices>
#include <QShortcut>
#include "TimelinePanel.h"
#include "DebuggerPanel.h"

namespace Studio
{
	// ���캯��
	MainWindow::MainWindow(QMainWindow* parent/*=0*/)
		: QMainWindow( parent)
		, m_resPanel(nullptr)
		, m_timelinePanel(nullptr)
		, m_debuggerPanel(nullptr)
	{
		setupUi( this);

		// ���ر���
		setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

		// ���ò˵����Ͽؼ�
		menubar->setTopLeftCornerIcon(":/icon/Icon/icon.png");

		// connect signal slot
		QObject::connect(m_actionSave, SIGNAL(triggered(bool)), this, SLOT(onSaveProject()));
	}

	// ��������
	MainWindow::~MainWindow()
	{
	}

	// ����Ŀʱ����
	void MainWindow::onOpenProject()
	{
		m_resPanel = EchoNew(ResPanel(this));
		m_scenePanel = EchoNew(NodeTreePanel(this));
		m_timelinePanel = EchoNew(TimelinePanel(this));
		m_debuggerPanel = EchoNew(DebuggerPanel(this));

		setCentralWidget(AStudio::Instance()->getRenderWindow());

		this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
		this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

		this->addDockWidget(Qt::LeftDockWidgetArea, m_resPanel);
		this->addDockWidget(Qt::RightDockWidgetArea, m_scenePanel);
		
		this->addDockWidget(Qt::BottomDockWidgetArea, AStudio::Instance()->getLogPanel());
		this->addDockWidget(Qt::BottomDockWidgetArea, m_debuggerPanel);
		this->addDockWidget(Qt::BottomDockWidgetArea, m_timelinePanel);

		this->tabifyDockWidget(AStudio::Instance()->getLogPanel(), m_debuggerPanel);
		this->tabifyDockWidget(m_debuggerPanel, m_timelinePanel);

		m_resPanel->onOpenProject();
	}

	// �����ļ�
	void MainWindow::onSaveProject()
	{
		int  a = 10;
	}

	// ���ļ�
	void MainWindow::OpenProject(const char* projectName)
	{
		AStudio::Instance()->OpenProject(projectName);

		// ��ʼ����Ⱦ����
		AStudio::Instance()->getRenderWindow();
	}

	void MainWindow::closeEvent(QCloseEvent *event)
	{
		AStudio::Instance()->getLogPanel()->close();
	}
}