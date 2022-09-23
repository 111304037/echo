#include "netclient.h"

namespace network
{
	// ���ص�
	static void socket_read_cb(evutil_socket_t fd, short events, void* arg)
	{
		client* ct = (client*)arg;

		char msg[1024];
		int len = ::recv(fd, msg, 1024, 0);
		while (len > 0)
		{
			// �ɷ������ദ����Ϣ
			ct->on_accept_msg(fd, msg, len);

			// ��������
			len = ::recv(fd, msg, 1024, 0);
		}
	}

	// ���캯��
	client::client()
		: m_base(nullptr)
		, m_thread(nullptr)
		, m_ev_sockfd(nullptr)
		, m_packet(nullptr)
	{
		network_init();
	}

	// ��������(�˳�����������)
	client::~client()
	{
// 		event_base_loopexit(m_base, nullptr);
// 		event_base_free(m_base);
// 
// 		network_uninit();
// 
// 		// �ȴ��߳̽���
// 		m_thread->join();
	}

	// ���ӷ�����
	bool client::connect(const char* ip, int port)
	{
		evutil_socket_t sockfd;
		int status;

		sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = inet_addr(ip);
		sin.sin_port = htons(port);

		// �½��׽���
		sockfd = ::socket(PF_INET, SOCK_STREAM, 0);
		if (sockfd == -1)
			return false;

		// ���ӷ�����
		status = ::connect(sockfd, (sockaddr*)&sin, sizeof(sin));
		if (status == -1)
		{
			::closesocket(sockfd);
			return false;
		}

		// �����׽��ֲ�����
		evutil_make_socket_nonblocking(sockfd);

		// ����¼�
		m_base = event_base_new();
		m_ev_sockfd = event_new(m_base, sockfd, EV_READ | EV_PERSIST, socket_read_cb, this);
		event_add(m_ev_sockfd, NULL);

		// ������ѭ��
		auto function = std::bind(&client::connect_internel, this, ip, port);
		m_thread = new std::thread(function, this);

		return true;
	}

	// ����
	void client::connect_internel(const char* ip, int port)
	{
		// ѭ�������¼�
		event_base_dispatch(m_base);
	}

	// ��������
	void client::send( const void* data, int size)
	{
		if (m_ev_sockfd == NULL)
		{
			return;
		}
		evutil_socket_t sockefd = event_get_fd(m_ev_sockfd);
		
		::send(sockefd, (const char*)data, size, 0);
	}

	// ��������
	void client::send(const message_lite& msg_lite)
	{
		message_packet packget(0);
		packget.m_header.m_body_len = msg_lite.ByteSize();
		packget.m_header.m_pack_id = bkdr_hash(msg_lite.GetTypeName().c_str());

		// �������
		if (msg_lite.SerializeToArray(packget.get_body(), packget.get_body_space()))
		{
			send( packget.get_data(), packget.get_data_len());
		}
	}

	// ���ܵ���Ϣ
	void client::on_accept_msg(evutil_socket_t fd, char* msg, int len)
	{
		// �����°������ȡδ��װ���
		message_packet* packet = m_packet;

		// ��װ��
		int msg_processed = 0;
		for (int i = 0; i < len; i++)
		{
			// �½���
			if (!packet)
			{
				packet = new message_packet(fd);
			}

			if (packet->on_accept_data(msg[i]))
			{
				// ��װ���
				std::lock_guard<std::mutex> lock(m_mutex);
				m_accepted_msgs.push(packet);

				packet = nullptr;
				msg_processed = i + 1;
			}
		}

		// ��δ��װ��ɣ���¼֮
		m_packet = packet;

		// ������������
		if (msg_processed < len)
		{
			on_accept_msg(fd, msg + msg_processed, len - msg_processed);
		}
	}

	// ��ȡ��һ��Ҫ�������Ϣ
	message_packet* client::get_next_msg()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (!m_accepted_msgs.empty())
		{
			message_packet* packet = m_accepted_msgs.front();
			m_accepted_msgs.pop();

			return packet;
		}

		return nullptr;
	}

	// �����ѽ�����Ϣ
	void client::process()
	{
		while (message_packet* packet = get_next_msg())
		{
			m_dispatcher.on_accept_msg(packet->m_fd, *packet);
			delete packet;
		}
	}
}