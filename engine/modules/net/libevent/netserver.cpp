#include "netserver.h"
#include "base.h"
#include <assert.h>

namespace network
{
	// ���¼��ص�
	static void socket_read_cb( evutil_socket_t fd, short events, void* arg)
	{
		server* sev = (server*)arg;

		char msg[1024];
		int len = ::recv(fd, msg, 1024, 0);
		while (len>0)
		{
			// �ɷ������ദ����Ϣ
			sev->on_accept_msg(fd, msg, len);

			// ��������
			len = ::recv(fd, msg, 1024, 0);
		}
	}

	// ���յ����ӻص�
	static void accept_cb(evutil_socket_t fd, short events, void* arg)
	{
		server* sev = (server*)arg;
		event_base* base = (event_base*)sev->get_base();

		evutil_socket_t sockfd;
		sockaddr_in sin_client;
		socklen_t len = sizeof(sin_client);			// len�����г�ֵ

		sockfd = ::accept( fd, (sockaddr*)&sin_client, &len);
		if (sockfd == -1)
		{
			return;
		}

		// �����¼�
		event* ev = event_new(NULL, -1, 0, NULL, NULL);

		// �����¼�
		event_assign(ev, base, sockfd, EV_READ | EV_PERSIST, socket_read_cb, (void*)sev);
		event_add(ev, NULL);

		sev->on_accept_connect(sockfd);
	}


	// ���캯��
	server::server()
		: m_base(NULL)
		, m_ev_listener(NULL)
		, m_port(-1)
		, m_thread(NULL)
		, m_max_conn_num(0)
	{
		network_init();
	}

	// �������� (Ŀǰ�˳�������ȷ)
	server::~server()
	{
// 		event_base_loopexit(m_base, nullptr);
// 		event_base_free(m_base);
// 
// 		network_uninit();
// 
// 		// �ȴ��߳̽���
// 		m_thread->join();
	}

	// ��������
	bool server::start( int port, int listen_num)
	{
		m_port = port;
		m_max_conn_num = listen_num;

		evutil_socket_t listener;

		listener = ::socket(AF_INET, SOCK_STREAM, 0);
		if (listener == -1)
			return false;

		evutil_make_listen_socket_reuseable(listener);

		// ��ʼ�������ַ
		sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = 0;
		sin.sin_port = htons(m_port);

		// ��
		if (::bind(listener, (sockaddr*)&sin, sizeof(sin)) < 0)
			return false;

		// ��ʼ����
		if (::listen(listener, listen_num) < 0)
			return false;

		evutil_make_socket_nonblocking(listener);

		// �½����л���
		m_base = event_base_new();
		if (!m_base)
			return false;

		// �½������¼�
		event* ev_listen = event_new( m_base, listener, EV_READ | EV_PERSIST, accept_cb, this);
		event_add(ev_listen, NULL);
		
		// ������ѭ��
		auto function = std::bind(&server::listen_internel, this);
		m_thread = new std::thread(function, this);

		return true;
	}

	// ����
	void server::listen_internel()
	{
		// ѭ�������¼�
		event_base_dispatch(m_base);
	}

	// �������ݵ��ͻ���
	void server::send(evutil_socket_t fd, void* data, int size)
	{
		::send(fd, (const char*)data, size, 0);
	}

	// ��������
	void server::send(evutil_socket_t fd, const message_lite& msg_lite)
	{
		message_packet packget(fd);
		packget.m_header.m_body_len = msg_lite.ByteSize();
		packget.m_header.m_pack_id = bkdr_hash(msg_lite.GetTypeName().c_str());

		// �������
		if (msg_lite.SerializeToArray(packget.get_body(), packget.get_body_space()))
		{
			send( fd, packget.get_data(), packget.get_data_len());
		}
	}

	// �����µ�����
	void server::on_accept_connect(evutil_socket_t fd)
	{
		connection conn;
		conn.m_fd = fd;
		m_conns[fd] = conn;
	}

	// ֹͣ����
	void server::on_stop_connect(evutil_socket_t fd)
	{
		connection_map::iterator it = m_conns.find(fd);
		if (it != m_conns.end())
		{
			assert(it->second.m_fd == fd);
			::closesocket(fd);
			m_conns.erase(it);
		}
	}

	// ���ܵ���Ϣ
	void server::on_accept_msg(evutil_socket_t fd, char* msg, int len)
	{
		// �����°������ȡδ��װ���
		message_packet* packet = m_conns[fd].m_packet;

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

				break;
			}
		}

		// ��δ��װ��ɣ���¼֮
		m_conns[fd].m_packet = packet;

		// ������������
		if (msg_processed < len)
		{
			on_accept_msg(fd, msg + msg_processed, len - msg_processed);
		}
	}

	// ��ȡ��һ��Ҫ�������Ϣ
	message_packet* server::get_next_msg()
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
	void server::process()
	{
		while (message_packet* packet = get_next_msg())
		{
			m_dispatcher.on_accept_msg(packet->m_fd, *packet);
			delete packet;
		}
	}
}