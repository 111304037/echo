#include "base.h"
#include <stdio.h>

namespace network
{
	// ��ʼ��
	void network_init()
	{
#if defined(WIN32) || defined(WIN64)
		// Initialize Winsock.
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR)
			printf("Error at WSAStartup()\n");
#endif
	}

	// ж��
	void network_uninit()
	{
#if defined(WIN32) || defined(WIN64)
		WSACleanup();
#endif
	}

	// BKDR ��ϣ����
	unsigned int bkdr_hash(const char* str)
	{
		unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
		unsigned int hash = 0;

		while (*str)
			hash = hash * seed + (*str++);

		return (hash & 0x7FFFFFFF);
	}

	// ���캯��
	message_packet::message_packet(evutil_socket_t fd)
		: m_fd( fd)
		, m_idx( 0)
	{

	}

	// ������Ϣ
	bool message_packet::on_accept_data(char c)
	{
		if (m_idx < sizeof(header))
		{
			m_data[m_idx] = c;
			m_idx++;

			// ����Ƿ�Ϊ�����ݰ�
			if (m_idx == sizeof(header) && m_header.m_body_len==0)
				return true;
			else
				return false;
		}
		else if (m_idx < get_data_len())
		{
			m_data[m_idx] = c;
			m_idx++;

			if (m_idx == get_data_len())
				return true;
			else
				return false;
		}
		else
		{
			assert(false);
			return false;
		}
	}
}