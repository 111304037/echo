#include "render/GPUBuffer.h"

namespace Echo
{
	// ���캯��
	GPUBuffer::GPUBuffer(GPUBufferType type, Dword usage, const Buffer& buff)
		: m_type(type)
		, m_usage(usage)
		, m_size( 0)
	{
	}

	// ��������
	GPUBuffer::~GPUBuffer()
	{
	}

	// ��ȡ��С
	ui32 GPUBuffer::getSize() const
	{
		return m_size;
	}
}
