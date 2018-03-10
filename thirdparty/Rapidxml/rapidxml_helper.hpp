#pragma once

#include "rapidxml.hpp"
#include "engine/core/Util/StringUtil.h"
#include <engine/core/Math/Vector3.h>
#include <engine/core/render/render/Color.h>

namespace Echo
{
	/**
	  rapidxml ��д����
	 */
	struct rapidxml_helper
	{
		rapidxml::xml_document<>* m_doc;

		// ���캯��
		rapidxml_helper(rapidxml::xml_document<>* doc) : m_doc(doc) {}

		// ��ȡֵ
		static const char* get_string(const rapidxml::xml_attribute<>* att, const char* default_val = "")
		{
			return att ? att->value() : default_val;
		}

		// ����bool
		static bool parsebool(const rapidxml::xml_attribute<>* att, bool default_val = false)
		{
			return att ? StringUtil::ParseBool(att->value()) : default_val;
		}

		// ����int
		static i32 parseI32(const rapidxml::xml_attribute<>* att, i32 default_val = 1.f)
		{
			return att ? StringUtil::ParseI32(att->value()) : default_val;
		}

		// ����float
		static float parsefloat(const rapidxml::xml_attribute<>* att, float default_val = 1.f)
		{
			return att ? StringUtil::ParseFloat(att->value()) : default_val;
		}

		// ����Vector3
		static Vector3 parseVec3(const rapidxml::xml_attribute<>* att, Vector3 default_val = Vector3::ZERO)
		{
			return att ? StringUtil::ParseVec3(att->value()) : default_val;
		}

		// ������ɫ
		static Color parseColor(const rapidxml::xml_attribute<>* att, Color default_val = Color::BLACK)
		{
			return att ? StringUtil::ParseColor(att->value()) : default_val;
		}

		// boolת��Ϊ�ַ���
		template<typename T> const char* tostr( T& val)
		{
			return m_doc->allocate_string(StringUtil::ToString(val).c_str());
		}
	};
}