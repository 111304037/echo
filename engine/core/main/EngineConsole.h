#pragma once

#include "Engine/Core.h"

namespace Echo
{
	/**
	 * EngineConsole �������̨ 
	 */
	class EngineConsole
	{
	public:
		// ִ��ָ��
		void exec( const String& cmd, String& output);

	private:
		// ����
		void parse( const StringArray& argv, String& output);

		//��Ⱦ������ش���
		void parseRenderQueue( const StringArray& argv, String& output);

		// ��ȡ����
		void parseGet( const StringArray& argv, String& output);
	};
}