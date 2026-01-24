#include "Octree.h"

#include "Loopie/Math/Frustum.h"
#include "Loopie/Components/MeshRenderer.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Core/Log.h"
#include "Loopie/Render/Colors.h"
#include "Loopie/Render/Gizmo.h"


namespace Loopie
{
	Octree::Octree(const AABB& worldBounds) : m_worldBounds(worldBounds)
	{
		m_rootNode = std::make_unique<OctreeNode>(worldBounds);
	}

	void Octree::Insert(std::shared_ptr<Entity> entity)
	{
		AABB entityAABB = GetEntityAABB(entity);
		InsertRecursively(m_rootNode.get(), entity, entityAABB, 0);
	}

	void Octree::Remove(std::shared_ptr<Entity> entity)
	{
		AABB entityAABB = GetEntityAABB(entity);
		RemoveRecursively(m_rootNode.get(), entity, entityAABB);
	}

	void Octree::Update(std::shared_ptr<Entity> entity)
	{
		Remove(entity);
		Insert(entity);
	}

	void Octree::Clear()
	{
		m_rootNode = std::make_unique<OctreeNode>(m_worldBounds);
	}

	void Octree::Rebuild()
	{
		std::unordered_set<std::shared_ptr<Entity>> allEntities;
		CollectAllEntities(allEntities);

		Clear();

		for (auto& entity : allEntities)
		{
			Insert(entity);
		}
	}

	AABB Octree::GetEntityAABB(const std::shared_ptr<Entity>& entity) const
	{
		auto meshRenderer = entity->GetComponent<MeshRenderer>();
		if (meshRenderer != nullptr)
		{
			return meshRenderer->GetWorldAABB();
		} 
		AABB aabb(entity->GetTransform()->GetPosition());
	}

	void Octree::CollectIntersectingObjectsWithRay(vec3 rayOrigin, vec3 rayDirection,
												   std::unordered_set<std::shared_ptr<Entity>>& entities)
	{
		vec3 rayHit;
		CollectIntersectingObjectsWithRayRecursively(m_rootNode.get(), rayOrigin, rayDirection, rayHit, entities);
	}

	void Octree::CollectIntersectingObjectsWithAABB(const AABB& queryBox,
													std::unordered_set<std::shared_ptr<Entity>>& entities)
	{
		CollectIntersectingObjectsWithAABBRecursively(m_rootNode.get(), queryBox, entities);
	}

	void Octree::CollectIntersectingObjectsWithSphere(const vec3& center, const float& radius,
													  std::unordered_set<std::shared_ptr<Entity>>& entities)
	{
		CollectIntersectingObjectsWithSphereRecursively(m_rootNode.get(), center, radius, entities);
	}

	void Octree::CollectVisibleEntitiesFrustum(const Frustum& frustum, 
											   std::unordered_set<std::shared_ptr<Entity>>& visibleEntities)
	{
		CollectVisibleEntitiesFrustumRecursively(m_rootNode.get(), frustum, visibleEntities);
	}

	void Octree::CollectAllEntities(std::unordered_set<std::shared_ptr<Entity>>& entities)
	{
		CollectAllEntitiesFromNode(m_rootNode.get(), entities);
	}

	void Octree::SetShouldDraw(bool value)
	{
		m_shouldDraw = value;
	}

	void Octree::ToggleShouldDraw()
	{
		m_shouldDraw = !m_shouldDraw;
	}

	bool Octree::GetShouldDraw() const
	{
		return m_shouldDraw;
	}

	
	
