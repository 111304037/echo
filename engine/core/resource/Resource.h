#pragma once

#include <engine/core/Memory/MemManager.h>
#include <engine/core/Thread/Threading.h>
#include <engine/core/io/DataStream.h>
#include "engine/core/Base/EchoDef.h"

// �汾��
#define VERSION_ORIGIN_BASE 1
#define VERSION_HALF_FLOAT_BASE 100
#define VERSION_EXTAND_MATERIAL 200

#define VERSION_SKELETON_BASE 1
#define VERSION_SKELETON_SUPPORT_SCALE 2

#define VERSION_ANIM_BASE 1
#define VERSION_ANIM_SEPERATE 2
#define VERSION_ANIM_SUPPORT_SCALE 3
#define VERSION_ANIM_ADD_BONEWEIGHTS 4

#define RESOURCE_USE_HALF_FLOAT 1

namespace Echo
{
	// ��Դ����
	enum ResourceType
	{
		RT_Unknown = 0,		// δ֪
		RT_Texture,			// ����
	};

	/**
	 * ��Դ�ӿ�
	 */
	class ResourceManager;
	class Resource
	{
	public:
		EE_AUTO_MUTEX

		// ����״̬
		enum LoadingState
		{
			LOADSTATE_CREATED,		// ��Դ�Ѵ���
			LOADSTATE_DOWNLOADING,	// ��Դ������(http archive etc)
			LOADSTATE_PREPARED,		// ׼���������(�Ѽ��ص��ڴ�)
			LOADSTATE_LOADED,		// ��Դ�������(��ʹ��)
			LOADSTATE_DESTORY		// ������
		};

		friend class ResourceManager;

	protected:
		Resource(ResourceType type);

		virtual bool prepareImpl(DataStream* stream) = 0;
		virtual void unprepareImpl(void) {}
		virtual bool loadImpl(void) = 0;
		virtual void unloadImpl(void) = 0;
		virtual size_t calculateSize(void) const = 0;

	public:
		Resource(const String& name, ResourceType type, bool isManual = false);
		virtual ~Resource();

		// ׼����Դ
		bool prepare();

		// ׼����Դ(����֤ͬ��)
		bool prepare(DataStream* stream);

		// ����(ͬ��)
		virtual bool load();

		// ��Դ����(������Դͬ��,������Դ�첽)
		bool prepareLoad();

		// ���¼���
		virtual bool reload();

		/** Returns true if the Resource is reloadable, false otherwise.
		*/
		virtual bool isReloadable(void) const { return !mIsManual;}

		/** Is this resource manually loaded?
		*/
		virtual bool isManuallyLoaded(void) const { return mIsManual;}

		void delayedRelease();
		void reserveDelayed();
		ui32 addDelayedTime(ui32 deltaTime);
		void resetDelayedTime() { mDelayedTime = 0; };

		/** Unloads the resource; this is not permanent, the resource can be reloaded later if required.*/
		virtual void unload(void);

		/** Retrieves info about the size of the resource.
		*/
		virtual size_t getSize(void) const { return mSize; }

		/** Gets resource name.*/
		virtual const String& getName(void) const { return mName; }

		/** Returns true if the Resource has been loaded, false otherwise.  No lock required to read this state since no modify*/
		virtual bool isLoaded(void) const { return (mLoadingState == LOADSTATE_LOADED); }

		// ������Դ״̬
		LoadingState getLoadingState() const { return mLoadingState; }

		// ��ȡ����
		ResourceType getType() const { return m_resourceType; }

		// ������ü���
		void addRefrenceCount();

	private:
		// �������ü���
		bool subRefrenceCount();

	protected:		
		String			mName;				// Unique name of the resource	
		LoadingState	mLoadingState;		// Is the resource currently loaded?
		size_t			mSize;				// The size of the resource in bytes
		bool			mIsManual;			// Is this file manually loaded?
		volatile int	mRefrenceCount;		// ���ü���
		EE_MUTEX(m_refrenceCountMutex);
		ui32			mDelayedTime;		// �ӳ��ͷ�ʱ��
		LoadingState	mOldLoadingState;	// �ɼ���״̬
		ResourceType	m_resourceType;		// ��Դ����
	};
}
