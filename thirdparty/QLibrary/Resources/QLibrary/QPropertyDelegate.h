#pragma once

#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QEvent>
#include "QPropertyModel.h"
#include <engine/core/util/StringUtil.h>

namespace QT_UI
{
	struct QPropertyWidgetInfo
	{
		typedef bool(*ItemDelegatePaintFun)(QPainter*, const QRect&, const string&);

		Echo::String		 m_type;
		bool				 m_isSupportCustomPaint;
		ItemDelegatePaintFun m_paintFun;
	};
	typedef map<Echo::String, QPropertyWidgetInfo>	QPropertyWidgetInfoMap;

	//------------------------------------------
	// ����ר��(��;����) 2010-03-15  ����
	//------------------------------------------
	class QPropertyDelegate : public QStyledItemDelegate
	{
		Q_OBJECT

	public:
		// ���캯��
		QPropertyDelegate(QPropertyModel* model, QObject *parent = 0);

		// size
		QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

		// ���ƺ���
		void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

		// �����༭���ؼ�
		QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

		// ���ÿؼ�����
		void  setEditorData( QWidget* editor, const QModelIndex& index) const;

		// ����ģ������
		void  setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

		// ���ñ༭��Geometry
		void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& index ) const;

	public:
		// register widget
		void registerWidget(const Echo::String& type, bool isSupportCustomPaint, QPropertyWidgetInfo::ItemDelegatePaintFun paintFun);

	protected:
		bool eventFilter(QObject *object, QEvent *event) Q_DECL_OVERRIDE{ return event->type() == QEvent::FocusOut; }

	private:
		// �Ƿ�ӵ���Զ�����Ⱦ
		bool IsSupportCustomPaint( const Echo::String& widgetType, const QVariant& value) const;

		// �Զ�����Ⱦ
		bool ItemDelegatePaint(  QPainter *painter, const Echo::String& widgetType, const QRect& rect, const QVariant& val) const;
	
	private slots:
		void commitEditor();
	
	private:
		QPropertyModel*			m_model;		// model
		QPropertyWidgetInfoMap	m_widgetInfos;
	};
}