	void Octree::InsertRecursively(OctreeNode* node, std::shared_ptr<Entity> entity, const AABB& entityAABB, int depth)
	{
		if (!node)return;
		if (node->m_isLeaf)
		{
			node->m_entities.insert(entity);

			if (node->m_entities.size() > MAX_ENTITIES_PER_NODE &&
				depth < MAXIMUM_DEPTH)
			{
				Subdivide(node);
				RedistributeEntities(node, depth);
			}
			return;
		}

		int intersectingChildren = 0;
		int childIndex = -1;

		for (int i = 0; i < OCTREE_CHILD_COUNT; ++i)
		{
			if (node->m_children[i]->m_aabb.Intersects(entityAABB))
			{
				intersectingChildren++;
				childIndex = i;
				if (intersectingChildren > 1)
				{
					node->m_entities.insert(entity);
					return;
				}
			}
		}

		if (intersectingChildren == 1)
			InsertRecursively(node->m_children[childIndex].get(), entity, entityAABB, depth + 1);
		else
			node->m_entities.insert(entity);
	}

	void Octree::RemoveRecursively(OctreeNode* node, std::shared_ptr<Entity> entity, const AABB& entityAABB)
	{
		if (!node || !node->m_aabb.Intersects(entityAABB))
			return;

		if (node->m_entities.erase(entity))
			return;

		if (node->m_isLeaf)
			return;

		for (int i = 0; i < OCTREE_CHILD_COUNT; ++i)
			RemoveRecursively(node->m_children[i].get(), entity, entityAABB);
	}

	void Octree::Subdivide(OctreeNode* node)
	{
		auto childAABBs = ComputeChildAABBs(node->m_aabb);

		for (int i = 0; i < OCTREE_CHILD_COUNT; ++i)
		{
			node->m_children[i] = std::make_unique<OctreeNode>(childAABBs[i]);
			node->m_children[i]->m_parent = node;
		}

		node->m_isLeaf = false;
	}

	void Octree::RedistributeEntities(OctreeNode* node, int depth)
	{
		auto entities = std::move(node->m_entities);

		for (auto& entity : entities)
			InsertRecursively(node, entity, GetEntityAABB(entity), depth);
	}

	std::array<AABB, OCTREE_CHILD_COUNT> Octree::ComputeChildAABBs(const AABB& parentAABB) const
	{
		std::array<AABB, OCTREE_CHILD_COUNT> children;
		vec3 center = parentAABB.GetCenter();
		vec3 min = parentAABB.MinPoint;
		vec3 max = parentAABB.MaxPoint;

		for (int i = 0; i < OCTREE_CHILD_COUNT; ++i)
		{
			vec3 childMin{
				(i & 1) ? center.x : min.x,
				(i & 2) ? center.y : min.y,
				(i & 4) ? center.z : min.z
			};

			vec3 childMax{
				(i & 1) ? max.x : center.x,
				(i & 2) ? max.y : center.y,
				(i & 4) ? max.z : center.z
			};

			children[i] = AABB(childMin, childMax);
		}

		return children;
	}

	void Octree::DebugDrawRecursively(OctreeNode* node, const vec4& color, int depth)
	{
		if (!node)
		{
			return;
		}

		// If is Leaf and depth is greater than 3, then color is YELLOW
		if (node->m_isLeaf && depth > 3)
		{
			Gizmo::DrawCube(node->m_aabb.MinPoint, node->m_aabb.MaxPoint, Color::YELLOW);
		}
		// The rest's is WHATEVER color contained within var color (normally GREEN)
		else
		{
			Gizmo::DrawCube(node->m_aabb.MinPoint, node->m_aabb.MaxPoint, color);
		}

		for (int i = 0; i < MAX_ENTITIES_PER_NODE; ++i)
		{			
			if (node->m_children[i])
			{
				DebugDrawRecursively(node->m_children[i].get(), color, depth + 1);
			}
		}
	}

	void Octree::DebugPrintOctreeHierarchyRecursively(OctreeNode* node, int depth) const
	{
		if (!node)
		{
			return;
		}
		Log::Info("Node depth = {0}. Entities in node = {1}", depth, node->m_entities.size());

		if (node->m_isLeaf)
		{
			return;
		}

		for (int i = 0; i < MAX_ENTITIES_PER_NODE; ++i)
		{
			DebugPrintOctreeHierarchyRecursively(node->m_children[i].get(), depth + 1);
		}
	}

