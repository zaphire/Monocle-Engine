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

	bool PolygonCollider::IntersectsLine(const Vector2& start, const Vector2& end)
	{
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
		points.push_back(point);
	}

	int PolygonCollider::GetPointCount()
	{
		return points.size();
	}

	const Vector2& PolygonCollider::GetPoint(int index, bool relativeToEntity)
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

		float left = points[0].x;
		for (int i = 1; i < points.size(); ++i)
		{
			if (points[i].x < left)
				left = points[i].x;
		}

		return left;
	}

	float PolygonCollider::GetRightmost(bool relativeToEntity)
	{
#ifdef DEBUG
		if (IsEmpty())
			Debug::Log("ERROR: Getting rightmost point of an empty PolygonCollider.");
#endif

		float right = points[0].x;
		for (int i = 1; i < points.size(); ++i)
		{
			if (points[i].x > right)
				right = points[i].x;
		}

		return right;
	}

	float PolygonCollider::GetTopmost(bool relativeToEntity)
	{
#ifdef DEBUG
		if (IsEmpty())
			Debug::Log("ERROR: Getting topmost point of an empty PolygonCollider.");
#endif

		float top = points[0].y;
		for (int i = 1; i < points.size(); ++i)
		{
			if (points[i].y < top)
				top = points[i].y;
		}

		return top;
	}

	float PolygonCollider::GetBottommost(bool relativeToEntity)
	{
#ifdef DEBUG
		if (IsEmpty())
			Debug::Log("ERROR: Getting bottommost point of an empty PolygonCollider.");
#endif

		float bottom = points[0].y;
		for (int i = 1; i < points.size(); ++i)
		{
			if (points[i].y > bottom)
				bottom = points[i].y;
		}

		return bottom;
	}
}