#include "HttpFileSystemArchive.h"

#ifdef ECHO_USE_LIBEVENT
#include <Foundation/Util/LogManager.h>
#include <Foundation/Util/PathUtil.h>
#include <Engine/Object/Root.h>
#include "NetConnection/NetConnectionManager.h"

#ifdef ECHO_PLATFORM_HTML5
	#include <emscripten.h>
#endif

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_helper.hpp"

namespace Echo
{
#ifdef ECHO_PLATFORM_HTML5
	// �û�����
	struct HttpUserData
	{
		HttpFileSystemArchive* m_archive;
		String				   m_fileName;

		HttpUserData( HttpFileSystemArchive* archive, const String& fileName)
			: m_archive( archive), m_fileName( fileName)
		{}
	};

	// http��Դ���سɻص�
	void http_async_wget_onload_func( void* args, void* data, int size)
	{
		HttpUserData* userData = static_cast<HttpUserData*>(args);
		userData->m_archive->onOpenFileComplete( userData->m_fileName, data, size, NetErrCode::HA_REQUEST_OK);

		EchoSafeDelete( args);
	}

	// http��Դ������Ϣ
	void http_args_callback_func( void* args)
	{
		HttpUserData* userData = static_cast<HttpUserData*>(args);
		EchoLogError("Http File [%s] Download Failed...", userData->m_fileName.c_str());

		EchoSafeDelete( userData);
	}

#else
	// �ص�
	void onRequestFileComplete(void* arg, const Echo::String& name, void* data, size_t size, NetErrCode::ErrCode errCode)
	{
		HttpFileSystemArchive* pThis = static_cast<HttpFileSystemArchive*>(arg);
		pThis->onOpenFileComplete(name, data, size, errCode);
	}

#endif

	// ���캯��
	HttpFileSystemArchive::HttpFileSystemArchive(const String& name, const String& archType)
		: Archive(name, archType)
	{
	}

	//  �Ƿ��Сд����
	bool HttpFileSystemArchive::isCaseSensitive(void) const
	{
		return true;
	}

	// ��������
	HttpFileSystemArchive::~HttpFileSystemArchive()
	{
		unload();
	}

	// ����
	void HttpFileSystemArchive::load()
	{
		using namespace rapidxml;
		try
		{
			String fullPath = PathUtil::IsRelativePath(mName) ? EchoRoot->getResPath() + mName : mName;
			file<> fdoc( fullPath.c_str());
			xml_document<> doc;
			doc.parse<0>(fdoc.data());

			// root�ڵ�
			xml_node<>* rootnode = doc.first_node("http");
			if (rootnode)
			{
				// ����url
				m_url = rapidxml_helper::get_string(rootnode->first_attribute("url"));

				// ���������ļ�
				for (xml_node<>* filenode = rootnode->first_node("file"); filenode; filenode = filenode->next_sibling("file"))
				{
					m_files.insert( filenode->value());
				}
			}
		}
		catch (...)
		{
			EchoLogError("Http Archive [%s] load failed", mName.c_str());
		}

		EchoNetConnectionManager->createNetConnection(m_url); 
	}

	// ж��
	void HttpFileSystemArchive::unload()
	{
		m_url.clear();
		m_files.clear();
	}

	// �������ƻ�ȡȫ·��
	String HttpFileSystemArchive::location(const String& filename)
	{
		return m_url + filename;
	}

	// ����Դ
	DataStream* HttpFileSystemArchive::open(const String& filename)
	{
		String httpPath = location(filename);

		EchoLogDebug("Download File [%s]", httpPath.c_str());

#ifdef ECHO_PLATFORM_HTML5
		HttpUserData* userData = new HttpUserData( this, filename);
		emscripten_async_wget_data( httpPath.c_str(), userData, http_async_wget_onload_func, http_args_callback_func);

		return NULL;
#else
		auto* conn = EchoNetConnectionManager->getNetConnection(m_url); 
		if (conn)
		{
			conn->setRequestCompleteCb(onRequestFileComplete); 
			conn->requestFile(httpPath, this, filename);
		}
		else
		{
			EchoLogError("[HttpFileSystemArchive:%d]::NetConection Lost! url == %s", __LINE__, m_url.c_str()); 
			onRequestFileComplete(this, filename, nullptr, 0, NetErrCode::HA_REQUEST_CONN_LOST);
			return NULL; 
		}

		return NULL;
#endif
	}