	void Octree::GatherStatisticsRecursively(OctreeNode* node, OctreeStatistics& stats, int depth) const
	{
		if (!node)
		{
			return;
		}

		stats.totalNodes++;
		stats.maxDepth = std::max(stats.maxDepth, depth);

		int entityCount = static_cast<int>(node->m_entities.size());
		stats.totalEntities += entityCount;

		stats.minEntitiesPerNode = std::min(stats.minEntitiesPerNode, entityCount);
		stats.maxEntitiesPerNode = std::max(stats.maxEntitiesPerNode, entityCount);

		if (entityCount == 0)
		{
			stats.emptyNodes++;
		}

		if (entityCount > MAX_ENTITIES_PER_NODE && depth >= MAXIMUM_DEPTH)
		{
			stats.overfilledNodes++;
		}

		if (node->m_isLeaf)
		{
			stats.leafNodes++;
		}
		else
		{
			stats.internalNodes++;

			for (int i = 0; i < MAX_ENTITIES_PER_NODE; ++i)
			{
				if (node->m_children[i])
				{
					GatherStatisticsRecursively(node->m_children[i].get(), stats, depth + 1);
				}
			}
		}
	}

	void Octree::CollectAllEntitiesFromNode(OctreeNode* node, std::unordered_set<std::shared_ptr<Entity>>& entities)
	{
		if (!node)
		{
			return;
		}

		for (const auto& entity : node->m_entities)
		{
			entities.insert(entity);
		}

		if (!node->m_isLeaf)
		{
			for (int i = 0; i < MAX_ENTITIES_PER_NODE; ++i)
			{
				if (node->m_children[i])
				{
					CollectAllEntitiesFromNode(node->m_children[i].get(), entities);
				}
			}
		}
	}

	void Octree::CollectIntersectingObjectsWithRayRecursively(OctreeNode* node, vec3 rayOrigin, vec3 rayDirection, vec3& rayHit,
															  std::unordered_set<std::shared_ptr<Entity>>& entities)
	{
		if (!node)
		{
			return;
		}

		// Early exit - if the ray doesn't intersect with this node's AABB, skip it and its children.
		if (!node->m_aabb.IntersectsRay(rayOrigin, rayDirection, rayHit))
		{
			return;
		}

		for (const auto& entity : node->m_entities)
		{
			if (GetEntityAABB(entity).IntersectsRay(rayOrigin, rayDirection, rayHit))
			{
				entities.insert(entity);
			}
		}

		if (node->m_isLeaf)
		{
			return;
		}

		for (int i = 0; i < MAX_ENTITIES_PER_NODE; ++i)
		{
			if (node->m_children[i])
			{
				CollectIntersectingObjectsWithRayRecursively(node->m_children[i].get(), rayOrigin, rayDirection, rayHit, entities);
			}
		}
	}

	void Octree::CollectIntersectingObjectsWithAABBRecursively(OctreeNode* node, const AABB& queryBox,
																std::unordered_set<std::shared_ptr<Entity>>& entities)
	{
		if (!node)
		{
			return;
		}

		// Early exit - if the queryBox' AABB doesn't intersect with this node's AABB, skip it and its children.
		if (!queryBox.Intersects(node->m_aabb))
		{
			return;
		}

		for (const auto& entity : node->m_entities)
		{
			if (GetEntityAABB(entity).Intersects(queryBox))
			{
				entities.insert(entity);
			}
		}

		if (node->m_isLeaf)
		{
			return;
		}

		for (int i = 0; i < MAX_ENTITIES_PER_NODE; ++i)
		{
			if (node->m_children[i])
			{
				CollectIntersectingObjectsWithAABBRecursively(node->m_children[i].get(), queryBox, entities);
			}
		}
	}

