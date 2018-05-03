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
	class LogPanel : public QWidget, public Echo::Log, public Ui_LogPanel
	{
		Q_OBJECT

	public:
		// ���캯��
		LogPanel(QWidget* parent = 0);

		// ��������
		~LogPanel();

		// �����Ϣ
		void OutMsg( int level,const char* msg, const char* icon);

	public:
		// ��־���
		virtual void logMessage(LogLevel level, const Echo::String &msg);
		 

		virtual void setPath(const Echo::String&) {}

	signals:
		void postMessage(int level, QString);

	private slots:
		void onLogMessage(int level, QString);
		void onClearMessage();


	private:
		Echo::ui32			m_sameMessageNum;
		Echo::String		m_lastMessage;
		int			m_lastLevel;
	};
}