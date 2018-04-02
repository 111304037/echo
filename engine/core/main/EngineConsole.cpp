#include "EngineConsole.h"
#include "engine/core/render/render/Viewport.h"
#include "engine/core/render/render/Renderer.h"

namespace Echo
{
	// ִ��ָ��
	void EngineConsole::exec( const String& cmd, String& output)
	{
		StringArray argv = StringUtil::Split( cmd, " ");
		parse( argv, output);
	}

	// ����
	void EngineConsole::parse( const StringArray& argv, String& output)
	{
		// ��Ⱦ����
		if( argv[0] == "renderqueue")
		{
		}
		else if( argv[0] == "get")
		{
			parseGet( argv, output);
		}
		else
		{
			output += StringUtil::Format( "unknown command [%s] \n", argv[0].c_str());
		}
	}

	// ��ȡ����
	void EngineConsole::parseGet( const StringArray& argv, String& output)
	{
		std::string param0 = argv.size()>1 ? argv[1].c_str() : "";
		if( param0 == "rendererinfo")
		{
			Viewport viewPort;
			Renderer::instance()->getViewportReal( viewPort);

			output += StringUtil::Format("\n%-15s[%4d,%4d,%4d,%4d]", "ViewPort:", viewPort.getLeft(), viewPort.getTop(), Renderer::instance()->getScreenWidth(), Renderer::instance()->getScreenHeight());
			output += StringUtil::Format( "\n%-15s[%4d,%4d,%4d,%4d]\n", "ViewPortReal:", viewPort.getLeft(), viewPort.getTop(), viewPort.getWidth(), viewPort.getHeight());
		}
		else
		{
			output += StringUtil::Format( "unknown param [%s] \n", param0.c_str());
		}
	}
}