	void Octree::CollectIntersectingObjectsWithSphereRecursively(OctreeNode* node, const vec3& center, const float& radius,
																 std::unordered_set<std::shared_ptr<Entity>>& entities)
	{
		if (!node)
		{
			return;
		}

		// Early exit - if the sphere doesn't intersect with this node's AABB, skip it and its children.
		if (!node->m_aabb.IntersectsSphere(center, radius))
		{
			return;
		}

		for (const auto& entity : node->m_entities)
		{
			if (GetEntityAABB(entity).IntersectsSphere(center, radius))
			{
				entities.insert(entity);
			}
		}

		if (node->m_isLeaf)
		{
			return;
		}

		for (int i = 0; i < MAX_ENTITIES_PER_NODE; ++i)
		{
			if (node->m_children[i])
			{
				CollectIntersectingObjectsWithSphereRecursively(node->m_children[i].get(), center, radius, entities);
			}
		}
	}

	void Octree::CollectVisibleEntitiesFrustumRecursively(OctreeNode* node, const Frustum& frustum,
														  std::unordered_set<std::shared_ptr<Entity>>& visibleEntities)
	{
		if (!node)
		{
			return;
		}

		// Early exit - if the frustum' AABB doesn't intersect with this node's AABB, skip it and its children.
		if (!frustum.Intersects(node->m_aabb))
		{
			return;
		}

		for (const auto& entity : node->m_entities)
		{
			if (frustum.Intersects(GetEntityAABB(entity)))
			{
				visibleEntities.insert(entity);
			}
		}

		if (node->m_isLeaf)
		{
			return;
		}

		for (int i = 0; i < MAX_ENTITIES_PER_NODE; ++i)
		{
			if (node->m_children[i])
			{
				CollectVisibleEntitiesFrustumRecursively(node->m_children[i].get(), frustum, visibleEntities);
			}
		}
	}

	void Octree::DebugDraw(const vec4& color)
	{
		if (m_shouldDraw)
		{
			DebugDrawRecursively(m_rootNode.get(), color, 0);
		}
	}

	void Octree::DebugPrintOctreeStatistics()
	{
		OctreeStatistics statistics = GetStatistics();
		Log::Info("==========================");
		Log::Info("Printing Octree Statistics");
		Log::Info("==========================");
		Log::Info("Total Nodes = {0}", statistics.totalNodes);
		Log::Info("Leaf Nodes = {0}", statistics.leafNodes);
		Log::Info("Internal Nodes = {0}", statistics.internalNodes);
		Log::Info("Total Entities = {0}", statistics.totalEntities);
		// The line below could be implemented with a few changes.
		//Log::Info("Total Visible Entities = {0}", statistics.totalEntities); 
		Log::Info("Max Depth = {0}", statistics.maxDepth);
		Log::Info("Min Entities Per Node = {0}", statistics.minEntitiesPerNode);
		Log::Info("Max Entities Per Node = {0}", statistics.maxEntitiesPerNode);
		Log::Info("Average Entities Per Node = {0}", statistics.averageEntitiesPerNode);
		Log::Info("Empty Leaves = {0}", statistics.emptyNodes);
		Log::Info("Overfilled Leaves = {0}", statistics.overfilledNodes);
	}

	void Octree::DebugPrintOctreeHierarchy()
	{
		Log::Info("==========================");
		Log::Info("Printing Octree Hierarchy");
		Log::Info("==========================");

		DebugPrintOctreeHierarchyRecursively(m_rootNode.get(), 0);
	}

	OctreeStatistics Octree::GetStatistics() const
	{
		OctreeStatistics stats;
		GatherStatisticsRecursively(m_rootNode.get(), stats, 0);

		if (stats.leafNodes > 0)
		{
			stats.averageEntitiesPerNode = static_cast<float>(stats.totalEntities) / stats.leafNodes;
		}

		return stats;
	}
}