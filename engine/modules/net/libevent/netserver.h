#pragma once

#include "base.h"
#include "dispatcher.h"

namespace network
{
	/**
	 * �������
	 */
	class server
	{
		typedef std::map<evutil_socket_t, connection> connection_map;
	public:
		server();
		~server();

		// ��������
		bool start( int port, int listen_num=2048);

		// �������ݵ��ͻ���
		void send( evutil_socket_t fd, void* data, int size);

		// ��������
		void send( evutil_socket_t fd, const message_lite& msg_lite);

		// ��ȡ��������
		int get_conn_num() const { return (int)m_conns.size(); }

		// ������Ϣ�ص�
		template<typename T> bool register_msg_cb(const typename msg_call_back_t<T>::recv_msg_cb& cb) { return m_dispatcher.register_msg_cb<T>(cb); }

		// �����ѽ�����Ϣ
		void process();

	public:
		// ����
		void listen_internel();

		// ��ȡbase
		event_base* get_base() { return m_base; }

		// �����µ�����
		void on_accept_connect(evutil_socket_t fd);

		// ֹͣ����
		void on_stop_connect(evutil_socket_t fd);

		// ���ܵ���Ϣ
		void on_accept_msg(evutil_socket_t fd, char* msg, int len);

		// ��ȡ��һ��Ҫ�������Ϣ
		message_packet* get_next_msg();

	private:
		std::thread*				m_thread;		// �߳̾��
		int							m_port;			// �������Ķ˿ں�
		int							m_max_conn_num;	// ���ͻ�����������
		event_base*					m_base;			// ����(����libevent����������һ��base)
		event*						m_ev_listener;	// ������
		connection_map				m_conns;		// ��������
		dispatcher					m_dispatcher;	// ��Ϣ�ַ���
		std::mutex					m_mutex;		// ������
		std::queue<message_packet*>	m_accepted_msgs;// ���յ�����Ϣ
	};
}