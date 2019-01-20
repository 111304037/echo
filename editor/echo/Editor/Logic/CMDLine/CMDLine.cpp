#include "CMDLine.h"
#include <string>
#include <QDir>
#include <QSplashScreen>
#include <QApplication>
#include <QTextCodec>
#include <QTime>
#include "Studio.h"
#include "EchoEngine.h"
#include "Window.h"
#include "ProjectWnd.h"
#include "GameMainWindow.h"
#include <engine/core/util/PathUtil.h>
#include <engine/core/util/TimeProfiler.h>

namespace Echo
{
	Studio::AStudio* g_astudio = NULL;

	// ���������
	bool CMDLine::Parser(int argc, char* argv[])
	{
		if ( argc > 1 )
		{	
			StringArray sargv;
			for( int i=1; i<argc; i++)
				sargv.push_back( argv[i]);

			if (sargv[0] == "play")
			{
				GameMode gameMode;
				gameMode.exec(argc, argv);
			}
			else if ( argc==2)
			{
				EditOpenMode openMode;
				openMode.exec(argc, argv);
			}

			return true;
		}
		else
		{
			EditorMode editorMode;
			editorMode.exec(argc, argv);
		}

		return false;
	}

	// exec command
	bool EditorMode::exec(int argc, char* argv[])
	{
		QApplication app(argc, argv);
		app.setAttribute(Qt::AA_NativeWindows);

		// ���ñ��뷽ʽ
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QTextCodec::setCodecForLocale(codec);

		// launch image
		//int idx = QTime(0, 0, 0).secsTo(QTime::currentTime()) % 4;
		//Echo::String iconLocation = Echo::StringUtil::Format(":/icon/Icon/Launch/launch-%d.png", idx);

		//QSplashScreen splash;
		//splash.setPixmap(QPixmap(iconLocation.c_str()));
		//splash.show();
		//splash.showMessage(QString::fromLocal8Bit("Echo (32 bit OpenGLES) Copyright @ 2018-2019 B-Lab"), Qt::AlignLeft | Qt::AlignBottom, Qt::white);

		// qss
		QFile qssFile(":/Qss/Qss/Ps.qss");
		qssFile.open(QFile::ReadOnly);
		if (qssFile.isOpen())
		{
			QString qss = QLatin1String(qssFile.readAll());
			app.setStyleSheet(qss);

			qssFile.close();
		}

		// �����༭��
		TIME_PROFILE
		(
			g_astudio = Studio::AStudio::instance();
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
			//ThreadSleepByMilliSecond(1000);
			g_astudio->getProjectWindow()->show();
			//splash.finish(g_astudio->getProjectWindow());
		)

		// run application
		app.exec();

		delete g_astudio;

		return true;
	}

	// exec command
	bool GameMode::exec(int argc, char* argv[])
	{
		Echo::String type = argv[1];
		if (type != "play")
			return false;

		Echo::String projectFile = argv[2];

		QApplication app(argc, argv);
		app.setAttribute(Qt::AA_NativeWindows);

		// set qss
		QFile qssFile(":/Qss/Qss/Ps.qss");
		qssFile.open(QFile::ReadOnly);
		if (qssFile.isOpen())
		{
			QString qss = QLatin1String(qssFile.readAll());
			app.setStyleSheet(qss);
			qssFile.close();
		}

		// start window
		Game::GameMainWindow mainWindow;
		mainWindow.show();
		mainWindow.start(projectFile);

		app.exec();

		return true;
	}

	// exec command
	bool EditOpenMode::exec(int argc, char* argv[])
	{
		QApplication app(argc, argv);
		app.setAttribute(Qt::AA_NativeWindows);

		// ���ñ��뷽ʽ
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QTextCodec::setCodecForLocale(codec);

		// ���ý�����
		QFile qssFile(":/Qss/Qss/Ps.qss");
		qssFile.open(QFile::ReadOnly);
		if (qssFile.isOpen())
		{
			QString qss = QLatin1String(qssFile.readAll());
			app.setStyleSheet(qss);

			qssFile.close();
		}

		// �����༭��
		TIME_PROFILE
		(
			g_astudio = Studio::AStudio::instance();
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
			Echo::String projectFile = argv[1];
			Echo::PathUtil::FormatPath(projectFile, false);
			g_astudio->getProjectWindow()->openProject( projectFile);
		)

		// ִ��
		app.exec();

		delete g_astudio;

		return true;
	}
}
