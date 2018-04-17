#include "ConfigMgr.h"
#include <engine/core/util/PathUtil.h>
#include <ostream>

namespace Studio
{
	// ���캯��
	ConfigMgr::ConfigMgr()
		: m_outPutDir( "" )
		, m_maxRecentProjects( 10)
	{
		m_cfgFile = Echo::PathUtil::GetCurrentDir() + "/cache/echo.cache";
	}

	// ��������
	ConfigMgr::~ConfigMgr()
	{
		saveCfgFile();
	}

	// �������ļ������ڣ�����֮
	bool ConfigMgr::isFileExit( )
	{
		FILE* pFile = fopen( m_cfgFile.c_str(), "rb" );
		if ( NULL == pFile )
		{
			// ·���������ȴ���·��
			Echo::String dir = Echo::PathUtil::GetFileDirPath( m_cfgFile);
			if( !Echo::PathUtil::IsDirExist( dir))
				Echo::PathUtil::CreateDir( dir);

			// ��������ֱ�Ӵ���
			pFile = fopen( m_cfgFile.c_str(), "wb" );
		}

		fclose( pFile );

		return true;
	}

	// ��������
	bool ConfigMgr::loadCfgFile( )
	{
		m_fileName.clear();
		  
		isFileExit();
		try
		{	
			pugi::xml_document doc;
			doc.load_file(m_cfgFile.c_str());

			// root�ڵ�
			pugi::xml_node projectNode = doc.child( "project" );
			if( projectNode)
			{
				loadRecentProject( &projectNode );
				loadOutPutDir( &projectNode );
				loadPropertys( &projectNode);
			}
			else
			{
				
			}
		}
		catch ( ... )
		{
			return false;
		}

		return true;
	}

	// д�������ļ���
	bool ConfigMgr::saveCfgFile( )
	{
		pugi::xml_document doc;
		pugi::xml_node dec = doc.prepend_child(pugi::node_declaration);
		dec.append_attribute("version") = "1.0";
		dec.append_attribute("encoding") = "utf-8";

		pugi::xml_node rootnode= doc.append_child("project" );

		saveData( doc, &rootnode);

		std::ofstream out( m_cfgFile.c_str());
		out << doc;

		return true;
	}

	// ʹĳ��Ŀ�л�����ǰ
	void ConfigMgr::switchProjectToTop(const char* fileName)
	{		
		for (Echo::list<Echo::String>::iterator iter = m_fileName.begin(); iter != m_fileName.end(); ++iter)
		{
			if ((*iter) == fileName)
			{
				m_fileName.erase(iter);
				m_fileName.push_front(fileName);

				return;
			}
		}
	}

	//�����һ������򿪵Ĺ���
	bool ConfigMgr::addRecentProject( const char* fileName )
	{
		if ( isPathExist(fileName) )
		{
			switchProjectToTop(fileName);
			saveCfgFile();

			return true;
		}

		// ��ౣ��ʮ����������ȥ�������
		size_t size = m_fileName.size();
		if ( size >= m_maxRecentProjects )
		{
			m_fileName.pop_back();
		}

		m_fileName.push_front(fileName);
		addToMenu();
		saveCfgFile();

		return true;
	}

	// ��ȡ����򿪵���Ŀ�ļ�
	Echo::String ConfigMgr::getLastOpenProjectFile()
	{
		if( m_fileName.size())
		{
			Echo::list<Echo::String>::type::reverse_iterator it = m_fileName.rbegin();

			return *it;
		}

		return Echo::String();
	}

	// �ж�·���Ƿ����
	bool ConfigMgr::isPathExist( Echo::String path )
	{
		Echo::list<Echo::String>::iterator iter = m_fileName.begin();
		for ( ; iter != m_fileName.end(); ++iter )
		{
			if ( (*iter) == path )
			{
				return true;
			}
		}

		return false;
	}

