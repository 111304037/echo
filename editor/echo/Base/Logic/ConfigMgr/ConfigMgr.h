#pragma once

#include <map>
#include <Engine/Core.h>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

namespace Studio
{
	/**
	 * �༭������
	 */
	class ConfigMgr
	{
	public:
		ConfigMgr();
		~ConfigMgr();

	public:
		// ��ȡ�����ļ�
		bool loadCfgFile( );

		// ���һ������򿪵���Ŀ
		bool addRecentProject( const char* fileName);

		// ʹĳ��Ŀ�л�����ǰ
		void switchProjectToTop(const char* fileName);

		// ���������ļ�
		bool saveCfgFile( );

		// �ļ��Ƿ���ڣ�����������򴴽���
		bool isFileExit( );

		// ��ǰ�����Ƿ��Ѿ�����
		bool isPathExist( Echo::String path );

		// �������·��
		void setOutPutDir( const char* path ) { m_outPutDir = path; }

		// ��ȡ���·��
		const char* getOutPutDir( ) { return m_outPutDir.c_str(); }

		// ��ȡ����򿪵���Ŀ�ļ�
		Echo::String getLastOpenProjectFile();

		// �������ƻ�ȡ����ֵ
		Echo::String GetValue( const char* property);

		// ��������
		void SetValue( const char* property, const char* value);

		// ��ȡ��������򿪵Ĺ���
		void getAllRecentProject(Echo::list<Echo::String>::type& projects);

	private:
		// ���浽�ļ�
		void saveData( rapidxml::xml_document<>& doc, rapidxml::xml_node<>* projectNode );

		// ��ȡ����򿪵Ĺ���
		void loadRecentProject( rapidxml::xml_node<>* node);

		// ��ȡ��Դ���·��
		void loadOutPutDir( rapidxml::xml_node<>* node);

		// ��������ֵ
		void loadPropertys( rapidxml::xml_node<>* node);

		// ��ӵ��˵���
		void addToMenu( );

	private:
		Echo::String						m_cfgFile;			// �����ļ�
		Echo::list<Echo::String>::type		m_fileName;			// ����򿪵Ĺ���
		Echo::String						m_outPutDir;		// ��Դת�����·��
		size_t								m_maxRecentProjects;// ��¼�����
		std::map<Echo::String, Echo::String>m_propertys;		// ����
	};
}