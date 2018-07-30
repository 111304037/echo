#include "QPropertyDelegate.h"
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QRadioButton>
#include "QFileSelect.h"
#include "QResSelect.h"
#include "QIntEditor.h"
#include "QRealEditor.h"
#include "QCheckBoxEditor.h"
#include "QVector2Editor.h"
#include "QVector3Editor.h"
#include "QColorSelect.h"
#include "QCheckBoxList.h"
#include "QResEditor.h"
#include <QCheckBox>
#include <QComboBox>
#include <QApplication>
#include <engine/core/util/PathUtil.h>

namespace QT_UI
{
	// ���캯��
	QPropertyDelegate::QPropertyDelegate( QPropertyModel* model, QObject *parent/* = 0*/)
		: QStyledItemDelegate(parent)
	{
		m_model = model;

		registerWidget("ColorSelect", true, QColorSelect::ItemDelegatePaint);
		registerWidget("CheckBox", true, QCheckBoxEditor::ItemDelegatePaint);
		registerWidget("ResEdit", true, nullptr);
		registerWidget("AssetsSelect", true, QResSelect::ItemDelegatePaint);
	}

	// register widget
	void QPropertyDelegate::registerWidget(const Echo::String& type, bool isSupportCustomPaint, QPropertyWidgetInfo::ItemDelegatePaintFun paintFun)
	{
		QPropertyWidgetInfo info;
		info.m_type = type;
		info.m_isSupportCustomPaint = isSupportCustomPaint;
		info.m_paintFun = paintFun;

		m_widgetInfos[type] = info;
	}

	// �Ƿ�ӵ���Զ�����Ⱦ
	bool QPropertyDelegate::IsSupportCustomPaint( const Echo::String& widgetType, const QVariant& value) const
	{
		auto& it = m_widgetInfos.find(widgetType);
		if (it != m_widgetInfos.end())
		{
			return it->second.m_isSupportCustomPaint;
		}

		return false;
	}

	// �Զ�����Ⱦ
	bool QPropertyDelegate::ItemDelegatePaint( QPainter *painter, const Echo::String& widgetType, const QRect& rect, const QVariant& val) const
	{
		auto& it = m_widgetInfos.find(widgetType);
		if (it != m_widgetInfos.end() && it->second.m_paintFun)
		{
			return (*(it->second.m_paintFun))(painter, rect, val.toString().toStdString());
		}

		return false;
	}

	QSize QPropertyDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		Echo::String widgetType;
		QString propertyName = index.model()->data(index, Qt::DisplayPropertyRole).toString();
		QVariant userData = index.model()->data(index, Qt::UserRole);
		Echo::String  userDataStr = userData.toString().toStdString().c_str();
		Echo::StringArray userDatas = Echo::StringUtil::Split(userDataStr, ",");
		if (!userDatas.empty())
			widgetType = userDatas[0];

		QVariant value;
		m_model->findValue(value, propertyName);

		QSize size = QStyledItemDelegate::sizeHint( option, index);
		if (widgetType == "AssetsSelect")
		{
			Echo::String ext = userDatas.size() >= 2 ? userDatas[1] : "";
			if (Echo::StringUtil::Equal(ext, ".png", false))
				size.setHeight(2 * size.height());
		}

