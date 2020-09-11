#include "Update.h"
#include <engine/core/base/echo_def.h>
#include <engine/core/io/stream/DataStream.h>
#include <engine/core/util/PathUtil.h>
#include <QMessageBox>
#include <QApplication>

namespace Studio
{
    /*
	static void http_update_astudio_cb(void* arg, const char* url, void* data, size_t len, int responsecode)
	{
		// д�뵱ǰĿ¼Cache�ļ���
		Echo::String savePath = Echo::PathUtil::GetCurrentDir() + "/update/astudio-setup.exe";
		Echo::PathUtil::WriteData(savePath.c_str(), data, static_cast<int>(len));

		// ����
		Echo::String cmd = Echo::PathUtil::GetCurrentDir() + "/update/astudio-setup.exe";
		//WinExec(cmd.c_str(), SW_SHOW);
	}

	static void http_get_all_versions_cb(void* arg, const char* url, void* data, size_t len, int responsecode)
	{
		Echo::StringArray* oResult = (Echo::StringArray*)arg;

		Echo::String httpText = (const char*)data;
		size_t beginPos = httpText.find(">astudio", 0);
		size_t endPos = httpText.find("setup.exe<", 0);
		while (beginPos != Echo::String::npos && endPos!=Echo::String::npos)
		{
			Echo::String astudio = httpText.substr(beginPos+1, endPos-beginPos+8);
			httpText = Echo::StringUtil::Replace(httpText, astudio, "");

			oResult->emplace_back(astudio);

			beginPos = httpText.find(">astudio", 0);
			endPos = httpText.find("setup.exe<", 0);
		}
	}*/

	Echo::StringArray Update::getAllEnabledVersions()
	{
		Echo::StringArray result;
		//network::httpget("http://10.137.81.231:80/", http_get_all_versions_cb, &result);

		return result;
	}

	// ����ĳ�汾������
	void Update::downloadVersion(const Echo::String& resName)
	{
		//network::httpget(("http://10.137.81.231:80/" + resName).c_str(), http_update_astudio_cb, nullptr);
	}
}
