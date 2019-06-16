#pragma once

#include <map>
#include <functional>
#include <QStandardItem>
#include <QStandardItemModel>
#include <thirdparty/pugixml/pugixml.hpp>

using namespace std;

namespace QT_UI
{
	typedef std::function<void(const char*)> SubEditCb; 

	//------------------------------------------
	// �QPropertyModel 2010-03-15  captain
	//------------------------------------------
	class QPropertyModel : public QStandardItemModel
	{
		Q_OBJECT

		typedef map<QString, QVariant> AttribList;
	public:
		// ���캯��
		QPropertyModel(const char* fileName, bool isEnableGB2312 = false, QObject* parent = 0);
		~QPropertyModel();

		// ����ID
		void setID(const QString& id) { m_id = id; }

		// ��ȡID
		const QString& getID() { return m_id; }

		// ��������
		void  setupModelData(pugi::xml_document* cfg);

		// �������Բ�������
		bool findValue(QVariant& value, const QString& propertyName);

		// ��������
		void setValue(const QString& propertyName, QVariant value);

		// ��������
		static bool setValue(const QString& modelName, const QString& propertyName, QVariant value);

		// ����text����item
		QStandardItem* findItemByText(const char* text);

		// ����ӽ��
		QStandardItem* addChildItem(QStandardItem* parent, const char* text, bool bold, int row, int col, const char* widget, const char* property);

		// �����ӽ��
		void removeChildItem(QStandardItem* parent);

		// �׳��Զ���Signal
		void ThrowSelfDefineSig(QString sigName, QString propertyName, QVariant value);

		// ���ûص�;
		void set2ndSubEditorCallback(SubEditCb cb);

		// �����ص�;
		void on2ndSubEditorCallback(const char* fileName); 

	private:
		// ���������ļ�
		void ParseCfg();

		// �ݹ����
		void addChildItem(pugi::xml_node* pFirstChildElement, QStandardItem* pParentItem);

		// ����Լ��
		void updateConstraint(const QString& propertyName, QVariant value);

		// ��ȡԼ��
		QMap<QString, QVariant> constraintCondition(pugi::xml_node* itemNode);

	signals:
		// ���ݱ䶯
		void Signal_ValueChanged(const QString& propertyName, QVariant value);
		void Signal_ValueChanged(const QString& propertyName, QVariant value, int row, int column);

		//�׳��Զ�����ź�,��Ҫ��Ϊ�˽��һЩ�����޸ĵ����漰������������߼�,����Ϊ�˽���,�׸������߼�����
		void Signal_ThrowSelfDefineSig(const QString& singalStr, const QString& propertyNamej,QVariant value);

	private:
		QString						m_id;					// ��ʶ
		pugi::xml_document*			m_cfg;
		AttribList					m_valueList;
		vector<QStandardItem*>		m_standItems;			// ����
		map<QString, QStandardItem*>m_propertyStandItmes;	// ���Զ�Ӧ��StandItem
		bool						m_isEnableGB2312;

		SubEditCb					m_2ndSubEditorCallback; 
	};
}
