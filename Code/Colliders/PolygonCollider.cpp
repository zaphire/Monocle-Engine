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
		//Polygons don't collide if they have less than 3 points
		if (points.size() <= 2)
			return false;

		// If it's past any of the edges, it can't be intersecting
		if (point.x > GetRightmost() || point.x < GetLeftmost() || point.y < GetTopmost() || point.y > GetBottommost())
			return false;

		// Draw a line from the point to outside the right side of the polygon. Count
		// the line intersections. If it is odd, the point is within the polygon.
		Vector2 lineTo = Vector2(GetRightmost() + 1.0f, point.y);
		int intersections = 0;
		for (int i = 0; i < points.size(); ++i)
		{
			Vector2 p = GetPoint(i);
			Vector2 next = GetPoint((i + 1) % points.size());
			if (Collider::LinesIntersect(point, lineTo, p, next))
				++intersections;
		}

		return (intersections % 2 == 1);
	}

	bool PolygonCollider::IntersectsLine(const Vector2& start, const Vector2& end, float lineRadius)
	{
		//TODO: Support line width!

		//Polygons don't collide if they have less than 3 points
		if (points.size() <= 2)
			return false;

		// If either end point is within the polygon, it intersects	
		if (IntersectsPoint(start) || IntersectsPoint(end))
			return true;

		// Otherwise, check the line against every polygon line
		for (int i = 0; i < points.size(); ++i)
		{
			Vector2 point = GetPoint(i);
			Vector2 next = GetPoint((i + 1) % points.size());
			if (Collider::LinesIntersect(start, end, point, next))
				return true;
		}

		return false;
	}

	bool PolygonCollider::IsEmpty()
	{
		return (points.size() == 0);
	}

	void PolygonCollider::AddPoint(Vector2 point)
	{
		//Update edge values
		if (IsEmpty())
		{
			leftmost = rightmost = point.x;
			topmost = bottommost = point.y;
		}
		else
		{
			if (point.x < leftmost)
				leftmost = point.x;
			else if (point.x > rightmost)
				rightmost = point.x;

			if (point.y < topmost)
				topmost = point.y;
			else if (point.y > bottommost)
				bottommost = point.y;
		}

		//Add to the vector
		points.push_back(point);
	}

	int PolygonCollider::GetPointCount()
	{
		return points.size();
	}

	Vector2 PolygonCollider::GetPoint(int index, bool relativeToEntity)
	{
#ifdef DEBUG
		if (index >= points.size())
			Debug::Log("ERROR: Getting a point out of a polygon that doesn't have that many points.");
#endif

		if (relativeToEntity)
			return points[index];
		else
			return points[index] + offset;
	}

	float PolygonCollider::GetLeftmost(bool relativeToEntity)
	{
#ifdef DEBUG
		if (IsEmpty())
			Debug::Log("ERROR: Getting leftmost point of an empty PolygonCollider.");
#endif

		if (relativeToEntity)
			return leftmost;
		else
			return offset.x + leftmost;
	}

	float PolygonCollider::GetRightmost(bool relativeToEntity)
	{
#ifdef DEBUG
		if (IsEmpty())
			Debug::Log("ERROR: Getting rightmost point of an empty PolygonCollider.");
#endif

		if (relativeToEntity)
			return rightmost;
		else
			return offset.x + rightmost;
	}

	float PolygonCollider::GetTopmost(bool relativeToEntity)
	{
#ifdef DEBUG
		if (IsEmpty())
			Debug::Log("ERROR: Getting topmost point of an empty PolygonCollider.");
#endif

		if (relativeToEntity)
			return topmost;
		else
			return offset.y + topmost;
	}

	float PolygonCollider::GetBottommost(bool relativeToEntity)
	{
#ifdef DEBUG
		if (IsEmpty())
			Debug::Log("ERROR: Getting bottommost point of an empty PolygonCollider.");
#endif

		if (relativeToEntity)
			return bottommost;
		else
			return offset.y + bottommost;
	}
}