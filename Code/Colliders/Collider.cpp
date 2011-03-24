#include "Collider.h"
#include "RectangleCollider.h"
#include "CircleCollider.h"
#include "PolygonCollider.h"
#include "../Entity.h"
#include <stdio.h> // for NULL

namespace Monocle
{
	Collider::Collider()
		: entity(NULL)
	{
	}

	void Collider::SetEntity(Entity* entity)
	{
		this->entity = entity;
	}

	Entity* Collider::GetEntity()
	{
		return entity;
	}

	Vector2 Collider::GetEntityPosition()
	{
		if (entity != NULL)
			return entity->position;
		else
			return Vector2();
	}

	bool Collider::Collide(Collider* a, Collider* b)
	{
		ColliderType typeA = a->GetColliderType();
		ColliderType typeB = b->GetColliderType();

		//Big nasty if/else for deciding which collision function to use
		if (typeA == CT_RECT && typeB == CT_RECT)									//Rectangle - Rectangle
			return CollideRectRect((RectangleCollider*)a, (RectangleCollider*)b);

		else if (typeA == CT_CIRCLE && typeB == CT_CIRCLE)							//Circle - Circle
			return CollideCircleCircle((CircleCollider*)a, (CircleCollider*)b);

		else if (typeA == CT_POLYGON && typeB == CT_POLYGON)						//Polygon - Polygon
			return CollidePolygonPolygon((PolygonCollider*)a, (PolygonCollider*)b);

		else if (typeA == CT_RECT && typeB == CT_CIRCLE)							//Rectangle - Circle
			return CollideRectCircle((RectangleCollider*)a, (CircleCollider*)b);
		else if (typeA == CT_CIRCLE && typeB == CT_RECT)
			return CollideRectCircle((RectangleCollider*)b, (CircleCollider*)a);

		else if (typeA == CT_RECT && typeB == CT_POLYGON)							//Rectangle - Polygon
			return CollideRectPolygon((RectangleCollider*)a, (PolygonCollider*)b);
		else if (typeA == CT_POLYGON && typeB == CT_RECT)
			return CollideRectPolygon((RectangleCollider*)b, (PolygonCollider*)a);

		else if (typeA == CT_CIRCLE && typeB == CT_POLYGON)							//Circle - Polygon
			return CollideCirclePolygon((CircleCollider*)a, (PolygonCollider*)b);
		else if (typeA == CT_POLYGON && typeB == CT_CIRCLE)
			return CollideCirclePolygon((CircleCollider*)b, (PolygonCollider*)a);

		// Unhandled case
		return false;
	}

	bool Collider::LinesIntersect(const Vector2& aStart, const Vector2& aEnd, const Vector2& bStart, const Vector2& bEnd)
	{
		float d = ((bEnd.y - bStart.y) * (aEnd.x - aStart.x)) - ((bEnd.x - bStart.x) * (aEnd.y - aStart.y));
		float nX = ((bEnd.x - bStart.x) * (aStart.y - bStart.y)) - ((bEnd.y - bStart.y) * (aStart.x - bStart.x));
		float nY = ((aEnd.x - aStart.x) * (aStart.y - bStart.y)) - ((aEnd.y - aStart.y) * (aStart.x - bStart.x));

		//Lines are parallel if d == 0, so check if they're coincident
		if (d == 0.0f)
		{
			if (nX == 0.0f && nY == 0.0f)
			{
				//Lines are coincident, now check if they overlap
				return ((aEnd.x > bStart.x && aStart.x < bEnd.x) || (aEnd.y > bStart.y && aStart.y < bEnd.y));
			}
			else
				return false;
		}
		
		float uX = nX / d;
		float uY = nY / d;

		return (uX >= 0.0f && uX <= 1.0f && uY >= 0.0f && uY <= 1.0f);
	}

	bool Collider::CollideRectRect(RectangleCollider* a, RectangleCollider* b)
	{
		if (a->GetBottom() < b->GetTop())
			return false;

		if (a->GetTop() > b->GetBottom())
			return false;

		if (a->GetRight() < b->GetLeft())
			return false;

		if (a->GetLeft() > b->GetRight())
			return false;

		return true;
	}

	bool Collider::CollideCircleCircle(CircleCollider* a, CircleCollider* b)
	{
		Vector2 diff = b->GetCenter() - a->GetCenter();
		return (diff.IsInRange(a->radius + b->radius));
	}

	bool Collider::CollidePolygonPolygon(PolygonCollider* a, PolygonCollider* b)
	{
		//If the polygons aren't even close to eachother, don't bother
		if (a->GetLeftmost() > b->GetRightmost())
			return false;
		if (a->GetRightmost() < b->GetLeftmost())
			return false;
		if (a->GetTopmost() > b->GetBottommost())
			return false;
		if (a->GetBottommost() < b->GetTopmost())
			return false;

		//Check if the first point of either polygon is inside the other
		//(accounts for the case where one poly is completely inside the other)
		if (a->IntersectsPoint(b->GetPoint(0)) || b->IntersectsPoint(a->GetPoint(0)))
			return true;

		//Compare lines, looking for an intersection
		int totalA = a->GetPointCount();
		int totalB = b->GetPointCount();
		for (int i = 0; i < totalA; ++i)
		{
			Vector2 startA = a->GetPoint(i);
			Vector2 endA = a->GetPoint((i + 1) % totalA);
			for (int j = 0; j < totalB; ++j)
			{
				Vector2 startB = b->GetPoint(j);
				Vector2 endB = b->GetPoint((j + 1) % totalB);
				if (LinesIntersect(startA, endA, startB, endB))
					return true;
			}
		}

		return false;
	}

	bool Collider::CollideRectCircle(RectangleCollider* a, CircleCollider* b)
	{
		//The center of the circle is within the rectangle
		if (a->IntersectsPoint(b->GetCenter()))
			return true;

		//Check the circle against the four edges of the rectangle
		Vector2 pA = a->GetTopLeft();
		Vector2 pB = a->GetTopRight();
		Vector2 pC = a->GetBottomRight();
		Vector2 pD = a->GetBottomLeft();
		if (b->IntersectsLine(pA, pB) || b->IntersectsLine(pB, pC) || b->IntersectsLine(pC, pD) || b->IntersectsLine(pD, pA))
			return true;

		return false;
	}

	bool Collider::CollideRectPolygon(RectangleCollider* a, PolygonCollider* b)
	{
		//TODO!

		return false;
	}

	bool Collider::CollideCirclePolygon(CircleCollider* a, PolygonCollider* b)
	{
		//If the center is inside the polygon, there's a collision
		if (b->IntersectsPoint(a->GetCenter()))
			return true;

		//Otherwise, try the circle against every line of the polygon
		int points = b->GetPointCount();
		for (int i = 0; i < points; ++i)
		{
			Vector2 start = b->GetPoint(i);
			Vector2 end = b->GetPoint((i + 1) % points);
			if (a->IntersectsLine(start, end))
				return true;
		}

		return false;
	}
}
