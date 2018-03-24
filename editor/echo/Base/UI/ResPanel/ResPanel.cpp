#include <QtGui>
#include <QDateTime>
#include <QMenuBar>
#include "ResPanel.h"
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

		//QStringList titleLable;
		//titleLable << "Assets";
		//m_dirModel->setHorizontalHeaderLabels(titleLable);

		//const Echo::ResourceGroupManager::ArchiveList& archiveList = EchoResourceManager->getArchiveList();
		//for (Echo::Archive* archive : archiveList)
		{
		//	m_dirModel->SetRootPath(archive->getName().c_str(), "none", m_resDirView, NULL/*m_proxyModel*/);
		//	m_dirModel->Refresh();
		}
	}

	// ��������
	ResPanel::~ResPanel()
	{

	}
}