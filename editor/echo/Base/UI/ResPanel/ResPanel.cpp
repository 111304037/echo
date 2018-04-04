#include <QtGui>
#include <QDateTime>
#include <QMenuBar>
#include "ResPanel.h"
#include "engine/core/util/PathUtil.h"
#include "engine/core/main/Root.h"

namespace Studio
{
	// ���캯��
	ResPanel::ResPanel( QWidget* parent/*=0*/)
		: QDockWidget( parent)
	{
		setupUi( this);

		// Ŀ¼���ͽṹ
		m_dirModel = new QT_UI::QDirectoryModel();
		m_dirModel->SetIcon("root", QIcon(":/icon/Icon/root.png"));
		m_dirModel->SetIcon("filter", QIcon(":/icon/Icon/folder_close.png"));
		m_dirModel->SetIcon("filterexpend", QIcon(":/icon/Icon/folder_open.png"));
		m_resDirView->setModel(m_dirModel);
		m_dirModel->Clean();

		QObject::connect(m_dirModel, SIGNAL(FileSelected(const char*)), this, SLOT(onSelectDir(const char*)));

		m_previewHelper = new QT_UI::QPreviewHelper(m_listView);
	}

	// ��������
	ResPanel::~ResPanel()
	{

	}

	// call when open project
	void ResPanel::onOpenProject()
	{
		m_dirModel->clear();

		QStringList titleLable;
		titleLable << "Res://";
		m_dirModel->setHorizontalHeaderLabels(titleLable);

		m_dirModel->SetRootPath(Echo::Root::instance()->getResPath().c_str(), "none", m_resDirView, NULL);
		m_dirModel->Refresh();

		onSelectDir(Echo::Root::instance()->getResPath().c_str());
	}

	// ѡ���ļ���
	void ResPanel::onSelectDir(const char* dir)
	{
		m_previewHelper->clear();
		m_previewHelper->setPath(dir);
	}
}