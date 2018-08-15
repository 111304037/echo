#include <QtGui>
#include "Window.h"
#include <QDateTime>
#include "GameMainWindow.h"
#include <engine/core/main/GameSettings.h>
#include <engine/core/input/input.h>

namespace Game
{
	// ���캯��
	Window::Window(QWidget* parent/* = NULL*/)
		: QWidget(parent)
		, m_app(nullptr)
		, m_timer(nullptr)
	{
		setUpdatesEnabled(false);
		setMouseTracking(true);

		m_app = new App;
	}

	// ��������
	Window::~Window()
	{
	}

	// ��ʼ��Ⱦ
	void Window::start(const Echo::String& echoProject)
	{
		m_app->init((HWND)winId(), echoProject);

		// ���ó�ʼ���
		Echo::i32 thisW = width();
		Echo::i32 thisH = height();
		Echo::i32 mainW = GameMainWindow::instance()->width();
		Echo::i32 mainH = GameMainWindow::instance()->height();
		Echo::i32 aimW = Echo::GameSettings::instance()->getDesignWidth();
		Echo::i32 aimH = Echo::GameSettings::instance()->getDesignHeight();
		Echo::i32 mainNewWidth = mainW + (aimW - thisW);
		Echo::i32 mainNewHeight = mainH + (aimH - thisH);
		GameMainWindow::instance()->resize(mainNewWidth, mainNewHeight);

		// ����
		GameMainWindow::instance()->moveToCenter();

		// ʱ���¼�
		m_timer = new QTimer(this);
		connect(m_timer, SIGNAL(timeout()), this, SLOT(Render()));
		m_timer->start(10);
	}

	// ��Ⱦ����
	void  Window::Render()
	{
		// ����Ƿ�λ�ڴ�����
		checkWindowSize();

		static DWORD lastTime = QDateTime::currentMSecsSinceEpoch();

		// ����delta Time
		DWORD curTime = QDateTime::currentMSecsSinceEpoch();
		DWORD elapsedTime = curTime - lastTime;

		// Call the main render function
		m_app->tick(elapsedTime * 0.001f);

		lastTime = curTime;
	}

	// ���ڴ�С�ı�
	void Window::checkWindowSize()
	{	
		int width = static_cast<int>(this->width());
		int height = static_cast<int>(this->height());

		if (m_dx9Size.width() != width || m_dx9Size.height() != height)
		{
			m_app->onSize(width, height);
			m_dx9Size.setWidth(width);
			m_dx9Size.setHeight(height);
		}
	}

	void Window::SetAspectRatio(const QSize& size)
	{
		m_ratio = size;
	}

	// �����豸
	void  Window::ResetDevice()
	{
	}

	// �������¼�
	void Window::wheelEvent(QWheelEvent * e)
	{
		//auto controller = m_inputController ? m_inputController : m_defaultInputController;
		//controller->wheelEvent(e);
	}

	// ����ƶ��¼�
	void Window::mouseMoveEvent(QMouseEvent* e)
	{
		// UI�¼�
		//if ( m_isLeftButtonDown)
		//{
			// ��ʼ�ϴ�λ��
		//	static QPointF lastPos = e->localPos();

			// ����λ��
		//	QPointF changedPos = e->localPos() - lastPos;

		//	lastPos = e->localPos();
		//}

		//auto controller = m_inputController ? m_inputController : m_defaultInputController;
		//controller->mouseMoveEvent(e);
	}

	// ��갴���¼�
	void Window::mousePressEvent(QMouseEvent* e)
	{
		Echo::Input::instance()->notifyMouseButtonDown(0);
	}

	void Window::mouseDoubleClickEvent(QMouseEvent* e)
	{
		//const auto& controller = m_inputController ? m_inputController : m_defaultInputController; 
		//controller->mouseDoubleClickEvent(e); 
	}

	// ����ͷ��¼�
	void Window::mouseReleaseEvent(QMouseEvent* e)
	{
		Echo::Input::instance()->notifyMouseButtonUp(0);
	}

	// ��갴���¼�
	void Window::keyPressEvent(QKeyEvent* e)
	{
		//auto controller = m_inputController ? m_inputController : m_defaultInputController;
		//controller->keyPressEvent(e);
	}

	// ���̧���¼�
	void Window::keyReleaseEvent(QKeyEvent* e)
	{
		//auto controller = m_inputController ? m_inputController : m_defaultInputController;
		//controller->keyReleaseEvent(e);
	}
}
