#include <QtGui>
#include "Window.h"
#include <QDateTime>

namespace Game
{
	// ���캯��
	Window::Window(QWidget* parent/* = NULL*/)
		: QWidget(parent)
		//, m_mouseMenu(NULL)
		//, m_timer(NULL)
		//, m_inputController(NULL)
		//, m_defaultInputController(NULL)
		//, m_isLeftButtonDown(false)
	{
		//setAttribute(Qt::WA_NativeWindow);
		//setUpdatesEnabled(false);
		//setMouseTracking(true);
	}

	// ��������
	Window::~Window()
	{
		//delete m_timer; m_timer = NULL;

		//EchoEngine::Instance()->Release();
		//delete m_defaultInputController;
	}

	// ��ʼ��Ⱦ
	void Window::BeginRender()
	{
		//EchoEngine::Instance()->Initialize((HWND)this->winId());

		//if (!m_defaultInputController)
		//	m_defaultInputController = new DefaultInputController; 

		// ʱ���¼�
		//m_timer = new QTimer(this);
		//connect(m_timer, SIGNAL(timeout()), this, SLOT(Render()));
		//m_timer->start(10);
	}

	// ��Ⱦ����
	void  Window::Render()
	{
		// ����Ƿ�λ�ڴ�����
		//ResizeWindow();

		//static DWORD lastTime = QDateTime::currentMSecsSinceEpoch();

		//// ����delta Time
		//DWORD curTime = QDateTime::currentMSecsSinceEpoch();
		//DWORD elapsedTime = curTime - lastTime;

		//auto controller = m_inputController ? m_inputController : m_defaultInputController;
		//auto pos = controller->mousePosition();
		//auto button = controller->pressedMouseButton();
		//auto elapsed = elapsedTime * 0.001f;

		//// Call the main render function
		////EchoEngine::Instance()->Render(elapsedTime, this->isVisible());

		//lastTime = curTime;
	}

	// ���ڴ�С�ı�
	void Window::ResizeWindow()
	{
		
		//int width = static_cast<int>(this->width());
		//int height = static_cast<int>(this->height());

		//if (m_dx9Size.width() != width || m_dx9Size.height() != height)
		//{
		//	EchoEngine::Instance()->Resize(width, height);
		//	m_dx9Size.setWidth(width);
		//	m_dx9Size.setHeight(height);
		//}
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
