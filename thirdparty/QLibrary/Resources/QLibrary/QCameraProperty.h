#pragma once

#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QWidget>
#include <functional>

namespace QT_UI
{
	//-------------------------------------------
	// ��ȡ�������λ�� 
	//-------------------------------------------
	class QCameraPosDir : public QWidget
	{
		Q_OBJECT
	public:
		QCameraPosDir(QWidget* parent = 0);

		// ����·��
		void SetPos( QString text) { m_lineEdit->setText( text);  }

		// ��ȡ·��
		QString GetPos() { return m_lineEdit->text(); }

	private slots:
		// ѡ��·��
		void OnCatchData();

	private:
		QHBoxLayout*	m_horizonLayout;
		QLineEdit*		m_lineEdit;
		QToolButton*	m_toolButton;
	};
}