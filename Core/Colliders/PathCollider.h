#pragma once

#include "Collider.h"
#include "../LevelEditor/Node.h"

namespace Monocle
{
	class PathCollider : public Collider
	{
	public:
		PathCollider();
		PathCollider(Node *node, float radius);
		ColliderType GetColliderType();
		bool IntersectsPoint(const Vector2& point, CollisionData *collisionData = NULL);
		bool IntersectsLine(const Vector2& start, const Vector2& end, float lineRadius = 1.0f, CollisionData *collisionData = NULL);

		Node *startNode;

		float radius;
	};
}