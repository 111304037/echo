#include "LogPanel.h"
#include <QListWidgetItem>

namespace Studio
{
	// ���캯��
	LogPanel::LogPanel(QWidget* parent/* = 0*/)
		: QDockWidget( parent)
	{
		setupUi(this);

		QObject::connect(this, SIGNAL(postMessage(int, QString)),this, SLOT(onLogMessage(int, QString)), Qt::QueuedConnection);
		QObject::connect(m_clear, SIGNAL(clicked()), this, SLOT(onClearMessage()));
	}

	// ��������
	LogPanel::~LogPanel()
	{
	}

	// ��ȡĿ������
	const Echo::String& LogPanel::getName() const 
	{ 
		static Echo::String logPanel = "LogPanel";

		return logPanel; 
	}

	// ��־���
	void LogPanel::logMessage(LogLevel level, const Echo::String &msg)
	{
		emit postMessage(level, QString::fromStdString(msg));
	}

	void LogPanel::onLogMessage(int level, QString msg)
	{
		switch (level)
		{
		case LL_WARNING: Warning(msg.toStdString().c_str()); break;
		case LL_ERROR:
		case LL_FATAL:	Error(msg.toStdString().c_str());	break;
		default:								break;
		}
	}

	// ����
	void LogPanel::Warning( const char* msg)
	{
		QListWidgetItem* warningItem = new QListWidgetItem( QString::fromLocal8Bit(msg));
		if( warningItem)
		{	
			warningItem->setIcon( QIcon(":/icon/Icon/CheckWarning.png"));
			m_logList->addItem( warningItem);
		}

		// ��ʾ����
		setVisible( true);
	}

	// ����
	void LogPanel::Error( const char* msg)
	{
		QListWidgetItem* errorItem = new QListWidgetItem( QString::fromLocal8Bit(msg));
		if( errorItem)
		{	
			errorItem->setIcon( QIcon(":/icon/Icon/CheckError.png"));
			m_logList->addItem( errorItem);
		}

		// ��ʾ����
		setVisible( true);
	}

	void LogPanel::onClearMessage()
	{
		m_logList->clear();
	}

	// �ر�
	//void LogPanel::closeEvent(QCloseEvent *event)
	//{
	//	m_logList->clear();

	//	QDialog::closeEvent( event);
	//}
}