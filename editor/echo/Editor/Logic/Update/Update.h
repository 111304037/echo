#pragma once

#include <engine/core/util/StringUtil.h>

namespace Studio
{
	class Update
	{
	public:
		// ��ȡ��ǰ�ɸ��°汾
		Echo::StringArray getAllEnabledVersions();

		// ����ĳ�汾������
		void downloadVersion(const Echo::String& resName);
	};
}
