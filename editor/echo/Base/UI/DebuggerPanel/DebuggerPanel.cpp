#include "DebuggerPanel.h"
#include <QListWidgetItem>

namespace Studio
{
	// ���캯��
	DebuggerPanel::DebuggerPanel(QWidget* parent/* = 0*/)
		: QWidget( parent)
	{
		setupUi(this);
	}

	// ��������
	DebuggerPanel::~DebuggerPanel()
	{
	}
}