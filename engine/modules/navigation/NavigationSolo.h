#pragma once

#include "Navigation.h"

class dtNavMesh;
class dtNavMeshQuery;
class dtCrowd;
class dtObstacleAvoidanceDebugData;
struct rcHeightfield;
struct rcCompactHeightfield;
struct rcContourSet;
struct rcPolyMesh;
struct rcPolyMeshDetail;

namespace Echo
{
	class DataStream;
	class InputGeometryData;

	/**
	* Ѱ·�ӿڷ�װ
	*/
	class NavigationSolo : public Navigation
	{
	public:
		NavigationSolo();
		~NavigationSolo();

		// ����
		virtual bool build(float agentRadius, float agentHeight, float stepHeight);

		// ����
		virtual void load(const String& filePath);

		// ����
		virtual void save(const char* savePath);

	protected:
		// �������
		virtual void cleanup();

	private:
		unsigned char*				m_triareas;
		rcHeightfield*				m_solid;
		rcCompactHeightfield*		m_chf;
		rcContourSet*				m_cset;
		rcPolyMesh*					m_pmesh;
		rcPolyMeshDetail*			m_dmesh;

		unsigned char*				m_navData;				// Ѱ·��������
		ui32						m_navDataSize;			// ���ݴ�С
	};
}