#pragma once

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <thread>
#include <mutex>
#include <functional>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <google/protobuf/message_lite.h>

#define  gp google::protobuf

namespace network
{
	typedef google::protobuf::MessageLite message_lite;

	/**
	 * ��Ϣ��
	 */
	struct message_packet
	{
		struct header
		{
			gp::uint32  m_body_len;		// ���ݳ���(������ͷ)
			gp::uint32	m_pack_id;		// ��ID
		};

		evutil_socket_t m_fd;			// ����Դ
		gp::uint32		m_idx;			// �����α�

		union
		{
			header	m_header;
			char	m_data[4096];		// ��������
		};

		// ���캯��
		message_packet(evutil_socket_t fd);

		// ��ȡ����
		char* get_data() { return m_data; }

		// ��ȡ����
		int get_data_capacity() { return sizeof(m_data); }

		// ��ȡ������
		gp::uint32 get_data_len() { return m_header.m_body_len + sizeof(header); }

		// ��ȡ��������
		char* get_body() { return m_data + sizeof(header); }

		// ��ȡ�������ݿռ�
		gp::uint32 get_body_space() { return 4096 - sizeof(header); }

		// ��ȡ�������ݳ���
		gp::uint32 get_body_len() { return m_header.m_body_len; }

		// ѹ��
		void compress(){}

		// ��ѹ��
		void decompress(){}

		// ������Ϣ
		bool on_accept_data(char c);
	};

	/**
	 * ��������
	 */
	struct connection
	{
		evutil_socket_t		m_fd;		// �׽��־��
		message_packet*		m_packet;	// ���ڴ�������ݰ�

		// ���캯��
		connection()
			: m_fd(-1)
			, m_packet(nullptr)
		{}

		// ��������
		~connection()
		{
		}
	};

	// ��ʼ��
	void network_init();

	// ж��
	void network_uninit();

	// BKDR ��ϣ����
	unsigned int bkdr_hash(const char* str);
}