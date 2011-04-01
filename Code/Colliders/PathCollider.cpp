#include "PathCollider.h"

namespace Monocle
{
	PathCollider::PathCollider() : Collider(), startNode(NULL)
	{
	}

	PathCollider::PathCollider(Node *node) : Collider(), startNode(node)
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