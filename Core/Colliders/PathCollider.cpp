#include "PathCollider.h"

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

	bool PathCollider::IntersectsLine(const Vector2& start, const Vector2& end, float radius, CollisionData *collisionData)
	{
		Node *node = startNode;
		while(node)
		{
			Node *next = node->GetNext();
			if (next)
			{
				if (LinesIntersect(start, end, node->position, next->position))
				{
					return true;
				}
			}
			node = next;
		}
		return false;
	}
}