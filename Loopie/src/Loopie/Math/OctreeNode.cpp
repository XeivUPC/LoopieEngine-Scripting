#include "OctreeNode.h"

#include "Loopie/Components/MeshRenderer.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Scene/Entity.h"


namespace Loopie
{
	OctreeNode::OctreeNode(const AABB& bounds):m_aabb(bounds){}
}