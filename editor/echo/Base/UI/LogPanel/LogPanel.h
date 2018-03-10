#pragma once

#include <QDockWidget>
#include <QString>
#include "ui_LogPanel.h"
#include <Engine/Core.h>
#include <engine/core/Util/Log.h>

namespace Studio
{
	//----------------------------------------
	// ��� ������ 2010-12-30
	//----------------------------------------
	class LogPanel : public QDockWidget, public Echo::Log, public Ui_LogPanel
	{
		Q_OBJECT

	public:
		// ���캯��
		LogPanel(QWidget* parent = 0);

		// ��������
		~LogPanel();

		// ����
		void Warning( const char* msg);

		// ����
		void Error( const char* msg);

	public:
		// ������־����
		virtual void setLogName(const Echo::String &logName) {}

		// ��ȡĿ������
		virtual const Echo::String& getName() const;

		// ������־���˼���
		virtual void setLogLevel(LogLevel level) {}

		// ��־���
		virtual void logMessage(LogLevel level, const Echo::String &msg);
		 
		// ��ȡ��־����
		virtual LogLevel getLogLevel() const { return LL_WARNING;}

		virtual void setPath(const Echo::String&) {}

	signals:
		void postMessage(int level, QString);

	private slots:
		void onLogMessage(int level, QString);
		void onClearMessage();

	protected:
		// �ر�
		//void closeEvent(QCloseEvent *event);
	};
}