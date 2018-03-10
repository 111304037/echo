#pragma once

#include <QMenuBar>
#include <QMainWindow>

namespace QT_UI
{
	class QMenuBarEx : public QMenuBar
	{
		Q_OBJECT

	public:
		QMenuBarEx(QWidget* parent);
		~QMenuBarEx(){}

		// �������Ͻ�ͼ��
		void setTopLeftCornerIcon(const char* icon);

	protected:
		// ����ƶ��¼�
		virtual void mouseMoveEvent(QMouseEvent *e);

		// ���˫���¼�
		virtual void mouseDoubleClickEvent(QMouseEvent *event);

		// ��갴���¼�
		virtual void mousePressEvent(QMouseEvent *event);

		// ����ͷ��¼�
		virtual void mouseReleaseEvent(QMouseEvent *event);

	protected slots:
		// ��С��
		void onMinus();

		// ���
		void onMaxmized();

	private:
		QPointF			m_lastPos;
		QMainWindow*	m_parent;
	};
}