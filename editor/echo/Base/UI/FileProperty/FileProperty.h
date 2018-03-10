#pragma once

#include <vector>
#include <QDialog>
#include <engine/core/Util/StringUtil.h>
#include "ui_FileProperty.h"

namespace Studio
{
	/**
	* �ļ�������ʾ�Ի���
	*/
	class FilePropertyDialog : public QDialog
	{
		Q_OBJECT

	public:
		FilePropertyDialog(QWidget* parent = 0);
		virtual ~FilePropertyDialog();

		// �����ļ���
		void setFile( const char* fileName);

	private:
		Ui_FileProperty* m_ui;		// ui��ʼ����
	};
}