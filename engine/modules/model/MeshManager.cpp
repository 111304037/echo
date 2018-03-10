#include "MeshManager.h"
#include "engine/core/Util/Exception.h"
#include "engine/core/Util/PathUtil.h"
#include "engine/core/Util/LogManager.h"
#include "SkinnedMesh.h"
#include "DynamicMesh.h"

namespace Echo
{
	static map<String, MeshManager::Creater>::type s_engine_outer_custom_mesh_creater_list;  //see the name carefully

	__ImplementSingleton(MeshManager);

	// ���캯��
	MeshManager::MeshManager()
	{
		__ConstructSingleton;
	}

	// ��������
	MeshManager::~MeshManager()
	{
		__DestructSingleton;
		// subclasses should unregister with resource group manager
	}

	// ����ģ��
	Mesh* MeshManager::createMesh(const String& name, bool needBackUp)
	{
		Mesh* pMesh = (Mesh*)ResourceManager::createResource(name, false);
	
		// trick һ����Դ����ķ�ʽ����Щģ�;Ͳ�����Դ���ã���Ҫһ����Դһ��ʵ������Ҫ��������mesh����һ�㶼����������
		if (pMesh->needCloneNew())
		{
			Mesh* pCloneMesh = pMesh->cloneNew();
			if (pCloneMesh)
			{
				pCloneMesh->addRefrenceCount();
				{	
					EE_LOCK_MUTEX(mResourcesMutex)
					mResources.insert(ResourceMap::value_type(pMesh->getCloneName(), pCloneMesh)); 
				}
				pCloneMesh->m_bNeedBackUp = needBackUp;
				return pCloneMesh;
			}
		}

		// Mesh�����Ƿ���Ҫ����
		pMesh->m_bNeedBackUp = needBackUp;

		return pMesh;
	}

	void MeshManager::registerCustomMeshCreater(const String& name, Creater creater)
	{
		s_engine_outer_custom_mesh_creater_list[name] = creater;
	}

	Resource* MeshManager::createImpl(const String& name, bool isManual)
	{
		String ext = PathUtil::GetFileExt(name);
		bool isSkinMesh = (ext == "skin");
		bool isDynamicMesh = (ext == "dmesh");
		bool isInstanceMesh = (ext == "instmesh");
		if (isSkinMesh)
		{
			return EchoNew(SkinnedMesh(name));
		}
		else if (isDynamicMesh)
		{
			return EchoNew(DynamicMesh(name));
		}
		else if (isInstanceMesh)
		{
			return EchoNew(InstanceMesh(name));
		}
		else
		{
			map<String, Creater>::iterator itr = s_engine_outer_custom_mesh_creater_list.begin();
			for (; itr != s_engine_outer_custom_mesh_creater_list.end(); itr++)
			{
				Resource* custom_mesh = itr->second(name);
				if (custom_mesh != NULL)
				{
					return custom_mesh;
				}
			}
			return EchoNew(Mesh(name));
		}
	}
}
