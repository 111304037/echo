#include "QMenubarEx.h"
#include <QToolButton>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QApplication>

namespace QT_UI
{
	// ���캯��
	QMenuBarEx::QMenuBarEx(QWidget* parent)
		: QMenuBar(parent)
	{
		m_parent = dynamic_cast<QMainWindow*>(parent);

		// ���ò˵����Ͽؼ�
		QWidget* menuTopRightButton = new QWidget(this);
		QToolButton* minusButton = new QToolButton(menuTopRightButton);
		minusButton->setIcon(QIcon(":/icon/Icon/minus.png"));
		minusButton->setStyleSheet("background-color: #00535353;");

		QToolButton* minimizeButton = new QToolButton(menuTopRightButton);
		minimizeButton->setIcon(QIcon(":/icon/Icon/minimize.png"));
		minimizeButton->setStyleSheet("background-color: #00535353;");

		QToolButton* closeButton = new QToolButton(menuTopRightButton);
		closeButton->setIcon(QIcon(":/icon/Icon/close.png"));
		closeButton->setStyleSheet("background-color: #00535353;");

		QHBoxLayout* horizontalLayout = new QHBoxLayout(menuTopRightButton);
		horizontalLayout->setSpacing(0);
		horizontalLayout->setObjectName(QStringLiteral("horizontalLayoutTR"));
		horizontalLayout->setContentsMargins(0, 0, 0, 0);
		horizontalLayout->addWidget(minusButton);
		horizontalLayout->addWidget(minimizeButton);
		horizontalLayout->addWidget(closeButton);

		setCornerWidget(menuTopRightButton, Qt::TopRightCorner);
		
		// �¼�����
		QObject::connect(minusButton, SIGNAL(clicked()), this, SLOT(onMinus()));
		QObject::connect(minimizeButton, SIGNAL(clicked()), this, SLOT(onMaxmized()));
		QObject::connect(closeButton, SIGNAL(clicked()), parent, SLOT(close()));
	}

	// �������Ͻ�ͼ��
	void QMenuBarEx::setTopLeftCornerIcon(const char* icon)
	{
		QToolButton* menuTopLeftButton = new QToolButton(this);
		menuTopLeftButton->setIcon(QIcon(icon));
		menuTopLeftButton->setIconSize(QSize(22, 22));
		menuTopLeftButton->setStyleSheet("background-color: #00535353;");
		setCornerWidget(menuTopLeftButton, Qt::TopLeftCorner);
	}

	// ��С��
	void QMenuBarEx::onMinus()
	{
		m_parent->setWindowState(Qt::WindowMinimized);
	}

	// ���
	void QMenuBarEx::onMaxmized()
	{
		if (m_parent->windowState()==Qt::WindowMaximized)
			m_parent->setWindowState(Qt::WindowNoState);
		else
			m_parent->setWindowState(Qt::WindowMaximized);
	}

	// ����ƶ��¼�
	void QMenuBarEx::mouseMoveEvent(QMouseEvent *e)
	{
		if (e->buttons()==Qt::LeftButton)
		{
			// ����ǰΪ���״̬�����޸�ΪĬ��״̬(��������λ��)
// 			if (m_parent->windowState() == Qt::WindowMaximized)
// 			{ 
// 				m_parent->setWindowState(Qt::WindowNoState);
// 				m_lastPos = e->screenPos();
// 			}

			// ����λ��
			QPointF changedPos = e->screenPos() - m_lastPos;
			QPoint  destPos = m_parent->pos() + QPoint(changedPos.x(), changedPos.y());
			m_parent->move(destPos);

			m_lastPos = e->screenPos();
		}
	}

	// ���˫���¼�
	void QMenuBarEx::mouseDoubleClickEvent(QMouseEvent *event)
	{
		// ���λ�ڲ˵���ʱ����Ӧ˫���¼���
		int posX = event->screenPos().x();
		int posY = event->screenPos().y();
		QWidget* widget = QApplication::widgetAt(posX, posY);
		QMenu* menu = qobject_cast<QMenu*>(widget);
		if (!menu)
			onMaxmized();
	}

	// ��갴���¼�
	void QMenuBarEx::mousePressEvent(QMouseEvent *event)
	{
		QMenuBar::mousePressEvent(event);

		if (event->buttons() == Qt::LeftButton)
		{
			m_lastPos = event->screenPos();
		}
	}

	// ����ͷ��¼�
	void QMenuBarEx::mouseReleaseEvent(QMouseEvent *event)
	{
		QMenuBar::mouseReleaseEvent(event);
	}

	// redefine paintEvent
	void QMenuBarEx::paintEvent(QPaintEvent* event)
	{
		QMenuBar::paintEvent(event);
	}
}