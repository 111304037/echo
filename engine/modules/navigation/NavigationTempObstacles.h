#pragma once

#include "Navigation.h"

class dtNavMesh;
class dtNavMeshQuery;
class dtCrowd;
struct rcHeightfield;
struct rcCompactHeightfield;
struct rcContourSet;
struct rcPolyMesh;	
struct rcPolyMeshDetail;
struct rcChunkyTriMesh;

namespace Echo
{
	class DataStream;
	class InputGeometryData;
	class BuildContext;

	/**
	 * Ѱ·�ӿڷ�װ
	 */
	class NavigationTempObstacles : public Navigation
	{
	public:
		NavigationTempObstacles();
		~NavigationTempObstacles();

		// ��������ͼ
		virtual bool build( float agentRadius, float agentHeight, float stepHeight);

		// ÿ֡����
		virtual void update(float delta);

		// ����
		virtual void load(const String& filePath);

		// ����
		virtual void save( const char* savePath);

	public:
		// ���Բ���ϰ���
		void addTempObstacleCylinder(const Vector3& pos, float radius, float height);

		// ��Ӻ����ϰ���
		void addTempObstacleBox(const Vector3& vMin, const Vector3& vMax);

		// ��������Χ���ϰ���
		void addTempObstacleObb(const Vector3& p0, const Vector3& p1, float width, float height);

		// �Ƴ��ϰ���
		void removeTempObstacle(const float* sp, const float* sq);

		// ��������ϰ���
		void clearAllTempObstacles();

	protected:
		// �ֽ�������Ϊ���鼯��
		int rasterizeTileLayers(const rcChunkyTriMesh* chunkyMesh, const int tx, const int ty, const rcConfig& cfg, struct TileCacheData* tiles, const int maxTiles);

		// ��յ���ͼ
		virtual void cleanup();

	private:
		struct LinearAllocator*		m_talloc;
		struct FastLZCompressor*	m_tcomp;
		struct MeshProcess*			m_tmproc;
		class dtTileCache*			m_tileCache;
		int							m_maxTiles;
		int							m_maxPolysPerTile;
		float						m_tileSize;

		int							m_cacheCompressedSize = 0;
		int							m_cacheRawSize;
		int							m_cacheLayerCount = 0;
		int							m_cacheBuildMemUsage = 0;

		bool						m_filterLowHangingObstacles;
		bool						m_filterLedgeSpans = true;
		bool						m_filterWalkableLowHeightSpans;

		struct dtNavMeshData*		m_navMeshData = nullptr;
		struct dtTileCacheData*		m_tileCacheData = nullptr;
	};
}
