#include "engine/core/Util/Singleton.h"
#include "AABBKeyFrame.h"
#include "Bone.h"
#include "engine/core/Util/LogManager.h"
#include "Engine/core/Geom/GeomLib.h"
#include "Engine/modules/Anim/AnimKeyFrame.h"
#include "engine/core/io/IO.h"
#include "AnimManager.h"
#include "Animation.h"

namespace Echo
{
#pragma pack(push, 1)

	struct _AnimKeyFrame
	{
		ui32	keyFrameTime;
		float	translate[3];
		float	axis[3];
		float	radian;
	};

#pragma pack(pop)

	Animation::Animation(const String& name, bool isManual)
		: Resource(name, RT_Animation, isManual)
		, m_type(AT_UNKNOWN)
		, m_version(0)
		, m_frameCount(0)
		, m_length(0)
		, m_fps(20)
		, m_boneCount(0)
		, m_bHasScaling(true)
		, m_isPartition(false)
		, m_partitionRootBone(0)
#ifdef LIULIANG_ANIMATION_MMAP
		, m_stream(nullptr)
#endif
	{
	}

	Animation::~Animation()
	{
		unloadImpl();
	}

	// ����
	Animation* Animation::create(const String& name, bool isManual)
	{
		return AnimManager::instance()->createAnim(name, isManual);
	}

	// �ͷ�
	void Animation::release()
	{
		AnimManager::instance()->releaseResource(this);
	}

	size_t Animation::calculateSize() const
	{
		// not implement
		return 0;
	}

	AnimKeyFrame* Animation::createAnimKeyFramemmap(ui32 time, ui32 boneCount, const char*& dataPtr)
	{
		AABBKeyFrame* pKeyFrame = EchoNew(AABBKeyFrame(this, time));
		pKeyFrame->setAnimDatammap(boneCount, dataPtr);
		dataPtr += (sizeof(Vector4) + sizeof(Vector4)) * m_boneCount;

		pKeyFrame->setBoxmmap(dataPtr);
		dataPtr += sizeof(Vector3) * 2;

		// Insert just before upper bound
		m_keyFrames.push_back(pKeyFrame);

		return pKeyFrame;
	}

	// �����ļ�ӳ�����Դ����
	bool Animation::prepareImplmmap()
	{
#ifdef LIULIANG_ANIMATION_MMAP
		m_type = AT_SKELETON;

		m_stream = IO::instance()->open(getName());
		m_data = (const char*)m_stream->getBuffer();
		m_size = m_stream->size();

		// �α�
		const char* dataPtr = m_data;

		const Header* header = (const Header*)dataPtr; dataPtr += sizeof(Header);
		m_bHasScaling = header->version >= VERSION_ANIM_SUPPORT_SCALE ? true : false;
		if (m_bHasScaling == false)
			return false;

		// ���� m_length ��û�д��ڵı�Ҫ
		m_frameCount = header->frameCount;
		m_length = ui32((float)m_frameCount / (float)header->fps * 1000);
		m_fps = header->fps;
		m_boneCount = header->boneCount;

		// ���ڿ��Ļ�������Ȩ����������ʱ�ò���...
		// ����Ȩ������
		m_boneWeights.resize(m_boneCount, 1.f);

		// ��������Ȩ������
		parseBoneWeights();

		// ����֡����
		for (ui32 i = 0; i < m_frameCount; ++i)
		{
			ui32 keyFrameTime = (i * 1000) / (m_fps * 1000);
			createAnimKeyFramemmap(keyFrameTime, m_boneCount, dataPtr);
		}

#endif
		return true;
	}

