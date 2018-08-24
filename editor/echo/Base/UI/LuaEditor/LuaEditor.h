#pragma once

#include <QMainWindow>
#include "ui_LuaEditor.h"
#include "QProperty.hpp"
#include "LuaSyntaxHighLighter.h"
#include <engine/core/script/lua/LuaScript.h>

namespace Studio
{
	class LuaEditor : public QDockWidget, public Ui_ScriptEditorPanel
	{
		Q_OBJECT

	public:
		LuaEditor(QWidget* parent = 0);
		~LuaEditor();

		// open lua file
		void open( const Echo::String& tex);

		// get current edit lua file path
		const Echo::String& getCurrentLuaFilePath();

	protected slots:
		// ����
		void save();

		// ���ݱ��޸�
		void onTextChanged();

		// ���±�����ʾ
		void updateTitle();

	protected:
		// size hint
		virtual QSize sizeHint() const override;

	private:
		Echo::LuaResPtr				m_luaRes;
		LuaSyntaxHighLighter*		m_luaSyntaxHighLighter;
	};
}