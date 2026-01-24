#pragma once
#include "Loopie/Math/AABB.h"

#include <memory>
#include <array>
#include <unordered_set>

namespace Loopie
{
	constexpr int OCTREE_CHILD_COUNT = 8;

	class Entity;

	class OctreeNode
	{
	friend class Octree;
	public:
		explicit OctreeNode(const AABB& bounds);
		~OctreeNode() = default;

	private:
		AABB m_aabb;
		//std::vector<std::shared_ptr<Entity>> m_entities = {};
		std::unordered_set<std::shared_ptr<Entity>> m_entities;

		OctreeNode* m_parent = nullptr;
		std::array<std::unique_ptr<OctreeNode>, OCTREE_CHILD_COUNT> m_children = {};
		bool m_isLeaf = true;
	};
}