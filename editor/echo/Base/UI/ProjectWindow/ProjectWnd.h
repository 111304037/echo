#pragma once

#include "ui_ProjectWindow.h"
#include "QPreviewWidget.h"
#include <engine/core/util/StringUtil.h>

class QShowEvent;
namespace Studio
{
	/**
	 * ��Ŀ������
	 */
	class ProjectWnd : public QMainWindow, public Ui_ProjectWindow
	{
		Q_OBJECT

	public:
		ProjectWnd(QMainWindow* parent = 0);
		~ProjectWnd();

		void addRecentProject(const char* projecet);

		// ��ʾ���пɸ��°汾
		void showAllUpdateableVersion();

		// new project file
		Echo::String newProject();

	protected:
		virtual void showEvent(QShowEvent* event) override;

	private slots:
		// ������Ŀ
		void openNewProject(int index);

		void onDoubleClicked(const QString& name);

		void onClicked(const QString& name);

		// �������
		void onDownloadNewVersion(QListWidgetItem* item);

	private:
		QT_UI::QPreviewWidget* m_previewerWidget; 
	};
}
