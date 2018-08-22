#pragma once

#include <QMainWindow>
#include "ui_LuaEditor.h"
#include "QProperty.hpp"
#include "LuaSyntaxHighLighter.h"

namespace Studio
{
	class LuaEditor : public QDockWidget, public Ui_ScriptEditorPanel
	{
		Q_OBJECT

	public:
		LuaEditor(QWidget* parent = 0);
		~LuaEditor();

		// ��ʾ����
		void open( const Echo::String& tex);

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
		Echo::String				m_fullPath;					// ȫ·��
		QString						m_origContent;				// �ļ�������
		LuaSyntaxHighLighter*		m_luaSyntaxHighLighter;
	};
}