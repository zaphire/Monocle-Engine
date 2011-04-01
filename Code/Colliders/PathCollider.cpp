#include "PathCollider.h"

namespace Monocle
{
	PathCollider::PathCollider() : Collider()
	{
	}

	ColliderType PathCollider::GetColliderType()
	{
		return CT_PATH;
	}

	bool PathCollider::IntersectsPoint(const Vector2& point)
	{
		return false;
	}

	bool PathCollider::IntersectsLine(const Vector2& start, const Vector2& end, float lineRadius)
	{
		return false;
	}
}