#include <QApplication>
#include <QDesktopWidget>
#include "About.h"
#include "engine/core/util/PathUtil.h"

namespace Studio
{
	AboutWindow::AboutWindow(QWidget* parent/*=0*/)
		: QMainWindow( parent)
	{
		setupUi( this);

		// ���ر���
		setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

		// ���ò˵����Ͽؼ�
		menubar->setTopLeftCornerIcon(":/icon/Icon/icon.png");

	}

	AboutWindow::~AboutWindow()
	{
	}
}