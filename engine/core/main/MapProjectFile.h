#ifndef _ECHO_MAPMapProjectFile_H_
#define _ECHO_MAPMapProjectFile_H_

#include "Engine/Core.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

using namespace rapidxml;

namespace Echo
{
	/**
	 * ��Ŀ�ļ�
	 */
	class MapProjectFile
	{
		typedef std::map<std::string, std::string>  BlocksProps;
		typedef std::map<std::string, BlocksProps>  BlocksPropMap;
	public:
		// �浵
		struct BlockItem
		{
			String		id;
			String		blocktype;		// 
			String		blockname;		// 
			String		lineto;		//

			BlockItem()
			{}
		};

	public:
		MapProjectFile();
		~MapProjectFile();

		// ����
		void load( const char* fileName);

		// ����
		void save( const char* fileName);


	public:
		// blocks��Ϣ
		vector<BlockItem>::type& getBlocks(){return m_blocks;}

		// blocks��Ϣ
		 void setBlocks(vector<BlockItem>::type& info){m_blocks = info;}

		 // blocks������Ϣ
		 BlocksPropMap& getBlockProps(){return m_props;}

		 // blocks������Ϣ
		 void setBlockProps(BlocksPropMap& info){m_props = info;}
	private:
		// ��xml�ж�ȡblocks��Ϣ
		void loadBlocks(xml_node<>* projectNode);

		// �������е�blocks�����xml
		void saveBlocks(xml_document<>& doc, xml_node<>* projectNode);

		// ��xml�ж�ȡblocks������Ϣ
		void loadBlockProps(xml_node<>* projectNode);

		// �������е�blocks���Ա����xml
		void saveBlockProps(xml_document<>& doc, xml_node<>* projectNode);

	private:
		vector<BlockItem>::type		m_blocks;		// �洢�������������Լ�������ϵ
		BlocksPropMap				m_props;		// �洢�������ڵĶ�������
	};
}

#endif