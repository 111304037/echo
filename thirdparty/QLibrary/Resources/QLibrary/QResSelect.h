#pragma once

#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QWidget>
#include <functional>
#include "engine/core/util/StringUtil.h"

namespace QT_UI
{
	//-------------------------------------------
	// AssetsSelect �ļ�ѡ��ؼ� 2013-1-22 ����
	//-------------------------------------------
	class QResSelect : public QWidget
	{
		Q_OBJECT

	public:
		using OpenFileDialogFunction = std::function < Echo::String(QWidget*, const char*, const char*, const char*) >;

	public:
		QResSelect(class QPropertyModel* model, QString propertyName, QWidget* parent = 0);

		// ����·��
		void SetPath( QString text) { m_lineEdit->setText( text);  }

		// ���ú�׺����
		void setExts( const char* exts) { m_exts = exts; }

		// �����ļ�����
		void setFiles(const char* files) { m_files = files; }

		// ��ȡ·��
		QString GetPath() { return m_lineEdit->text(); }

		// set open operation
		static void setOpenFileDialogFunction(const OpenFileDialogFunction& func) { m_openFileFunction = func; }

	private slots:
		// ѡ��·��
		void OnSelectPath();

		// edit finished
		void onEditFinished();

	private:
		QString			m_exts;
		QString			m_files;
		QHBoxLayout*	m_horizonLayout;
		QLineEdit*		m_lineEdit;
		QToolButton*	m_toolButton;
		static OpenFileDialogFunction m_openFileFunction;
		QPropertyModel* m_propertyModel;
		QString			m_propertyName;
	};
}