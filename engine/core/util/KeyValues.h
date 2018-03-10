#pragma once

#include "StringUtil.h"
#include "engine/core/Math/Vector3.h"

namespace Echo
{
	// ��ֵ��
	struct KeyValues
	{
		map<String, String>::type m_keyValues;

		// �������ַ���
		void exportToStr(String& str);

		// �����ַ�����ʼ��
		void parseFromStr(const String& str);

		// ��ȡֵ
		const String& getValueStr(const String& key)const;
		bool getValueBool(const String& key, bool defaultValue) const;
		i32 getValueI32(const String& key, i32 defaultValue)const;
		float getValueFloat(const String& key, float defaultValue) const;
		const Vector3& getValueVec3(const String& key, const Vector3& defaultValue)const;

		// ����ֵ
		void setValue(const String& key, const String& value);
		void setValue(const String& key, bool value);
		void setValue(const String& key, i32 value);
		void setValue(const String& key, float value);
		void setValue(const String& key, const Vector3& value);

		// ���
		void clear() { m_keyValues.clear(); }

		// ��ȡ��ֵ������
		i32 getSize() { return (i32)m_keyValues.size(); }
	};
}