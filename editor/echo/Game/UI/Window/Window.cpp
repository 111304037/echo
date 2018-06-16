#include <QtGui>
#include "Window.h"
#include <QDateTime>
#include "GameMainWindow.h"

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
		Echo::i32 aimW = Echo::Engine::instance()->getProjectFile()->getWindowWidth();
		Echo::i32 aimH = Echo::Engine::instance()->getProjectFile()->getWindowHeight();
		Echo::i32 mainNewWidth = mainW + (aimW - thisW);
		Echo::i32 mainNewHeight = mainH + (aimH - thisH);
		GameMainWindow::instance()->resize(mainNewWidth, mainNewHeight);

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

		//auto controller = m_inputController ? m_inputController : m_defaultInputController;
		//auto pos = controller->mousePosition();
		//auto button = controller->pressedMouseButton();
		auto elapsed = elapsedTime;

		// Call the main render function
		m_app->tick(elapsedTime);

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
		//if (!hasFocus())
		//{
		//	setFocus();
		//}

		// UI�¼�
		//if ( e->button()==Qt::LeftButton)
		//{
		//	m_isLeftButtonDown = true;
		//}

		// ע��������¼�
		//auto controller = m_inputController ? m_inputController : m_defaultInputController;
		//controller->mousePressEvent(e);
	}

	void Window::mouseDoubleClickEvent(QMouseEvent* e)
	{
		//const auto& controller = m_inputController ? m_inputController : m_defaultInputController; 
		//controller->mouseDoubleClickEvent(e); 
	}

	// ����ͷ��¼�
	void Window::mouseReleaseEvent(QMouseEvent* e)
	{
		// UI�¼�
		//if (e->button() == Qt::LeftButton)
		//{
		//	m_isLeftButtonDown = false;
		//}

		//auto controller = m_inputController ? m_inputController : m_defaultInputController;
		//controller->mouseReleaseEvent(e);
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
