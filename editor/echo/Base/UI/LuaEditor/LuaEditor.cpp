#include "ProjectMgr.h"
#include <QFileDialog>
#include "MainWindow.h"
#include "LuaEditor.h"
#include "Studio.h"
#include <engine/core/util/PathUtil.h>

namespace Studio
{
	// ���캯��
	LuaEditor::LuaEditor(QWidget* parent)
		: QDockWidget(parent)
	{
		setupUi( this);

		// ����
		QFont font;
		font.setFamily("Courier");
		font.setStyleHint(QFont::Monospace);
		font.setFixedPitch(true);
		font.setPointSize(10);
		font.setBold(true);
		m_textEditor->setFont(font);

		// Tab Space
		const int tabStop = 4;  // 4 characters
		QFontMetrics metrics(font);
		m_textEditor->setTabStopWidth(tabStop * metrics.width(' '));

		// �﷨������
		m_luaSyntaxHighLighter = new LuaSyntaxHighLighter(m_textEditor->document());

		// ��Ϣ����
		QObject::connect(m_textEditor, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
	}

	// ��������
	LuaEditor::~LuaEditor()
	{
	}

	// size hint
	QSize LuaEditor::sizeHint() const
	{
		float width = MainWindow::instance()->width() * 0.7f;
		float height = MainWindow::instance()->height() * 0.65f;
		return QSize(width, height);
	}

	// ��ʾ����
	void LuaEditor::open(const Echo::String& fullPath)
	{
		m_luaRes = ECHO_DOWN_CAST<Echo::LuaScript*>(Echo::Res::get(fullPath));
		if (m_luaRes)
		{
			m_textEditor->setPlainText(m_luaRes->getSrc());

			// 0.���ı���
			updateTitle();
		}
	}

	// get current edit lua file path
	const Echo::String& LuaEditor::getCurrentLuaFilePath()
	{
		if (m_luaRes)
		{
			return m_luaRes->getPath();
		}

		return Echo::StringUtil::BLANK;
	}

	// ���ݱ��޸�
	void LuaEditor::onTextChanged()
	{
		if (m_luaRes)
		{
			m_luaRes->setSrc( m_textEditor->toPlainText().toStdString().c_str());

			updateTitle();
		}
	}

	// ����
	void LuaEditor::save()
	{
		if (m_luaRes)
		{
			m_luaRes->save();

			// 0.���ı���
			updateTitle();
		}
	}

	// ���±�����ʾ
	void LuaEditor::updateTitle()
	{
		if (m_luaRes)
		{
			if (m_luaRes->getSrc() == m_textEditor->toPlainText())
			{
				Echo::String fileName = m_luaRes->getPath();
				setWindowTitle( fileName.c_str());
			}
			else
			{
				Echo::String fileName = m_luaRes->getPath();
				setWindowTitle((fileName + "*").c_str());
			}
		}
		else
		{
			setWindowTitle("LuaEditor");
		}
	}
}