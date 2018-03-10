#include "CMDLine.h"
#include <string>
#include <QDir>
#include <QSplashScreen>
#include "Studio.h"
#include "EchoEngine.h"

namespace Echo
{
	Studio::AStudio* g_astudio = NULL;

	// ���������
	bool CMDLine::Parser(int argc, char* argv[], QSplashScreen* splash)
	{
		if ( argc > 1 )
		{	
			StringArray sargv;
			for( int i=1; i<argc; i++)
				sargv.push_back( argv[i]);

			//�����б༭��
			g_astudio = new Studio::AStudio(" ");
			g_astudio->setAppPath(QDir::currentPath().toStdString().c_str());

			return true;
		}
		else
		{
			// �����༭��
			TIME_PROFILE
			(
				g_astudio = new Studio::AStudio;
				g_astudio->setAppPath(QDir::currentPath().toStdString().c_str());
			)

			// ��������
			TIME_PROFILE
			(
				g_astudio->getConfigMgr()->loadCfgFile();
			)
			
			TIME_PROFILE
			(
				g_astudio->Start();
			)

			// ��ʾ������
			TIME_PROFILE
			(
				ThreadSleepByMilliSecond(1000);
				g_astudio->getProjectWindow()->show();
				splash->finish(g_astudio->getProjectWindow());
			)
		}

		return false;
	}

	// �ͷ�
	void CMDLine::Release()
	{
		delete g_astudio;
	}
}
