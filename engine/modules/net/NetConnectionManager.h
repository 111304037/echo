#pragma once

#include "engine/core/Memory/MemAllocDef.h"

#ifdef ECHO_USE_LIBEVENT
extern "C"
{
	struct event_base;
	struct evhttp_uri;
	struct evhttp_connection;
	struct evhttp_request;
}

namespace Echo
{
	class NetErrCode
	{
	public: 
		enum ErrCode
		{
			HA_REQUEST_TIME_OUT,  // ��ʱ
			HA_REQUEST_CONN_LOST, // ���Ӷ�ʧ
			HA_REQUEST_OK = 0
		};
	};

	typedef void(*onRequestCompleteCb)(void* arg, const Echo::String& fileName, void* data, size_t len, NetErrCode::ErrCode err);
	
	class NetConnection : public ObjectAlloc
	{
		NetConnection(NetConnection& rhs); 
		NetConnection& operator=(NetConnection& rhs); 

	public: 
		NetConnection(); 
		~NetConnection(); 

	public:
		// ����������Ϣ
		void createConnection(const Echo::String& url); 

		// �������ʱ�ص�
		void setRequestCompleteCb(onRequestCompleteCb cb);

		// �����ļ�
		void requestFile(const String& url, void* arg, const String& fileName);

		// ������ɻص�
		void onRequestComplete(evhttp_request* request); 

	private:
		struct NetContext
		{
			String name; 
			void* pThis; 
		};

	private: 
		onRequestCompleteCb m_onRequestCompleteCallback; 

		struct evhttp_uri*				m_uri;
		struct evhttp_connection*		m_con;

		map<String, NetContext>::type   m_contexts; 
	};


	class NetConnectionManager : public ObjectAlloc
	{
		NetConnectionManager(NetConnectionManager& rhs); 
		NetConnectionManager& operator=(NetConnectionManager& rhs); 

	public: 
		NetConnectionManager(); 
		~NetConnectionManager(); 

	public: 
		// �½���������
		void createNetConnection(const Echo::String& url); 

		// �Ƴ���������
		void removeNetConnection(const Echo::String& url); 

		// ��ȡ��������
		NetConnection* getNetConnection(const Echo::String& url); 

		// ��ȡHttpģ��
		event_base* getNetConnectionBase() const { return m_eventBase; }
	
	private:
		event_base*						  m_eventBase; 
		map<String, NetConnection*>::type m_netConnectinos;
		
	};
}

#else
namespace Echo
{
	class NetConnectionManager {};
}
#endif
