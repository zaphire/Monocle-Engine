#include "PathCollider.h"
#include "CollisionData.h"

namespace Monocle
{
	PathCollider::PathCollider()
		: Collider(), startNode(NULL), radius(0.0f)
	{
	}

	PathCollider::PathCollider(Node *node, float radius)
		: Collider(), startNode(node), radius(radius)
	{
	}

	ColliderType PathCollider::GetColliderType()
	{
		return CT_PATH;
	}

	bool PathCollider::IntersectsPoint(const Vector2& point, CollisionData *collisionData)
	{
		return false;
	}

	// ignores radius for now
	bool PathCollider::IntersectsLine(const Vector2& start, const Vector2& end, float radius, CollisionData *collisionData)
	{
		Node *node = startNode;
		while(node)
		{
			Node *next = node->GetNext();
			if (next)
			{
				if (node->variant != -1)
				{
					if (LinesIntersect(node->GetWorldPosition(), next->GetWorldPosition(), start, end, collisionData))
					{
						collisionData->collider = this;
						return true;
					}
				}
			}
			node = next;
		}
		return false;
	}
}