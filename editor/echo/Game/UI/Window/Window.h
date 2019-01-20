#pragma once

#include <QWidget>
#include <QMenu>
#include "App.h"

namespace Game
{
	//-----------------------------------
	// ��ʾ����
	//-----------------------------------
	class Window : public QWidget
	{
		Q_OBJECT

	public:
		Window( QWidget* parent = NULL);
		virtual ~Window();

		// ��ʼ��Ⱦ
		void start(const Echo::String& echoProject);

		// aspect ratio
		void SetAspectRatio(const QSize& size);
		QSize GetAspectRatio() const { return m_ratio; }

		// �������¼�
		virtual void wheelEvent( QWheelEvent * e);

		// ����ƶ��¼�
		virtual void mouseMoveEvent( QMouseEvent* e);

		// ��갴���¼�
		virtual void mousePressEvent( QMouseEvent* e);

		// ���˫���¼�
		virtual void mouseDoubleClickEvent(QMouseEvent* e);

		// ����ͷ��¼�
		virtual void mouseReleaseEvent( QMouseEvent* e);

		// ��갴���¼�
		virtual void keyPressEvent( QKeyEvent* e);

		// ���̧���¼�
		virtual void keyReleaseEvent( QKeyEvent* e);

	private slots:
		// ��Ⱦ
		void  Render();

		// �����豸
		void  ResetDevice();

	private:
		// ���ڴ�С�ı�
		void checkWindowSize();

	private:
		QSize					m_dx9Size;
		QTimer*					m_timer = nullptr;
		QSize					m_ratio;
		App*					m_app = nullptr;
	};
}