		return size;
	}

	// ���ƺ���
	void QPropertyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		Echo::String widgetType;
		QString propertyName = index.model()->data( index, Qt::DisplayPropertyRole).toString();
		QVariant userData = index.model()->data( index, Qt::UserRole);
		Echo::String  userDataStr = userData.toString().toStdString().c_str();
		Echo::StringArray userDatas = Echo::StringUtil::Split(userDataStr, ",");
		if(!userDatas.empty())
			widgetType = userDatas[0];

		QVariant value;
		if (m_model->findValue(value, propertyName) && IsSupportCustomPaint(widgetType, value))
		{
			if (!ItemDelegatePaint(painter, widgetType, option.rect, value))
				QStyledItemDelegate::paint(painter, option, index);
		}
		else
		{
			QStyledItemDelegate::paint(painter, option, index);
		}
	}

	void QPropertyDelegate::commitEditor()
	{
		QWidget* ed = qobject_cast<QWidget*>(sender());
		if (ed)
		{
			emit commitData(ed);
		}
	}

	// createEditor, returns an editor widget
	QWidget* QPropertyDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		QVariant     userData		= index.model()->data( index, Qt::UserRole);
		Echo::String userData1	    = QString::fromUtf8( userData.toByteArray()).toStdString().c_str();
		Echo::StringArray userDatas = Echo::StringUtil::Split( userData1, ",");
		
		QString propertyName = index.model()->data(index, Qt::DisplayPropertyRole).toString();
		// δָ���ؼ�,����NULL
		if( !userDatas.size())
			return NULL;

		QString  widgetType = userDatas[0].c_str();

		if (widgetType == "spinBox")
		{
			return new QSpinBox(parent);
		}
		else if (widgetType == "doubleSpinBox")
		{
			return new QDoubleSpinBox(parent);
		}
		else if (widgetType == "LineEdit")
		{
			return new QLineEdit(parent);
		}
		else if (widgetType == "FileSelect")
		{
			return new QFileSelect(parent);
		}
		else if( widgetType == "AssetsSelect")
		{
			QResSelect* widget = new QResSelect(m_model, propertyName, userDatas.size() > 1 ? userDatas[1].c_str() : nullptr, userDatas.size() > 2 ? userDatas[2].c_str() : nullptr, parent);

			return widget;
		}
		else if (widgetType == "ResEdit")
		{
			QResEditor* widget = new QResEditor(m_model, propertyName, userDatas.size() > 1 ? userDatas[1].c_str() : nullptr, parent);
			return widget;
		}
		else if (widgetType == "ColorSelect")
		{
			QColorSelect* widget = new QColorSelect(parent);
			QObject::connect(widget, SIGNAL(clicked()), this, SLOT(commitEditor()));
			return widget;
		}
		else if (widgetType == "RadioButton")
		{
			return new QRadioButton(parent);
		}
		else if( widgetType == "CheckBox")
		{
			QCheckBox* widget = new QCheckBox(parent);
			QObject::connect(widget, SIGNAL(stateChanged(int)), this, SLOT(commitEditor()));
			return widget;
		}
		else if (widgetType == "CheckBoxList")
		{
			return new QCheckBoxList(parent);
		}
		else if( widgetType == "ComboBox")
		{
			QComboBox* pWidget = new QComboBox( parent);
			for( size_t i=1; i<userDatas.size(); i++)
			{
				QString item = QString::fromUtf8(userDatas[i].c_str());
				pWidget->addItem(item);
			}
			return pWidget;
		}
		else if (widgetType == "Int")
		{
			return new QIntEditor( m_model, propertyName, parent);
		}
		else if (widgetType == "Real")
		{
			return new QRealEditor(m_model, propertyName, parent);
		}
		else if (widgetType == "Vector2")
		{
			return new QVector2Editor(m_model, propertyName, parent);
		}
		else if(widgetType == "Vector3")
		{
			return new QVector3Editor(m_model, propertyName, parent);
		}
		else if (widgetType == "BoneSelectComboBox")
		{
			QComboBox* pWidget = new QComboBox(parent);
			m_model->ThrowSelfDefineSig("BoneSelectComboBoxEdit", propertyName, reinterpret_cast<qintptr>(pWidget));
			return pWidget;
		}
		else if (widgetType == "default")
		{
			return QStyledItemDelegate::createEditor(parent, option, index);
		}

		return NULL;
	}

	// ���±༭������
	void  QPropertyDelegate::setEditorData( QWidget* editor, const QModelIndex& index) const
	{
		QVariant     userData		= index.model()->data( index, Qt::UserRole);
		Echo::String userData1	    = userData.toString().toStdString().c_str();
		Echo::StringArray userDatas = Echo::StringUtil::Split( userData1, ",");
		QString  widgetType = userDatas[0].c_str();

		QVariant value;
		QString propertyName = index.model()->data( index, Qt::DisplayPropertyRole).toString();
		if( m_model->findValue( value, propertyName))
		{
			if( widgetType == "spinBox")
			{
				QSpinBox* widget = qobject_cast<QSpinBox*>(editor);
				widget->setRange(-65535, 65535);
				widget->setValue( value.toInt());
			}
			else if( widgetType == "doubleSpinBox")
			{
				QDoubleSpinBox* widget = qobject_cast<QDoubleSpinBox*>(editor);
				widget->setRange(-65535.f, 65535.f); 
				widget->setValue( value.toFloat());
				widget->setDecimals(3);
				widget->setSingleStep(0.1f); 
			}
			else if( widgetType == "LineEdit")
			{
				QLineEdit* widget = qobject_cast<QLineEdit*>(editor);
				widget->setText( value.toString());
			}
			else if( widgetType == "FileSelect")
			{
				QFileSelect* widget = qobject_cast<QFileSelect*>(editor);
				widget->SetPath( value.toString());
			}
			else if( widgetType == "AssetsSelect")
			{
				QResSelect* widget = qobject_cast<QResSelect*>(editor);
				widget->SetPath( value.toString().toStdString().c_str());
			}
			else if (widgetType == "ResEdit")
			{
				QResEditor* widget = qobject_cast<QResEditor*>(editor);
				widget->SetId(value.toString());
			}
			else if( widgetType == "ColorSelect")
			{
				QColorSelect* widget = qobject_cast<QColorSelect*>( editor);
				widget->SetColor( value.toString().toStdString());
				widget->OnSelectColor();
				m_model->setValue(propertyName, widget->GetColor().c_str());
			}
			else if( widgetType == "CheckBox")
			{
				QCheckBox* widget = qobject_cast<QCheckBox*>(editor);
				widget->setChecked( value.toBool());
				m_model->setValue(propertyName, widget->isChecked());
			}
			else if( widgetType == "CheckBoxList")
			{
				QCheckBoxList* widget = qobject_cast<QCheckBoxList*>(editor);
				widget->SetValue( value.toString());
			}
			else if( widgetType == "ComboBox")
			{
				QComboBox* widget = qobject_cast<QComboBox*>(editor);
				widget->setCurrentText( value.toString());
			}
			else if (widgetType == "Int")
			{
				QIntEditor* widget = qobject_cast<QIntEditor*>(editor);
				widget->setValue(value.toString());
			}
			else if (widgetType == "Real")
			{
				QRealEditor* widget = qobject_cast<QRealEditor*>(editor);
				widget->setValue(value.toString());
			}
			else if (widgetType == "Vector2")
			{
				QVector2Editor* widget = qobject_cast<QVector2Editor*>(editor);
				widget->setValue(value.toString());
			}
			else if(widgetType == "Vector3")
			{
				QVector3Editor* widget = qobject_cast<QVector3Editor*>(editor);
				widget->setValue(value.toString());
			}
			else if (widgetType == "BoneSelectComboBox")
			{
				QComboBox* widget = qobject_cast<QComboBox*>(editor);
				if (value.toString().isEmpty())
				{
					widget->setCurrentIndex(-1);
				}
				widget->setCurrentText(value.toString());
			}
			else if( widgetType == "default")
			{
				QStyledItemDelegate::setEditorData(editor, index);
			}
		}
	}

	// ����ģ������
	void  QPropertyDelegate::setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
	{
		QVariant     userData		= index.model()->data( index, Qt::UserRole);
		Echo::String userData1	    = userData.toString().toStdString().c_str();
		Echo::StringArray userDatas = Echo::StringUtil::Split( userData1, ",");
		QString  widgetType = userDatas[0].c_str();
		
		QVariant value;
		QString propertyName = index.model()->data( index, Qt::DisplayPropertyRole).toString();
		if( m_model->findValue( value, propertyName))
		{
			if( widgetType == "spinBox")
			{
				QSpinBox* widget = qobject_cast<QSpinBox*>(editor);
				this->m_model->setValue( propertyName, widget->value());
			}
			else if( widgetType == "doubleSpinBox")
			{
				QDoubleSpinBox* widget = qobject_cast<QDoubleSpinBox*>(editor);
				m_model->setValue( propertyName, widget->value());
			}
			else if( widgetType == "LineEdit")
			{
				QLineEdit* widget = qobject_cast<QLineEdit*>(editor);
				m_model->setValue( propertyName, widget->text());
			}
			else if( widgetType == "FileSelect")
			{
				QFileSelect* widget = qobject_cast<QFileSelect*>(editor);
				m_model->setValue( propertyName, widget->GetPath());
			}
			else if( widgetType == "AssetsSelect")
			{
				QResSelect* widget = qobject_cast<QResSelect*>(editor);
				m_model->setValue( propertyName, widget->GetPath());
			}
			else if (widgetType == "ResEdit")
			{
				QResEditor* widget = qobject_cast<QResEditor*>(editor);
				m_model->setValue(propertyName, widget->GetId());
			}
			else if( widgetType == "ColorSelect")
			{
				QColorSelect* widget = qobject_cast<QColorSelect*>(editor);
				m_model->setValue( propertyName, widget->GetColor().c_str());
			}
			else if( widgetType == "CheckBox")
			{
				QCheckBox* widget = qobject_cast<QCheckBox*>(editor);
				m_model->setValue( propertyName, widget->isChecked());
			}
			else if( widgetType=="CheckBoxList")
			{
				QCheckBoxList* widget = qobject_cast<QCheckBoxList*>(editor);
				m_model->setValue( propertyName, widget->GetValue());
			}
			else if( widgetType=="ComboBox")
			{
				QComboBox* widget = qobject_cast<QComboBox*>( editor);
				m_model->setValue( propertyName, widget->currentText());
			}
			else if (widgetType == "BoneSelectComboBox")
			{
				QComboBox* widget = qobject_cast<QComboBox*>(editor);
				m_model->setValue(propertyName, widget->currentText());
			}
			else if( widgetType == "default")
			{
				QStyledItemDelegate::setModelData(editor, model, index);
			}
		}
	}

	// ���ñ༭��Geometry
	void QPropertyDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& index ) const
	{
		QCheckBox* widget = qobject_cast<QCheckBox*>(editor);
		if( widget)
		{
			QRect rect( option.rect.left()+3, option.rect.top(), option.rect.width()-3, option.rect.height());
			editor->setGeometry( rect);
			editor->setFocus();
		}
		else
		{
			editor->setGeometry(option.rect);
			editor->setFocus();
		}
	}
}