#ifndef __ECHO_RESOURCE_MANAGER_H__
#define __ECHO_RESOURCE_MANAGER_H__

#include "Resource.h"

namespace Echo
{
	/**
	 * ��Դ������(�̰߳�ȫ)
	 */
	class ResourceManager
	{
	public: 
		typedef map< String, Resource* >::type ResourceMap;
		typedef std::pair<Resource*, bool> ResourceCreateOrRetrieveResult;

	public:
		ResourceManager();
		virtual ~ResourceManager();

		// ��ȡ�ڴ�ʹ��״̬
		virtual size_t getMemoryUsage(void) const { return mMemoryUsage; }

		// create resource
		virtual Resource* createResource(const String& name, bool isManual = false);

		// �ͷ���Դ 
		virtual bool releaseResource(Resource* r, bool isUnloadImmediately=false);

		// �������ƻ�ȡ��Դ
		virtual Resource* getByName(const String& name);

		void setReleaseDelayTime(ui32 t);

		ui32 getReleaseDelayTime() const;

		virtual void updateDelayedRelease(ui32 frameTime);

		virtual void releaseAllDelayed();

		void outputResourceLog(const String& managerType);

		// ���¼���ָ�����͵���Դ
		void reloadResources( ResourceType type);

		// �������ƻ�ȡ��Դ
		static Resource* getResource( const String& name);

		// ��ȡ��Դ����
		i32 getResourceNum() { return mResources.size(); }

	protected:
		// ��������Դ
		virtual Resource* createNewResource(const String& name, bool isManual = false);

		virtual ResourceCreateOrRetrieveResult createOrRetrieve(const String& name, bool isManual = false);

		// ��Դ����ʵ��
		virtual Resource* createImpl(const String& name, bool isManual)=0;

	protected:
		EE_MUTEX(	mResourcesMutex);		// ��Դ����������
		ResourceMap mResources;
		EE_MUTEX(	mResourcesDelayMutex);
		ResourceMap mResources_delay;
		size_t		mMemoryUsage;		// In bytes
		ui32		mReleaseDelayTime;
	};
}

#endif