	bool Animation::prepareImpl(DataStream* stream)
	{
		if (!stream)
			return false;

#ifdef LIULIANG_ANIMATION_MMAP
		return prepareImplmmap();
#endif

		m_type = AT_SKELETON;

		ui32 version;
		stream->read(&version, sizeof(ui32));
		m_version = version;

		bool useHalfFloat = false;
		if (version >= VERSION_HALF_FLOAT_BASE)
		{
			useHalfFloat = true;
		}

		if (!useHalfFloat && version >= VERSION_ANIM_SUPPORT_SCALE)
		{
			m_bHasScaling = true;
		}
		else
		{
			m_bHasScaling = false;
		}

		// fps
		ui32 fps;
		stream->read(&fps, sizeof(ui32));

		// frame count
		ui32 frameCount;
		stream->read(&frameCount, sizeof(ui32));

		// bone count
		ui32 boneCount;
		stream->read(&boneCount, sizeof(ui32));

		m_frameCount = frameCount;
		m_length = ui32((float)frameCount / (float)fps * 1000);
		m_fps = fps;
		m_boneCount = boneCount;

#ifdef ECHO_PLATFORM_ANDROID
		if (m_boneCount < 0 || m_boneCount > 1024)
		{
			EchoLogError("[Animation::prepareImpl:%d]:: Anim[%s], bondCount == %d, Invalid.", __LINE__, stream->getName().c_str(), m_boneCount);
			return false;
		}
#endif
		// ����Ȩ������
		m_boneWeights.resize(m_boneCount, 1.f);

		if (version >= VERSION_ANIM_ADD_BONEWEIGHTS)
		{
			stream->read(&m_boneWeights[0], sizeof(float)*m_boneCount);
		}

		// ��������Ȩ������
		parseBoneWeights();

		float position[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float rotation[4];
		Vector4 vPos;
		Quaternion qRot;
		Box box;
		for (ui32 i = 0; i < frameCount; ++i)
		{
			ui32 keyFrameTime = ui32(i * 1000.f / fps);
			AABBKeyFrame* pKeyFrame = ECHO_DOWN_CAST<AABBKeyFrame*>(createAnimKeyFrame(keyFrameTime));
			pKeyFrame->createAnimData(boneCount);
			for (ui32 j = 0; j < boneCount; ++j)
			{
				if (useHalfFloat)
				{
					ui16 iPosRot[7];
					stream->read(&iPosRot, sizeof(iPosRot));
					for (int k = 0; k < 3; ++k)
					{
						position[k] = Math::HalfToFloat(iPosRot[k]);
					}
					vPos.set(position[0], position[1], position[2], 1.0f);

					for (int k = 0; k < 4; ++k)
					{
						rotation[k] = Math::HalfToFloat(iPosRot[k + 3]);
					}
					qRot.set(rotation[0], rotation[1], rotation[2], rotation[3]);
				}
				else
				{
					if (m_bHasScaling)
					{
						stream->read(&position, sizeof(float) * 4);
					}
					else
					{
						stream->read(&position, sizeof(float) * 3);
					}

					vPos.set(position[0], position[1], position[2], position[3]);

					stream->read(&rotation, sizeof(rotation));
					qRot.set(rotation[0], rotation[1], rotation[2], rotation[3]);
				}
				pKeyFrame->setAnimData(j, qRot, vPos);
			}

			if (useHalfFloat)
			{
				ui16 iPosRot[6];
				stream->read(&iPosRot, sizeof(iPosRot));

				box.vMin.x = Math::HalfToFloat(iPosRot[0]);
				box.vMin.y = Math::HalfToFloat(iPosRot[1]);
				box.vMin.z = Math::HalfToFloat(iPosRot[2]);
				box.vMax.x = Math::HalfToFloat(iPosRot[3]);
				box.vMax.y = Math::HalfToFloat(iPosRot[4]);
				box.vMax.z = Math::HalfToFloat(iPosRot[5]);
			}
			else
			{
				stream->read(&box.vMin, sizeof(Vector3));
				stream->read(&box.vMax, sizeof(Vector3));
			}

			pKeyFrame->setBox(box);
		}

		return true;
	}

	// ����
	void Animation::save(const char* filePath)
	{
#ifdef ECHO_PLATFORM_WINDOWS
		FILE* fileHandle;
		fopen_s(&fileHandle, filePath, "wb");

		// �汾��
		i32 version = VERSION_ANIM_ADD_BONEWEIGHTS;
		fwrite(&version, sizeof(i32), 1, fileHandle);

		// FPS
		fwrite(&m_fps, sizeof(m_fps), 1, fileHandle);

		// ֡��
		fwrite(&m_frameCount, sizeof(m_frameCount), 1, fileHandle);

		// ������
		fwrite(&m_boneCount, sizeof(m_boneCount), 1, fileHandle);

		// ����Ȩ������
		if (m_boneWeights.size() < m_boneCount)
		{
			m_boneWeights.resize(m_boneCount);
		}
		fwrite(m_boneWeights.data(), sizeof(float)*m_boneCount, 1, fileHandle);

		// �ؼ�֡����
		for (size_t keyIdx = 0; keyIdx < m_keyFrames.size(); keyIdx++)
		{
			AnimKeyFrame* keyFrame = m_keyFrames[keyIdx];

			// д�ؼ�֡����
			for (ui32 boneIdx = 0; boneIdx < m_boneCount; boneIdx++)
			{
				Vector4 translation = keyFrame->getAnimTranslate(boneIdx);
				Quaternion rotation = keyFrame->getAnimQuaternion(boneIdx);
				fwrite(&translation, sizeof(translation), 1, fileHandle);
				fwrite(&rotation, sizeof(rotation), 1, fileHandle);
			}

			// д�ؼ�֡��Χ��
			AABBKeyFrame* aabbKeyFrame = ECHO_DOWN_CAST<AABBKeyFrame*>(keyFrame);
			if (aabbKeyFrame)
			{
				fwrite(&aabbKeyFrame->getBox().vMin, sizeof(Vector3), 1, fileHandle);
				fwrite(&aabbKeyFrame->getBox().vMax, sizeof(Vector3), 1, fileHandle);
			}
		}

		fflush(fileHandle);
		fclose(fileHandle);
#endif
	}

	void Animation::unprepareImpl()
	{
	}

	bool Animation::loadImpl()
	{
		return true;
	}

	void Animation::unloadImpl()
	{
#ifdef LIULIANG_ANIMATION_MMAP
		EchoSafeDelete(m_stream, DataStream);
#endif

		EchoSafeDeleteContainer(m_keyFrames, AnimKeyFrame);
	}

	AnimKeyFrame* Animation::createAnimKeyFrame(ui32 time)
	{
		AnimKeyFrame* pKeyFrame = NULL;

		switch (m_type)
		{
			case Animation::AT_SKELETON:
			{
				pKeyFrame = EchoNew(AABBKeyFrame(this, time));
			} break;
			default:
			{
				pKeyFrame = EchoNew(AnimKeyFrame(this, time));
			}
		}

		// Insert just before upper bound
		m_keyFrames.push_back(pKeyFrame);

		return pKeyFrame;
	}

	void Animation::removeAnimKeyFrameByIdx(ui32 idx)
	{
		// If you hit this assert, then the track index is out of bounds
		EchoAssert(idx < (ui32)m_keyFrames.size());

		AnimKeyFrameList::iterator it = m_keyFrames.begin();

		it += idx;

		EchoSafeDelete(*it, AnimKeyFrame);
		m_keyFrames.erase(it);
	}

	void Animation::removeAnimKeyFrames()
	{
		EchoSafeDeleteContainer(m_keyFrames, AnimKeyFrame);
	}

	void Animation::setAnimType(AnimType type)
	{
		m_type = type;
	}

	Animation::AnimType Animation::getAnimType() const
	{
		return m_type;
	}

	ui32 Animation::getFrameCount() const
	{
		return m_frameCount;
	}

	ui32 Animation::getLength() const
	{
		return m_length;
	}

	ui32 Animation::getFPS() const
	{
		return m_fps;
	}

	bool Animation::hasScaling() const
	{
		return m_bHasScaling;
	}

	AnimKeyFrame* Animation::getAnimKeyFrame(ui32 time) const
	{
		ui32 nFrame = (ui32)Math::Floor((float)time*0.001f * m_fps);
		EchoAssert(nFrame >= 0 && nFrame < m_frameCount);

		return m_keyFrames[nFrame];
	}

	AnimKeyFrame* Animation::getAnimKeyFrameByIdx(ui32 idx) const
	{
		EchoAssert(idx <= m_frameCount);

		return m_keyFrames[idx];
	}

	const Animation::AnimKeyFrameList& Animation::getAnimKeyFrameList() const
	{
		return m_keyFrames;
	}

	ui32 Animation::getFrameIndex(ui32 time, i32 deltaTime) const
	{
		int nFrame = deltaTime >= 0 ? (int)Math::Floor((float)time*0.001f * m_fps)
									: (int)Math::Ceil((float)time*0.001f * m_fps);

		return Math::Clamp<int>(nFrame, 0, m_frameCount - 1);
	}

	// ���ù���Ȩ��
	void Animation::setBoneWeight(const Bone* bone, float weight)
	{
		m_boneWeights[bone->getId()] = weight;

		parseBoneWeights();
	}

	// ��ȡ��ָ������Ӱ��Ȩ��
	float Animation::getBoneWeight(const Bone* bone)
	{
		return m_boneWeights[bone->getId()];
	}

	// ����weights
	void Animation::parseBoneWeights()
	{
		for (size_t i = 0; i < m_boneWeights.size(); i++)
		{
			if (Math::Abs(m_boneWeights[i] - 0.5f) < Math::EPSILON)
			{
				m_isPartition = true;
				m_partitionRootBone = i;
				break;
			}
		}
	}

	void Animation::optAnimation()
	{
#ifdef ECHO_PLATFORM_WINDOWS
		String filePath = IO::instance()->getFullPath(getName());
		FILE* fileHandle;
		fopen_s(&fileHandle, filePath.c_str(), "wb");

		// �汾��
		i32 version = m_version;
		fwrite(&version, sizeof(i32), 1, fileHandle);

		// FPS && ֡��
		ui32 newFrameCount = m_frameCount % 2 != 0 ? (m_frameCount + 1) / 2 : m_frameCount / 2 + 1;
		ui32 fps = (ui32)(((real32)newFrameCount * 1000.f) / (real32)m_length);
		fwrite(&fps, sizeof(ui32), 1, fileHandle);
		
		fwrite(&newFrameCount, sizeof(m_frameCount), 1, fileHandle);

		// ������
		fwrite(&m_boneCount, sizeof(m_boneCount), 1, fileHandle);

		// ����Ȩ������
		if (m_boneWeights.size() < m_boneCount)
		{
			m_boneWeights.resize(m_boneCount);
		}
		if (version >= VERSION_ANIM_ADD_BONEWEIGHTS)
		{
			fwrite(m_boneWeights.data(), sizeof(float)*m_boneCount, 1, fileHandle);
		}

		// ��һ֡�����һ֡����
		// �м�Ĺؼ�֡��һ֡����һ֡
		// first frame
		AnimKeyFrame* keyFrame = m_keyFrames[0];
		for (ui32 boneIdx = 0; boneIdx < m_boneCount; boneIdx++)
		{
			Vector4 translation = keyFrame->getAnimTranslate(boneIdx);
			Quaternion rotation = keyFrame->getAnimQuaternion(boneIdx);
			fwrite(&translation, sizeof(translation), 1, fileHandle);
			fwrite(&rotation, sizeof(rotation), 1, fileHandle);
		}
		AABBKeyFrame* aabbKeyFrame = ECHO_DOWN_CAST<AABBKeyFrame*>(keyFrame);
		if (aabbKeyFrame)
		{
			fwrite(&aabbKeyFrame->getBox().vMin, sizeof(Vector3), 1, fileHandle);
			fwrite(&aabbKeyFrame->getBox().vMax, sizeof(Vector3), 1, fileHandle);
		}

		// �ؼ�֡����
		size_t size = m_keyFrames.size() - 1;
		size_t keyIdx = 2;
		if (keyIdx > newFrameCount-2)
		{
			keyIdx = 1;
		}
		for (; keyIdx <= size; keyIdx+=2)
		{
			AnimKeyFrame* keyFrame = m_keyFrames[keyIdx];

			// д�ؼ�֡����
			for (ui32 boneIdx = 0; boneIdx < m_boneCount; boneIdx++)
			{
				Vector4 translation = keyFrame->getAnimTranslate(boneIdx);
				Quaternion rotation = keyFrame->getAnimQuaternion(boneIdx);
				fwrite(&translation, sizeof(translation), 1, fileHandle);
				fwrite(&rotation, sizeof(rotation), 1, fileHandle);
			}

			// д�ؼ�֡��Χ��
			AABBKeyFrame* aabbKeyFrame = ECHO_DOWN_CAST<AABBKeyFrame*>(keyFrame);
			if (aabbKeyFrame)
			{
				fwrite(&aabbKeyFrame->getBox().vMin, sizeof(Vector3), 1, fileHandle);
				fwrite(&aabbKeyFrame->getBox().vMax, sizeof(Vector3), 1, fileHandle);
			}
		}
		
		// last frame
		keyFrame = m_keyFrames[m_frameCount - 1];
		for (ui32 boneIdx = 0; boneIdx < m_boneCount; boneIdx++)
		{
			Vector4 translation = keyFrame->getAnimTranslate(boneIdx);
			Quaternion rotation = keyFrame->getAnimQuaternion(boneIdx);
			fwrite(&translation, sizeof(translation), 1, fileHandle);
			fwrite(&rotation, sizeof(rotation), 1, fileHandle);
		}
		aabbKeyFrame = ECHO_DOWN_CAST<AABBKeyFrame*>(keyFrame);
		if (aabbKeyFrame)
		{
			fwrite(&aabbKeyFrame->getBox().vMin, sizeof(Vector3), 1, fileHandle);
			fwrite(&aabbKeyFrame->getBox().vMax, sizeof(Vector3), 1, fileHandle);
		}

		fflush(fileHandle);
		fclose(fileHandle);
#endif
	}

}
