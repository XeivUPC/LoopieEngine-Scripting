#pragma once
#include "Loopie/Math/OctreeNode.h"

#include <memory>
#include <array>


namespace Loopie {
	constexpr int MAX_ENTITIES_PER_NODE = 8;
	constexpr int MAXIMUM_DEPTH = 5;

	class Entity;
	struct Frustum;

	struct OctreeStatistics
	{
		int totalNodes = 0;
		int leafNodes = 0;
		int internalNodes = 0;
		int totalEntities = 0;
		int maxDepth = 0;
		int minEntitiesPerNode = INT_MAX;
		int maxEntitiesPerNode = 0;
		float averageEntitiesPerNode = 0.0f;
		int emptyNodes = 0;
		int overfilledNodes = 0;
	};

	class Octree
	{
	public:
		explicit Octree(const AABB& rootBounds);
		~Octree() = default;

		void Insert(std::shared_ptr<Entity> entity);
		void Remove(std::shared_ptr<Entity> entity);
		void Update(std::shared_ptr<Entity> entity);
		void Clear();
		void Rebuild();

		void CollectIntersectingObjectsWithRay(vec3 rayOrigin, vec3 rayDirection, std::unordered_set<std::shared_ptr<Entity>>& entities);
		void CollectIntersectingObjectsWithAABB(const AABB& queryBox, std::unordered_set<std::shared_ptr<Entity>>& entities);
		void CollectIntersectingObjectsWithSphere(const vec3& center, const float& radius, std::unordered_set<std::shared_ptr<Entity>>& entities);
		void CollectVisibleEntitiesFrustum(const Frustum& frustum, std::unordered_set<std::shared_ptr<Entity>>& visibleEntities);
		void CollectAllEntities(std::unordered_set<std::shared_ptr<Entity>>& entities);

		void DebugDraw(const vec4& color);
		void DebugPrintOctreeStatistics();
		void DebugPrintOctreeHierarchy();

		OctreeStatistics GetStatistics() const;

		void SetShouldDraw(bool value);
		void ToggleShouldDraw();
		bool GetShouldDraw() const;


	private:
		AABB GetEntityAABB(const std::shared_ptr<Entity>& entity) const;
		void InsertRecursively(OctreeNode* node, std::shared_ptr<Entity> entity, const AABB& entityAABB, int depth);
		void RemoveRecursively(OctreeNode* node, std::shared_ptr<Entity> entity, const AABB& entityAABB);
		void Subdivide(OctreeNode* node);
		void RedistributeEntities(OctreeNode* node, int depth);
		std::array<AABB, OCTREE_CHILD_COUNT> ComputeChildAABBs(const AABB& parentAABB) const;
		void DebugDrawRecursively(OctreeNode* node, const vec4& color, int depth);
		void DebugPrintOctreeHierarchyRecursively(OctreeNode* node, int depth) const;
		void GatherStatisticsRecursively(OctreeNode* node, OctreeStatistics& stats, int depth) const;

		void CollectAllEntitiesFromNode(OctreeNode* node, std::unordered_set<std::shared_ptr<Entity>>& entities);

		void CollectIntersectingObjectsWithRayRecursively(OctreeNode* node, vec3 rayOrigin, vec3 rayDirection, vec3& rayHit, std::unordered_set<std::shared_ptr<Entity>>& entities);
		void CollectIntersectingObjectsWithAABBRecursively(OctreeNode* node, const AABB& queryBox, std::unordered_set<std::shared_ptr<Entity>>& entities);
		void CollectIntersectingObjectsWithSphereRecursively(OctreeNode* node, const vec3& center, const float& radius, std::unordered_set<std::shared_ptr<Entity>>& entities);
		void CollectVisibleEntitiesFrustumRecursively(OctreeNode* node, const Frustum& frustum, std::unordered_set<std::shared_ptr<Entity>>& visibleEntities);

	private:
		AABB m_worldBounds;
		std::unique_ptr<OctreeNode> m_rootNode;
		bool m_shouldDraw = true;
	};
}