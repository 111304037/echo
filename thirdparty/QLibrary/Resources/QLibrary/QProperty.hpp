#pragma once

#include "QPropertyModel.h"
#include "QPropertyDelegate.h"
#include "QPropertyConfigHelper.h"
#include <QSortFilterProxyModel>

namespace QT_UI
{
	//----------------------------------
	// QProperty 2013-1-21 ����
	// ��¼Model,Delegate��
	//----------------------------------
	struct QProperty
	{
		QPropertyModel*			m_model;
		QPropertyDelegate*		m_delegate;

		// ���캯��
		QProperty(const char* config, QWidget* parent = 0, bool isGB2312 = false)
		{
			// ����
			m_model			= EchoNew( QPropertyModel(config, isGB2312, parent));
			m_delegate		= EchoNew( QPropertyDelegate( m_model));
		}

		// ��������
		~QProperty()
		{
			EchoSafeDelete( m_model, QPropertyModel);
			EchoSafeDelete( m_delegate, QPropertyDelegate);
		}
	};
}