	// ��Դ���سɹ��ص�
	void HttpFileSystemArchive::onOpenFileComplete(const Echo::String& name, void* data, size_t size, NetErrCode::ErrCode err)
	{
		if (err == NetErrCode::HA_REQUEST_OK)
		{		
			EchoLogDebug( "Http download File [%s] completed...", name.c_str());

			MemoryDataStream* stream = EchoNew(MemoryDataStream(name, size, true)); 
			stream->write(data, size); 
			stream->seek(0); 

			auto iter = m_callbacks.find(name); 

			if (iter != m_callbacks.end())
			{
				for (const auto& element : iter->second)
				{
					element(name, stream); 
				}

				m_callbacks.erase(iter);
			}
			else
			{
				EchoLogDebug("[onOpenFileComplete:%d]::[%s]Callback Is Not Exist!", __LINE__, name.c_str()); 
			}
			EchoSafeDelete(stream);
		}
		else
		{
			EchoLogError("Request Net File Failed, errCode == %d, fileName == %s", err, name.c_str()); 
		}
	}

	// �г������ļ�
	StringArray HttpFileSystemArchive::list(bool recursive, bool dirs)
	{
		StringArray strs; strs.reserve(m_files.size());
		for (set<String>::type::iterator it = m_files.begin(); it != m_files.end(); it++)
		{
			strs.push_back(*it);
		}

		return strs;
	}

	// ö�������ļ���Ϣ
	FileInfoList* HttpFileSystemArchive::listFileInfo(bool dirs)
	{
		FileInfoList* ret = EchoNewT(FileInfoList);

		//findFiles("*", dirs, 0, ret);

		return ret;
	}

	// �ж��ļ��Ƿ����
	bool HttpFileSystemArchive::exists(const String& filename)
	{
		set<String>::type::iterator it = m_files.find(filename);
		if (it != m_files.end())
			return true;

		return false;
	}

	// ����ļ�
	bool HttpFileSystemArchive::addFile(const String& fullPath)
	{
		String fileName = PathUtil::GetPureFilename(fullPath);
		StringUtil::LowerCase(fileName);
		if (m_files.find(fileName) == m_files.end())
		{
			m_files.insert(fileName);
			return true;
		}
		else
		{
			EchoLogError("Http Archive::addFile [%s] failed", fileName.c_str());
			return false;
		}
	}

	// �Ƴ��ļ�
	bool HttpFileSystemArchive::removeFile(const char* _fileName)
	{
		String fileName = _fileName;
		StringUtil::LowerCase(fileName);

		set<String>::type::iterator it = m_files.find(fileName);
		if (it != m_files.end())
		{
			m_files.erase(it);
			return true;
		}
		else
		{
			EchoLogError("Http Archive::removeFile [%s] failed", fileName.c_str());
			return false;
		}
	}


	bool HttpFileSystemArchive::addAsyncCompleteCallback(const Echo::String& name, EchoOpenResourceCb callback)
	{
		auto iter = m_callbacks.find(name); 
		if (iter == m_callbacks.end())
		{
			vector<EchoOpenResourceCb>::type callbacks; 
			callbacks.push_back(callback); 
			m_callbacks[name] = callbacks; 
			return true; 
		}

		auto& callbacks = iter->second; 
		callbacks.push_back(callback);

		return true;
	}

	// ��ȡ����
	const String& HttpFileSystemArchiveFactory::getType(void) const
	{
		static String name = "http";
		return name;
	}

}
#endif