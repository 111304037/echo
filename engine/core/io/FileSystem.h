#pragma once

#include "DataStream.h"

namespace Echo
{
	/**
	 * �ļ�ϵͳ�浵
	 */
	class FileSystem 
	{
	public:
		FileSystem();
		~FileSystem();

		// ����·��
		void setPath(const String& path, const String& prefix);

		// ���ļ�
		DataStream* open(const String& filename);

		// �Ƿ��Сд����
		bool isCaseSensitive() const;

		// �Ƿ����
		bool isExist(const String& filename);

		// ��ȡǰ׺
		const String& getPrefix() const { return m_prefix; }

		// ��ȡ·��
		const String& getPath() const { return m_path; }

		// ȫ·��
		String getFullPath(const String& resPath);

	private:
		String			m_path;
		String			m_prefix;
	};
}
