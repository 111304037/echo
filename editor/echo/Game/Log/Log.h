#pragma once

#include <engine/core/log/LogManager.h>

namespace Game
{
	class GameLog : public Echo::Log
	{
	public:
		GameLog(const Echo::String& name);

		/** ��־��� */
		virtual void logMessage(LogLevel level, const Echo::String &msg);
	};
}