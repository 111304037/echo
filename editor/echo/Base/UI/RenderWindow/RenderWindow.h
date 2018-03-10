#pragma once

#include <QWidget>
#include <QMenu>
#include "IRenderWindowInputController.h"

namespace Studio
{
	//-----------------------------------
	// Dx9MdiArea dx��ʾ����
	//-----------------------------------
	class RenderWindow : public QWidget
	{
		Q_OBJECT

	public:
		RenderWindow( QWidget* parent = NULL);
		virtual ~RenderWindow();

		// ��ʼ��Ⱦ
		void BeginRender();

		// ���ڴ�С�ı�
		void ResizeWindow();

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

		// ���������
		void setInputController( IRWInputController* controller) { m_inputController = controller; }

		// ��ȡ���������
		IRWInputController* getInputController(){return m_inputController? m_inputController:m_defaultInputController; }

	private slots:
		// ��Ⱦ
		void  Render();

		// �����豸
		void  ResetDevice();

	private:
		QSize					m_dx9Size;					// �������С
		QTimer*					m_timer;					// ��ʱ��
		QMenu*					m_mouseMenu;				// ���˵�
		IRWInputController*		m_inputController;			// ���������
		IRWInputController*		m_defaultInputController;	// Ĭ�����������
		bool					m_isLeftButtonDown;			// �������Ƿ���
		QSize					m_ratio;
	};
}