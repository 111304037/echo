#include "GameMainWindow.h"
#include "engine/core/util/PathUtil.h"

namespace Game
{
	static GameMainWindow* g_inst = nullptr;

	// ���캯��
	GameMainWindow::GameMainWindow(QWidget* parent/*=0*/)
		: QMainWindow( parent)
		, m_renderWindow(nullptr)
	{
		setupUi( this);

		m_renderWindow = new Window(this);
		setCentralWidget(m_renderWindow);;

		// ���ر���
		setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

		// ���ò˵����Ͽؼ�
		menubar->setTopLeftCornerIcon(":/icon/Icon/icon.png");

		//resize(800, 490);
		EchoAssert(!g_inst);
		g_inst = this;

	}

	// ��������
	GameMainWindow::~GameMainWindow()
	{
	}

	// get instance
	GameMainWindow* GameMainWindow::instance()
	{
		return g_inst;
	}

	// ��ʼ��Ⱦ
	void GameMainWindow::start(const Echo::String& echoProject)
	{
		Echo::String iconPath = Echo::PathUtil::GetFileDirPath(echoProject);

		setWindowTitle(echoProject.c_str());
		menubar->setTopLeftCornerIcon((iconPath + "icon.png").c_str());

		m_renderWindow->start(echoProject);
	}
}