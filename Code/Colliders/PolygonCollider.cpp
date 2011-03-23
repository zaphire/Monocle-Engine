#include "PolygonCollider.h"

namespace Monocle
{
	PolygonCollider::PolygonCollider(Vector2 offset)
	{
		this->offset = offset;
	}

	ColliderType PolygonCollider::GetColliderType()
	{
		return CT_POLYGON;
	}

	bool PolygonCollider::IntersectsPoint(const Vector2& point)
	{
		if (IsEmpty())
			return false;

		//TODO!

		return false;
	}

	bool PolygonCollider::IntersectsLine(const Vector2& start, const Vector2& end)
	{
		if (IsEmpty())
			return false;

		//TODO!

		return false;
	}

	bool PolygonCollider::IsEmpty()
	{
		return (points.size() == 0);
	}

	void PolygonCollider::AddPoint(Vector2 point)
	{
		points.push_back(point);
	}

	int PolygonCollider::GetPointCount()
	{
		return points.size();
	}
}