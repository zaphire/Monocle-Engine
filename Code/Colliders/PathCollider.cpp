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

	bool PathCollider::IntersectsLine(const Vector2& start, const Vector2& end, float lineRadius, CollisionData *collisionData)
	{
		return false;
	}
}