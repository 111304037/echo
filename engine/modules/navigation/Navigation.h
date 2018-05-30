#pragma once

#include <Recast/Recast/Recast.h>
#include <Recast/Recast/DetourNavMesh.h>
#include <Recast/Recast/DetourNavMeshQuery.h>
#include <Recast/Recast/DetourCrowd.h>
#include <engine/core/Math/Math.h>

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

	/// These are just sample areas to use consistent values across the samples.
	/// The use should specify these base on his needs.
	enum SamplePolyAreas
	{
		SAMPLE_POLYAREA_GROUND,
		SAMPLE_POLYAREA_WATER,
		SAMPLE_POLYAREA_ROAD,
		SAMPLE_POLYAREA_DOOR,
		SAMPLE_POLYAREA_GRASS,
		SAMPLE_POLYAREA_JUMP,
		SAMPLE_POLYAREA_TYPE_0,
		SAMPLE_POLYAREA_TYPE_1,
		SAMPLE_POLYAREA_TYPE_2,
		SAMPLE_POLYAREA_TYPE_3,
		SAMPLE_POLYAREA_TYPE_4,
		SAMPLE_POLYAREA_TYPE_5,
		SAMPLE_POLYAREA_TYPE_6,
		SAMPLE_POLYAREA_TYPE_7,
		SAMPLE_POLYAREA_TYPE_8,
		SAMPLE_POLYAREA_TYPE_9,
		SAMPLE_POLYAREA_TYPE_10,
	};
	enum SamplePolyFlags
	{
		SAMPLE_POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
		SAMPLE_POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
		SAMPLE_POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
		SAMPLE_POLYFLAGS_JUMP = 0x08,		// Ability to jump.
		SAMPLE_POLYFLAGS_TYPE_0 = 0x10,
		SAMPLE_POLYFLAGS_TYPE_1 = 0x20,
		SAMPLE_POLYFLAGS_TYPE_2 = 0x40,
		SAMPLE_POLYFLAGS_TYPE_3 = 0x80,
		SAMPLE_POLYFLAGS_TYPE_4 = 0x100,
		SAMPLE_POLYFLAGS_TYPE_5 = 0x200,
		SAMPLE_POLYFLAGS_TYPE_6 = 0x400,
		SAMPLE_POLYFLAGS_TYPE_7 = 0x800,
		SAMPLE_POLYFLAGS_TYPE_8 = 0x1000,
		SAMPLE_POLYFLAGS_TYPE_9 = 0x2000,
		SAMPLE_POLYFLAGS_TYPE_10 = 0x4000,

		SAMPLE_POLYFLAGS_DISABLED = 0x8000,	// Disabled polygon
		SAMPLE_POLYFLAGS_ALL = 0xffff	// All abilities.
	};


	/**
	* Ѱ·�ӿڷ�װ
	*/
	class Navigation
	{
	public:
		virtual ~Navigation();

		// ÿ֡����
		virtual void update(float delta) {}

		// ��ȡ��������
		const dtNavMesh* getNavMesh() const { return m_navMesh; }

		// ��ȡѰ·��ѯ��(����NavMeshѰ·)
		const dtNavMeshQuery* getNavMeshQuery() const { return m_navQuery; }

	public:
		// Ѱ·
		void findPath(const Vector3& startPos, const Vector3& endPos, float*& path, int& pathCount, int type = 0);
		void findPath(const float spos[], const float espos[], float*& smoothPath, int & nsmoothPath, int include_flags);
		void findStraightPath(const Vector3& startPos, const Vector3& endPos, float*& path, int& pathCount);

		// ������ĳλ������Ķ����
		bool findNearestPoly(const Vector3& position, const Vector3& ext, Vector3& nearest);

		//��Ŀ�������ʼ���������һ��������֮�����Ŀ����������Ч�㣬���벻Ҫ̫��
		bool findNearestPolyBetween(const Vector3& start, const Vector3& end, Vector3& nearest, float deltaY = 10.f, float step = 2.f);

		// ���ÿ����ߵ���������
		void setIncludeFlag(unsigned int nFlag);

		// ���ò������ߵ���������
		void setExcludeFlag(unsigned int nFlag);

		void setCrowdIncludeFlag(int type, unsigned int nFlag);

		void setAgentIncludeFlagType(int agentId, unsigned char type);

		// ������������
		void setAreaCost(int i, float cost);

		// ��ѯ��ؽӿ�
	public:
		bool rayCast(const Vector3& startPos, const Vector3& endPos, float& oLength, Vector3& hitPosition);

		/**
		* Casts a 'walkability' ray along the surface of the navigation mesh from
		* the start position toward the end position.
		*/
		bool rayCast(const Vector3& startPos, const Vector3& dir, float& distance, Vector3& endPos, int include_flags);

		/**
		* ���߼��(�������뵼��ģ���ཻ�����)
		* ����[in]		startPos	�������
		* ����[in]		dir			���߳���(��ص�λ��)
		* ����[in out]  oLenght		���߳���(̽�ⳤ��Խ�̣�Ч��Խ��)
		* ����[out]     oHitPosition�ཻ���(�����)
		*/
		bool rayDetect(const Vector3& startPos, const Vector3& dir, float& oLength, Vector3& oHitPosition) const;

		/**
		* ���߼��(����������ĳ������ཻ��)
		* ����[in]		startPos	�������
		* ����[in]		dir			���߳���(��ص�λ��)
		* ����[in out]  oLenght		���߳���(̽�ⳤ��Խ�̣�Ч��Խ��)
		* ����[in]		polyRef		�����ID
		* ����[out]     oHitPosition�ཻ���
		* ����ֵ	 true==�ཻ��false==���ཻ
		*/
		bool rayDetectPoly(const Vector3& startPos, const Echo::Vector3& dir, float& oLength, dtPolyRef polyRef, Vector3& oHitPosition) const;

		// Ⱥ��Ѱ·��ؽӿ�
	public:
		void crowdInit(float agentRadius = 0.6f);

		void setQueryExtents(float agentRadius = 0.6f);

		// ��ֹ�����ƶ�
		void crowdStopAgentMove(int nAgentIndex = 0);

		// ���Ⱥ��Ѱ·����
		int	crowdAddAgent(const Vector3& position, float weight = 0.f, float speed = 3.5f, float radius = 0.f, float maxAcceleration = 10000.f);

		void setDoCollide(bool docollide);
		// �����Ƿ��ڻ�Ծ״̬
		bool crowdIsAgentActive(const int agentId);

		// �Ƴ�����
		void crowdRemoveAgent(int nAgentIndex);

		// �Ƴ�����Ⱥ��Ѱ·����
		void crowdRemoveAllAgents();

		// Ĭ��������
		void crowdMoveAgentToword(const Vector3& dir, int nAgentIndex = 0);

		// ��ȡ����λ��
		Vector3 crowdGetAgentPosition(int nAgentIndex = 0);

		// ���ô���λ��
		void crowdResetAgentPosition(int nAgentIndex, Echo::Vector3 pos);

		// �������д���
		void crowdUpdateAllAgents(ui32 nElapes);

		// ����ָ������
		void crowdUpdateAgentOnly(ui32 nElapes, int idx);

		// ����Ⱥ��Ѱ·Ŀ���
		void crowdUpdateAgentTarget(const float* p);

		// ���ô�����ƶ�Ŀ��
		void crowdSetAgentTarget(int aIndex);

		// �Ƴ�����Ŀ���
		void crowdRemoveAgentTarget(int aIndex);

	public:
		// ���ü�����
		void setGeometry(InputGeometryData* inputData) { m_geom = inputData; }

		// ������������
		virtual bool build(float agentRadius, float agentHeight, float stepHeight) = 0;

		// ��յ�������
		virtual void cleanup() {}

		// build navigation mesh from imort data
		virtual void load(const String& filePath) = 0;

		// ����
		virtual void save(const char* savePath) = 0;

		// �Ƿ��Ѽ���
		bool isLoaded() const { return m_isLoaded; }

	protected:
		Navigation();

		// ����ͨ������
		void resetCommonSettings();

		// ���߼��
		bool rayDetect(dtNavMeshQuery* query, const Vector3& startPos, const Vector3& dir, float& oLength, const dtQueryFilter* filter, Echo::Vector3& oHitPosition) const;
	
	protected:
		InputGeometryData*			m_geom;					// ��������Ѱ·���ݵļ�����
		bool						m_isLoaded;				// �Ƿ��Ѽ��ص�������
		dtNavMesh*					m_navMesh;				// ��������
		dtNavMeshQuery*				m_navQuery;

		dtObstacleAvoidanceDebugData*m_vod;
		dtCrowdAgentDebugInfo		m_agentDebug;
		dtCrowd*					m_crowd;

		float						m_cellSize;
		float						m_cellHeight;
		float						m_agentHeight;
		float						m_agentRadius;
		float						m_agentMaxClimb;
		float						m_agentMaxSlope;
		float						m_regionMinSize;
		float						m_regionMergeSize;
		float						m_edgeMaxLen;
		float						m_edgeMaxError;
		float						m_vertsPerPoly;
		float						m_detailSampleDist;
		float						m_detailSampleMaxError;
		bool						m_monotonePartitioning;

		class BuildContext*			m_ctx;					// ����Ѱ·ͼ����

		float						m_targetPos[3];			// Ѱ·Ŀ��λ��
		dtPolyRef					m_targetRef;

		float						m_spos[3];
		float						m_epos[3];
		dtQueryFilter				m_filter;

		static const int			MAX_POLYS = 256;
		dtPolyRef					m_startRef;
		dtPolyRef					m_endRef;
		dtPolyRef					m_polys[MAX_POLYS];
		int							m_npolys;
		float						m_polyPickExt[3];

		float						m_straightPath[MAX_POLYS * 3];
		unsigned char				m_straightPathFlags[MAX_POLYS];
		dtPolyRef					m_straightPathPolys[MAX_POLYS];
		int							m_nstraightPath;

		// path finder data
		static const int			MAX_SMOOTH = 2048;
		float						m_smoothPath[MAX_SMOOTH * 3];
		int							m_nsmoothPath;
	};
}