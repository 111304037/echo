#ifndef __EFFECT_PARTICLE_H__
#define __EFFECT_PARTICLE_H__

#include "Engine/Core.h"
#include <engine/core/Util/ObjectPool.h>

namespace Echo
{
	/**
	 * ���Ӷ���
	 */
	class EffectParticle
	{
	public:
		EffectParticle(void);
		virtual ~EffectParticle(void);

		virtual void copyAttributesTo(EffectParticle* particle);

		void reParticleState();
		void forwardParticleState();
		void forwardParticleState2();

		EffectParticle* parentParticle;

		//// copy attribute begin
		Vector3		position;			// λ��
		Quaternion	oritation;
		float		selfRotation;
		Color		color;				// ��ɫ
		float		scale;				// ����
		Vector3		direction;			// ����

		Vector3		originalPosition;
		Quaternion	originalOritation;
		float		originalSelfRotation;
		Color		originalColor;
		float		originalScale;
		float       originalSpeed;
		Vector3		originalDirection;

		Vector3		movementDirection;
		i32			mTime;
		i32         mLeaveTime;
		// copy attribute end

		Vector3		scenePosition;
		Quaternion	sceneOritation;

		Vector3		mlastPosition;
		int			mReDirection;
		bool		mInController;

		unsigned long startPlayTime;	//��ʼ�������ӵ�ʱ��,����ȷ�����ŵ��˵ڼ�֡
		int			  mRandomAnimIndex; //ƽ��ģʽ��Ҫ���Ŷ�����һ֡

		static const size_t sCopyAttributeSize;
		static const size_t sReStateSize;
	};

	/**
	 * ���ӳ�
	 * �������ӽ���ҳ����Ҫ����10ҳ
	 */
	class EffectParticlePool
	{
	public:
		// ���
		struct Chunk
		{
			friend class EffectParticlePool;

			ui32						m_size;
			ui32						m_capacity;		// ��С
			ObjectPool<EffectParticle>*	m_pool;			// ��Ӧ�Ķ����

			// �½�
			EffectParticle* New() 
			{
				if (m_size < m_capacity)
				{
					m_size++;

					EffectParticle* particle = m_pool->New();
					EchoAssert(particle);
					return particle;
				}

				return NULL;
			}

			// ����
			void Delete(EffectParticle* particle)
			{
				m_size--;
				m_pool->Delete(particle);
			}

		private:
			Chunk(ui32 capacity, ObjectPool<EffectParticle>* pool)
				: m_size(0), m_capacity(capacity), m_pool(pool)
			{}

			~Chunk()
			{
				EchoAssert(!m_size);
			}
		};

	private:
		// ҳ
		struct Page
		{
			ObjectPool<EffectParticle>*	m_pool;				// �����
			ui32						m_capacityRemain;	// ʣ������
			ui32						m_idleTime;			// 
			ui32						m_capacity;			// ����

			Page(ui32 capacity);
			Page(const Page&) = delete;
			Page(Page&& other) :
				m_pool(other.m_pool),
				m_capacityRemain(other.m_capacityRemain),
				m_idleTime(other.m_idleTime)
			{
				other.m_pool = nullptr;
			}
			Page& operator=(const Page&) = delete;
			Page& operator=(Page&& other)
			{
				std::swap(m_pool, other.m_pool);
				m_capacityRemain = other.m_capacityRemain;
				m_idleTime = other.m_idleTime;
				other.m_pool = nullptr;
				return *this;
			}
			~Page();
		};

	public:
		EffectParticlePool(ui32 pageSize);
		~EffectParticlePool();

		// ��ȡ����
		void buildDesc( String& desc);

		// �½���
		Chunk* NewChunk(ui32 chunkCapacity);

		// �ͷſ�
		void ReleaseChunk(Chunk* chunk);

		// ����
		void tick(i32 elapsedTime);

	private:
		vector<Page>::type		m_pages;	// ���ж����
		ui32					m_pageSize;	// Ĭ��ҳ��С
#ifdef ECHO_DEBUG
		vector<Chunk*>::type	m_chunks;	// ��
#endif
	};
}

#endif
