#include "StringUtil.h"
#include "LogManager.h"
#include "AssertX.h"
#include <stdarg.h>

namespace Echo
{
	LogManager* LogManager::s_instance = NULL;
	// ��ȡ��һʵ��
	LogManager* LogManager::instance()
	{
		if (NULL == s_instance)
		{
			s_instance = EchoNew(LogManager);
		}
		return s_instance;
	}

	// �滻ʵ��
	void LogManager::replaceInstance(LogManager* inst)
	{
		EchoAssert(!s_instance);
		s_instance = inst;
	}

	// ���캯��
	LogManager::LogManager()
		: m_logLevel( Log::LL_INVALID)
	{

	}

	// ��������
	LogManager::~LogManager()
	{
	}

	// �����־
	bool LogManager::addLog( Log* pLog)
	{
		for ( size_t i = 0; i < m_logArray.size(); i++ )
		{
			if ( m_logArray[i] == pLog )
				return false;
		}

		m_logArray.push_back(pLog);

		return true;
	}

	// �������ƻ�ȡ��־
	Log* LogManager::getLog(const String& name) const
							{
		for( size_t i=0; i<m_logArray.size(); i++)
				{
			if( m_logArray[i]->getName() == name)
				return m_logArray[i];
				}

			return NULL;
	}

	// �Ƴ���־
	void LogManager::removeLog( const String &name)
	{
		for( LogArray::iterator it=m_logArray.begin(); it!=m_logArray.end(); it++)
	{
			if( (*it)->getName() == name)
		{
				m_logArray.erase( it);
				return;
		}
	}
	}

	// �Ƴ���־
	void LogManager::removeLog( Log* pLog)
	{
		for( LogArray::iterator it=m_logArray.begin(); it!=m_logArray.end(); it++)
		{
			if( (*it) == pLog)
			{
				m_logArray.erase( it);
				return;
			}
		}
	}

	// �����־
	void LogManager::logMessage(Log::LogLevel level, const char* formats, ...)
	{
        if ( Log::LL_INVALID != m_logLevel )
		{
            if ( level < m_logLevel )
                return;
		}

		if (!m_logArray.empty())
		{
			char szBuffer[4096];
			int bufferLength = sizeof(szBuffer);
			va_list args;
			va_start(args, formats);
			vsnprintf(szBuffer, bufferLength, formats, args);
			va_end(args);

			szBuffer[bufferLength - 1] = 0;

			for (LogArray::iterator it = m_logArray.begin(); it != m_logArray.end(); it++)
			{
				(*it)->logMessage(level, szBuffer);
			}
		}
	}

	void LogManager::logEvent(Log::LogLevel level, const std::wstring& message)
	{
		logMessage(level, "%s", StringUtil::WCS2MBS(message).c_str());
	}

	// ������Ϣ
	void LogManager::error(const char* msg)
	{
		logMessage(Log::LL_ERROR, msg);
	}

	// ����
	void LogManager::warning(const char* msg)
	{
		logMessage(Log::LL_WARNING, msg);
	}

	// ��Ϣ
	void LogManager::info(const char* msg)
	{
		logMessage(Log::LL_INFO, msg);
	}
}
