#pragma once

#include "engine/core/base/variant.h"
#include "engine/core/Util/StringUtil.h"
#include <thirdparty/pugixml/pugixml.hpp>
#include "engine/core/resource/Res.h"

namespace Echo
{
	/**
	 * ��Ŀ�ļ�
	 */
	class ProjectSettings : public Res
	{
		ECHO_RES(ProjectSettings, Res, ".echo");

	public:
		ProjectSettings();
		~ProjectSettings();

	private:
	};
}
