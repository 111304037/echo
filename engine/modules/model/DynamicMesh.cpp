#include <Engine/modules/Model/DynamicMesh.h>
#include "DynamicSubMesh.h"
#include <Rapidxml/rapidxml.hpp>
#include <Rapidxml/rapidxml_helper.hpp>
#include "SubMesh.h"

namespace Echo
{
	// ���캯��
	DynamicMesh::DynamicMesh(const String& name)
		: Mesh(name)
	{
		m_isSkinned = false;
	}

	// ��������
	DynamicMesh::~DynamicMesh()
	{
	}

	// ����ʵ��
	bool DynamicMesh::prepareImpl(DataStream* stream)
	{
		MemoryReader reader(mName.c_str());
		if (reader.getSize())
		{
			rapidxml::xml_document<> doc;
			doc.parse<0>(reader.getData<char*>());
			rapidxml::xml_node<>* node = doc.first_node("DynamicMesh");
			node = node->first_node("DynamicSubMesh");
			Echo::String type = rapidxml_helper::get_string(node->first_attribute("type"), "default");
			Echo::String params = rapidxml_helper::get_string(node->first_attribute("params"), "");



			DynamicSubMesh* subMesh = nullptr;
			if (type == "default")
			{
				subMesh = EchoNew(DynamicSubMesh(this));

				// �����ʽ
				DynamicSubMesh::VertexDefine vertDefine;
				vertDefine.m_isUseNormal = rapidxml_helper::parsebool(node->first_attribute("isusenormal"), false);
				vertDefine.m_isUseDiffuseUV = rapidxml_helper::parsebool(node->first_attribute("isusediffuseuv"), false);
				vertDefine.m_isUseVertexColor = rapidxml_helper::parsebool(node->first_attribute("isusecolor"), false);
				subMesh->setVertDefine(vertDefine);
			}
			else if (type == "linestrip")
			{
				subMesh = EchoNew(DynamicSubMeshLineStrip(this));
			}
			else if (type == "sphere")
			{
				subMesh = EchoNew(DynamicSubMeshSphere(this));
			}

			// �����ģ��
			if (subMesh)
			{
				subMesh->set(params);
				m_subMeshs.push_back(subMesh);
				m_isValid = true;

				return true;
			}
			else
			{
				EchoLogWarning("no dynamic mesh generate.");
				return false;
			}
		}
		else
		{
			EchoLogWarning("no dynamic mesh file exist.");
			return false;
		}
	}

	void DynamicMesh::unprepareImpl()
	{

	}

	// ����
	void DynamicMesh::update(ui32 delta, const Echo::Vector3& vpos, const Echo::Quaternion& qrotate)
	{
		// ������ģ��
		for (SubMesh* subMesh : m_subMeshs)
		{
			subMesh->update(delta);
		}
	}
}
