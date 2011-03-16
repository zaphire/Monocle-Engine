#include "CircleCollider.h"

namespace Monocle
{
	CircleCollider::CircleCollider(float radius, Vector2 offset)
		: Collider()
	{
		this->offset	= offset;
		this->radius	= radius;
	}

	ColliderType CircleCollider::GetColliderType()
	{
		return CT_CIRCLE;
	}

	bool CircleCollider::IntersectsPoint(const Vector2& point)
	{
		//TODO: take into account entity position
		Vector2 diff = point - offset;
		return (diff.IsInRange(radius));
	}

	bool CircleCollider::IntersectsLine(const Vector2& start, const Vector2& end)
	{
		//TODO!
		return false;
	}
}