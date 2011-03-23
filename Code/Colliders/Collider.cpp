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

	const Vector2& Collider::GetEntityPosition()
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

		//Big nasty if/else for deciding which collision function to use *puke*
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
		//TODO!

		return false;
	}

	bool Collider::CollideRectCircle(RectangleCollider* a, CircleCollider* b)
	{
		Vector2 aOff = a->GetEntityPosition();
		Vector2 bOff = b->GetEntityPosition();

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
		//TODO!

		return false;
	}
}
