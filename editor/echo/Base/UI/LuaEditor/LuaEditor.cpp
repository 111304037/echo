#include "ProjectMgr.h"
#include <QFileDialog>
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
		//QObject::connect(m_actionSave, SIGNAL(triggered()), this, SLOT(save()));
		QObject::connect(m_textEditor, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
	}

	// ��������
	LuaEditor::~LuaEditor()
	{
	}

	// ��ʾ����
	void LuaEditor::open(const Echo::String& fullPath)
	{
		m_fullPath = "";

		// 1.��ʾLua
		QFile file(fullPath.c_str());
		if (file.open(QFile::ReadOnly | QFile::Text))
		{
			m_fullPath = fullPath;
			m_origContent = file.readAll();
			m_textEditor->setPlainText(m_origContent);

			file.close();
		}

		// 0.���ı���
		updateTitle();
	}

	// ���ݱ��޸�
	void LuaEditor::onTextChanged()
	{
		// 0.���ı���
		updateTitle();
	}

	// ����
	void LuaEditor::save()
	{
		if (!m_fullPath.empty())
		{
			QFile file(m_fullPath.c_str());
			if (file.open(QFile::WriteOnly | QFile::Text))
			{
				m_origContent = m_textEditor->toPlainText();
				file.write(m_origContent.toUtf8());

				file.flush();
				file.close();
			}

			// 0.���ı���
			updateTitle();
		}
	}

	// ���±�����ʾ
	void LuaEditor::updateTitle()
	{
		if (m_origContent == m_textEditor->toPlainText())
		{
			Echo::String fileName = Echo::PathUtil::GetPureFilename(m_fullPath);
			this->setWindowTitle( fileName.empty() ? "LuaEditor" : fileName.c_str());
		}
		else
		{
			Echo::String fileName = Echo::PathUtil::GetPureFilename(m_fullPath);
			this->setWindowTitle(fileName.empty() ? "LuaEditor" :(fileName+"*").c_str());
		}
	}
}