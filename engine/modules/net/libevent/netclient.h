#pragma once

#include "base.h"
#include "dispatcher.h"

namespace network
{
	/**
	 * ����ͻ���
	 */
	class client
	{
	public:
		client();
		~client();

		// ���ӷ�����
		bool connect(const char* ip, int port);

		// ��������
		void send(const void* data, int size);

		// ��������
		void send(const message_lite& msg_lite);

		// ������Ϣ�ص�
		template<typename T> bool register_msg_cb(const typename msg_call_back_t<T>::recv_msg_cb& cb) { return m_dispatcher.register_msg_cb<T>(cb);}

		// �����ѽ�����Ϣ
		void client::process();

		event* get_ev_sockfd(){ return m_ev_sockfd; }

	private:
		// ����
		void connect_internel(const char* ip, int port);

	public:
		// ���ܵ���Ϣ
		void on_accept_msg(evutil_socket_t fd, char* msg, int len);

		// ��ȡ��һ��Ҫ�������Ϣ
		message_packet* get_next_msg();

	private:
		std::thread*					m_thread;		// �߳̾��
		event_base*						m_base;			// �¼�����
		event*							m_ev_sockfd;	// �׽����¼�
		dispatcher						m_dispatcher;	// ��Ϣ�ַ�
		std::mutex						m_mutex;		// ������
		message_packet*					m_packet;		// ��װ�е����ݰ�
		std::queue<message_packet*>		m_accepted_msgs;// ���յ�����Ϣ
	};
}