	// ��������
	void ConfigMgr::saveData( pugi::xml_document& doc, pugi::xml_node* projectNode)
	{
		if( projectNode)
		{
			pugi::xml_node recentNodes = projectNode->append_child("recentProjects" );
			pugi::xml_node outputDir = projectNode->append_child("outputDir" );
			pugi::xml_node propertys = projectNode->append_child("propertys");

			Echo::list<Echo::String>::iterator Iter = m_fileName.begin();
			for( ; Iter != m_fileName.end(); ++Iter )
			{
				pugi::xml_node recentNode = recentNodes.append_child( "recentProject" );
				recentNode.append_attribute("project_value") = (*Iter).c_str();
			}

			outputDir.append_attribute("dir_value") = m_outPutDir.c_str();

			// ������������
			for( std::map<Echo::String, Echo::String>::iterator it=m_propertys.begin(); it!=m_propertys.end(); it++)
			{
				pugi::xml_node propertyNode = propertys.append_child("property");
				propertyNode.append_attribute("property") = it->first.c_str();
				propertyNode.append_attribute("value") = it->second.c_str();
			}
		}
	}

	// ���ļ���ȡ����Ĺ��� 
	void ConfigMgr::loadRecentProject( pugi::xml_node* node)
	{
		if ( node )
		{
			pugi::xml_node recentNodes = node->child( "recentProjects" );
			if ( !recentNodes )
				return ;

			for ( pugi::xml_node recentNode = recentNodes.child("recentProject"); recentNode; recentNode=recentNode.next_sibling("recentProject") )
			{
				Echo::String recentValue= recentNode.attribute( "project_value" ).as_string();
				if ( !isPathExist( recentValue ) )
				{
					m_fileName.push_back(recentValue);
				}
			}
		}
		
		addToMenu();
	}

	// ��ȡ���·��
	void ConfigMgr::loadOutPutDir( pugi::xml_node* node )
	{
		if ( node )
		{
			pugi::xml_node outputDirNode = node->child( "outputDir" );
			if ( outputDirNode )
				m_outPutDir = outputDirNode.attribute( "dir_value").as_string("");
		}
	}

	// ��������ֵ
	void ConfigMgr::loadPropertys( pugi::xml_node* node)
	{
		if ( node )
		{
			pugi::xml_node propertysNode = node->child( "propertys" );
			if( propertysNode)
			{
				for ( pugi::xml_node propertyNode = propertysNode.child("property");  propertyNode; propertyNode=propertyNode.next_sibling("property") )
				{
					Echo::String property = propertyNode.attribute("property").as_string();
					Echo::String value    = propertyNode.attribute( "value").as_string();
					
					m_propertys[property] = value;
				}
			}
		}
	}

	// ��ӵ��˵���
	void ConfigMgr::addToMenu()
	{
		//QMenu* pMenu = NULL;//UI_MainWindow->GetRecentMenu();
		//if ( !pMenu )
		//{
		//	return ;
		//}

		//pMenu->clear();
		//Echo::list<Echo::String>::iterator Iter = m_fileName.begin();
		//for ( ; Iter != m_fileName.end(); ++Iter )
		//{
		//	// ��Ŀ¼������action�У�����MainWindow��ȡ
		//	QAction* pAction = new QAction( pMenu );
		//	pAction->setText( (*Iter).c_str() );
		//	pMenu->addAction( pAction );
		//}
	}

	// �������ƻ�ȡ����ֵ
	Echo::String ConfigMgr::GetValue( const char* property)
	{
		std::map<Echo::String, Echo::String>::iterator it = m_propertys.find( property);
		if( it!=m_propertys.end())
		{
			return it->second;
		}

		return "";
	}

	// ��������
	void ConfigMgr::SetValue( const char* property, const char* value)
	{
		m_propertys[property] = value;

		saveCfgFile();
	}

	void ConfigMgr::getAllRecentProject(Echo::list<Echo::String>::type& projects)
	{
		projects.insert(projects.begin(), m_fileName.begin(), m_fileName.end());
	}
}