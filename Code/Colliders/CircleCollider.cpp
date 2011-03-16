#include "CircleCollider.h"

namespace Monocle
{
	CircleCollider::CircleCollider(Vector2 position, float radius)
	{
		this->position	= position;
		this->radius	= radius;
	}

	ColliderType CircleCollider::GetColliderType()
	{
		return CT_CIRCLE;
	}

	bool CircleCollider::IntersectsPoint(const Vector2& point)
	{
		Vector2 diff = point - position;
		return (diff.IsInRange(radius));
	}

	bool CircleCollider::IntersectsLine(const Vector2& start, const Vector2& end)
	{
		//TODO!
		return false;
	}
}