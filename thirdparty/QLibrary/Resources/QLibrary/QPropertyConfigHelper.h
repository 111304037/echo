#pragma once

#include <QtWidgets/QTreeView>
#include <engine/core/Util/Array.hpp>
#include <engine/core/Math/Math.h>

namespace QT_UI
{
	// �ؼ�����
	enum WidgetType
	{
		WT_None,		// �գ����ɱ༭
		WT_Int,
		WT_Vector3,
		WT_LineEdit,
		WT_CheckBox,
		WT_DoubleSpinBox,
		WT_ColorSelect,
		WT_ComboBox,
		WT_AssetsSelect,
		WT_2ND_EDITOR,			// ��ɾ��
	};

	/**
	 * ����������ʾ
	 */
	struct QProperty;
	class QPropertyConfigHelper
	{
	public:
		QPropertyConfigHelper();
		~QPropertyConfigHelper();

		// ���ñ���
		void setHeader( const char* header0, const char* header1, const char* header2=nullptr, const char* header3=nullptr);

		// ��ʼĳ�˵���
		void beginMenu( const char* text);

		// ����ĳ�˵���
		void endMenu();

		// ��ӱ༭��
		void addItem(const char* propertyName, float value, WidgetType widget, const char* widgetParams = nullptr);
		void addItem(const char* propertyName, Echo::ui32 value, WidgetType widget, const char* widgetParams = nullptr);
		void addItem(const char* propertyName, bool value, WidgetType widget, const char* widgetParams = nullptr);
		void addItem(const char* propertyName, const Echo::Vector3& value, WidgetType widget, const char* widgetParams = nullptr);
		void addItem(const char* propertyName, const Echo::String& value, WidgetType widget, const char* widgetParams = nullptr);
		void addItem_c2(const char* propertyName, const Echo::String& value, WidgetType widget, const char* widgetParams = nullptr);
		void addItem_c3(const char* propertyName, const Echo::String& value, WidgetType widget, const char* widgetParams = nullptr);

		template<typename T>
		void addItem(const char* propertyName, T value, WidgetType widget, const char* widgetParams = nullptr)
		{
			static_assert(false, "ambiguous overload for function `addItem`.");
		}

		// ��ȡ���
		Echo::String getResult();

		// Ӧ�õ�view,
		void applyTo(const Echo::String& id, QTreeView* treeView, const QObject* receiver, const char* memeber, bool clear = true);

		// ��ȡ���Կؼ�
		QT_UI::QProperty* getQProperty() { return m_property; }

		void clear() { m_result.clear(); }
	private:
		// UI��ʾ��ʽ
		Echo::String FormatUI(WidgetType widget, const char* widgetParams);

	private:
		Echo::String						m_id;
		Echo::i32							m_curDepth;		// ��ǰ���
		Echo::array<Echo::ui32, 25>			m_idxs;			// ��Ӧ����к�
		Echo::String						m_result;		// ���
		Echo::String						m_lastResult;	// �ϴ�����
		Echo::map<QString, QVariant>::type	m_values;		// ���г�ʼֵ
		QT_UI::QProperty*					m_property;		// ����
		QMetaObject::Connection				m_signalConn;	// ��Ϣ����
	};
}