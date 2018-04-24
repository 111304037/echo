#include "GameMainWindow.h"

namespace Game
{
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

		resize(800, 490);
	}

	// ��������
	GameMainWindow::~GameMainWindow()
	{
	}

	// ��ʼ��Ⱦ
	void GameMainWindow::start(const Echo::String& echoProject)
	{
		setWindowTitle(echoProject.c_str());
		m_renderWindow->start(echoProject);
	}
}