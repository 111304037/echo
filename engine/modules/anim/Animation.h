#pragma once

#include "engine/core/Resource/Resource.h"

namespace Echo
{
	class AnimManager;
	class AnimKeyFrame;
	class Bone;
	class Animation: public Resource
	{
		typedef vector<AnimKeyFrame*>::type	AnimKeyFrameList;
	public:
		friend class AnimManager;

		// ��������
		enum AnimType
		{
			AT_UNKNOWN,
			AT_SKELETON,
			AT_KEYFRAME,
		};

	public:
		// ����
		static Animation* create(const String& name, bool isManual=false);

		// �ͷ�
		void release();

		virtual AnimKeyFrame* createAnimKeyFrame(ui32 time);
		virtual void removeAnimKeyFrameByIdx(ui32 idx);
		virtual void removeAnimKeyFrames();

		virtual void setAnimType(AnimType type);

		// ��ȡ��������
		virtual AnimType getAnimType() const;
		virtual ui32 getFrameCount() const;
		virtual ui32 getLength() const;
		virtual ui32 getFPS() const;
		virtual bool hasScaling() const;
		virtual AnimKeyFrame* getAnimKeyFrame(ui32 time) const;
		virtual AnimKeyFrame* getAnimKeyFrameByIdx(ui32 idx) const;
		virtual const AnimKeyFrameList&	getAnimKeyFrameList() const;
		virtual ui32 getFrameIndex(ui32 time, i32 deltaTime) const;

		// ���ù���Ȩ��
		void setBoneWeight( const Bone* bone, float weight);

		// ��ȡ��ָ������Ӱ��Ȩ��
		float getBoneWeight( const Bone* bone);

		// ��ȡ����Ȩ��
		vector<float>::type& getBoneWeights() { return m_boneWeights; }

		// �Ƿ��Ǿֲ�����
		bool isPartition() const { return m_isPartition; }

		// ���ǲ��ֶ��������ز��ֶ���������
		int getPartionRootBone() const { return m_partitionRootBone; }

		// ��ȡ��������
		ui32 getBoneCount() const { return m_boneCount;}

		// ����
		void save( const char* filePath);

		void optAnimation();

	protected:
		Animation(const String& name, bool isManual);
		virtual ~Animation();

		virtual size_t calculateSize() const;
		virtual bool prepareImpl( DataStream* stream);
		virtual bool prepareImplmmap();
		AnimKeyFrame* createAnimKeyFramemmap(ui32 time, ui32 boneCount, const char*& dataPtr);
		virtual void unprepareImpl();
		virtual bool loadImpl();
		virtual void unloadImpl();

		// ����weights
		void parseBoneWeights();

	protected:
		AnimType			m_type;			// ��������
		ui32				m_version;
		ui32				m_frameCount;	// ֡����
		ui32				m_length;		// ��������
		ui32				m_fps;			// ֡��
		ui32				m_boneCount;	// ��������
		vector<float>::type m_boneWeights;	// ����Ȩ������
		bool				m_bHasScaling;	// �Ƿ����������Ϣ
		AnimKeyFrameList	m_keyFrames;	// ֡����
		bool				m_isPartition;	// �Ƿ�Ϊ���ֶ���
		int					m_partitionRootBone;	// ���ֶ���������

#ifdef LIULIANG_ANIMATION_MMAP
		struct Header
		{
			ui32 version;	// �汾��
			ui32 fps;
			ui32 frameCount;
			ui32 boneCount;
		};

		DataStream*			m_stream;		// �ļ���(Ψһ��Ҫɾ����ָ��)
		const char*			m_data;			// �ļ�����
		ui32				m_size;			// ���ݴ�С
#endif
	};
}