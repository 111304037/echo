#include "DebuggerPanel.h"
#include <QListWidgetItem>

namespace Studio
{
	// ���캯��
	DebuggerPanel::DebuggerPanel(QWidget* parent/* = 0*/)
		: QDockWidget( parent)
	{
		setupUi(this);
	}

	// ��������
	DebuggerPanel::~DebuggerPanel()
	{
	}
}