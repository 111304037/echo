#pragma once

#include "Log.h"

namespace Echo
{
	/** 
	 * ��־ϵͳ 
	 */
	class LogManager
	{
		typedef vector<Log*>::type	LogArray;

	public:
		LogManager();
		~LogManager();

		// ��ȡ��һʵ��
		static LogManager* instance();

		// �滻ʵ��
		static void replaceInstance(LogManager* inst);

		// �����־
		bool addLog( Log* pLog);

		// �������ƻ�ȡ��־
		Log* getLog(const String& name) const;

		// ���������Ƴ���־
		void removeLog( const String& name);

		// ������־ָ���Ƴ���־
		void removeLog( Log* pLog);

		// �Ƴ�������־
		void removeAllLog() { m_logArray.clear(); }

		// ������־����
		void setLogLeve( Log::LogLevel level) { m_logLevel = level; }

		// ��ȡ��־����
		Log::LogLevel getLogLevel() { return m_logLevel; }

		// �����־
		void logMessage(Log::LogLevel level, const char* formats, ...);

		// ����¼�
		void logEvent(Log::LogLevel level, const std::wstring& message);

	public:
		// lua
		void error(const char* msg);
		void warning(const char* msg);
		void info(const char* msg);

		static LogManager* s_instance;

	protected:
		Log::LogLevel	m_logLevel;		// ��־����
		LogArray		m_logArray;		// A list of all the logs the manager can access
	};
}

#define EchoLogDebug(formats, ...)		Echo::LogManager::instance()->logMessage(Echo::Log::LL_DEBUG, formats, ##__VA_ARGS__);
#define EchoLogInfo(formats, ...)		Echo::LogManager::instance()->logMessage(Echo::Log::LL_INFO, formats, ##__VA_ARGS__);
#define EchoLogWarning(formats, ...)	Echo::LogManager::instance()->logMessage(Echo::Log::LL_WARNING, formats, ##__VA_ARGS__);
#define EchoLogError(formats, ...)		Echo::LogManager::instance()->logMessage(Echo::Log::LL_ERROR, formats, ##__VA_ARGS__);
#define EchoLogFatal(formats, ...)		Echo::LogManager::instance()->logMessage(Echo::Log::LL_FATAL, formats, ##__VA_ARGS__);
