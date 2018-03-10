#pragma once

#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QWidget>
#include <functional>

namespace QT_UI
{
	//-------------------------------------------
	// AssetsSelect �ļ�ѡ��ؼ� 2013-1-22 ����
	//-------------------------------------------
	class QAssetsSelect : public QWidget
	{
		Q_OBJECT
	public:
		using OpenFileDialogFunction = std::function < QString(QWidget*, const char*, const char*, const char*) > ;
		QAssetsSelect( QWidget* parent = 0);

		// ����·��
		void SetPath( QString text) { m_lineEdit->setText( text);  }

		// ���ú�׺����
		void setExts( const char* exts) { m_exts = exts; }

		// �����ļ�����
		void setFiles(const char* files) { m_files = files; }

		// ��ȡ·��
		QString GetPath() { return m_lineEdit->text(); }

	//	void setOpenFileDialogFunction(const OpenFileDialogFunction& func) { m_openFileFunction = func; }

	public:
		static OpenFileDialogFunction m_openFileFunction;
	private slots:
		// ѡ��·��
		void OnSelectPath();

	private:
		QString			m_exts;
		QString			m_files;
		QHBoxLayout*	m_horizonLayout;
		QLineEdit*		m_lineEdit;
		QToolButton*	m_toolButton;
	};
}