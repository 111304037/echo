#include <QApplication>
#include <QTextCodec>
#include <QFileInfo>
#include <QDir>
#include <QScopedPointer>
#include <QtPlugin>
#include <QSplashScreen>
#include <Engine/Core.h>
#include "CMDLine.h"
#include "Studio.h"
#include <shellapi.h>
#include <windows.h>
#include <QTime>

int main( int argc, char* argv[])
{
	// set directory path
	QString filePath = QFileInfo(argv[0]).path();
	if (filePath.length() <= 1)
	{
		char buffer[1024] = {}; 
		::GetModuleFileNameA(NULL, buffer, 1024); 
		filePath = QFileInfo(buffer).path();
	}
	else
	{
		filePath = QFileInfo(argv[0]).path(); 
	}

	QDir::setCurrent( filePath);
	QApplication::setLibraryPaths(QStringList() << QApplication::libraryPaths() << QDir::currentPath().append("/plugins/Qt"));

	const auto& list = QApplication::libraryPaths(); 

	QApplication app( argc, argv);
	app.setAttribute(Qt::AA_NativeWindows);

	// ���ñ��뷽ʽ
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	QTextCodec::setCodecForLocale(codec);

	// ���ʹ��launch image
	int idx = QTime(0, 0, 0).secsTo(QTime::currentTime()) % 4;
	Echo::String iconLocation = Echo::StringUtil::Format(":/icon/Icon/Launch/launch-%d.png", idx);

	QSplashScreen splash;
	splash.setPixmap(QPixmap(iconLocation.c_str()));
	splash.show();
	splash.showMessage(QString::fromLocal8Bit( "Echo (32 bit OpenGLES) Copyright @ 2018-2019 B-Lab"), Qt::AlignLeft | Qt::AlignBottom, Qt::white);

	// ���ý�����
 	QFile qssFile( ":/Qss/Qss/Ps.qss");
 	qssFile.open( QFile::ReadOnly);
 	if( qssFile.isOpen())
 	{
 		QString qss = QLatin1String( qssFile.readAll());
 		app.setStyleSheet( qss);
 		
 		qssFile.close();
 	}

	// ���������У����������档
	bool isCmdModel = Echo::CMDLine::Parser(argc, argv, &splash);
	if ( !isCmdModel )
	{
		// ִ��
		app.exec( );
	}

	// �ͷ���Դ
	Echo::CMDLine::Release();
}
