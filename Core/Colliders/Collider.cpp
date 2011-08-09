#include "Collider.h"
#include "RectangleCollider.h"
#include "CircleCollider.h"
#include "PolygonCollider.h"
#include "PathCollider.h"
#include "../Entity.h"
#include <stdio.h> // for NULL
#include "../CollisionData.h"
#include "../Macros.h"

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
			return entity->GetWorldPosition();
		else
			return Vector2();
	}

	bool Collider::Collide(Collider* a, Collider* b, CollisionData *collisionData)
	{
		ColliderType typeA = a->GetColliderType();
		ColliderType typeB = b->GetColliderType();

		//Big nasty if/else for deciding which collision function to use
		if (typeA == CT_RECT && typeB == CT_RECT)									//Rectangle - Rectangle
			return CollideRectRect((RectangleCollider*)a, (RectangleCollider*)b, collisionData);

		else if (typeA == CT_CIRCLE && typeB == CT_CIRCLE)							//Circle - Circle
			return CollideCircleCircle((CircleCollider*)a, (CircleCollider*)b, collisionData);

		else if (typeA == CT_POLYGON && typeB == CT_POLYGON)						//Polygon - Polygon
			return CollidePolygonPolygon((PolygonCollider*)a, (PolygonCollider*)b, collisionData);

		else if (typeA == CT_RECT && typeB == CT_CIRCLE)							//Rectangle - Circle
			return CollideRectCircle((RectangleCollider*)a, (CircleCollider*)b, collisionData);
		else if (typeA == CT_CIRCLE && typeB == CT_RECT)
			return CollideRectCircle((RectangleCollider*)b, (CircleCollider*)a, collisionData);

		else if (typeA == CT_RECT && typeB == CT_POLYGON)							//Rectangle - Polygon
			return CollideRectPolygon((RectangleCollider*)a, (PolygonCollider*)b, collisionData);
		else if (typeA == CT_POLYGON && typeB == CT_RECT)
			return CollideRectPolygon((RectangleCollider*)b, (PolygonCollider*)a, collisionData);

		else if (typeA == CT_CIRCLE && typeB == CT_POLYGON)							//Circle - Polygon
			return CollideCirclePolygon((CircleCollider*)a, (PolygonCollider*)b, collisionData);
		else if (typeA == CT_POLYGON && typeB == CT_CIRCLE)
			return CollideCirclePolygon((CircleCollider*)b, (PolygonCollider*)a, collisionData);

		else if (typeA == CT_CIRCLE && typeB == CT_PATH)
			return CollideCirclePath((CircleCollider*)a, (PathCollider*)b, collisionData);
		else if (typeA == CT_PATH && typeB == CT_CIRCLE)
			return CollideCirclePath((CircleCollider*)b, (PathCollider*)a, collisionData);

		else if ((typeA == CT_RECT && typeB == CT_PATH) || (typeA == CT_PATH && typeB == CT_RECT))
		{
			Debug::Log("Warning: Collisions between Rectangle and Path not yet supported.");
		}
		else
		{
			Debug::Log("Warning: Unhandled collider pair.");
		}

		// Unhandled case
		return false;
	}

	bool Collider::LinesIntersect(const Vector2& aStart, const Vector2& aEnd, const Vector2& bStart, const Vector2& bEnd, CollisionData *collisionData)
	{
		
		Vector2 s1, s2;
		s1 = aEnd - aStart;
		s2 = bEnd - bStart;

		float s, t;
		s = (-s1.y * (aStart.x - bStart.x) + s1.x * (aStart.y - bStart.y)) / (-s2.x * s1.y + s1.x * s2.y);
		t = ( s2.x * (aStart.y - bStart.y) - s2.y * (aStart.x - bStart.x)) / (-s2.x * s1.y + s1.x * s2.y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		{
			if (collisionData)
			{
				collisionData->hitPoint = aStart + (t * s1);
			}
			return true;
		}

		return false;
		

		/*
		float d = ((bEnd.y - bStart.y) * (aEnd.x - aStart.x)) - ((bEnd.x - bStart.x) * (aEnd.y - aStart.y));
		float nX = ((bEnd.x - bStart.x) * (aStart.y - bStart.y)) - ((bEnd.y - bStart.y) * (aStart.x - bStart.x));
		float nY = ((aEnd.x - aStart.x) * (aStart.y - bStart.y)) - ((aEnd.y - aStart.y) * (aStart.x - bStart.x));

		// ignore this optimization if we care about finding a hitPoint
		if (!collisionData)
		{
			//Lines are parallel if d == 0, so check if they're coincident
			if (d == 0.0f)
			{
				if (nX == 0.0f && nY == 0.0f)
				{
					//Lines are coincident, now check if they overlap
					return ((aEnd.x > bStart.x && aStart.x < bEnd.x) || (aEnd.y > bStart.y && aStart.y < bEnd.y));
				}
				else
				{
					return false;
				}
			}
		}

		float uX = nX / d;
		float uY = nY / d;

		if (collisionData)
		{
			if (uX >= 0.0f && uX <= 1.0f && uY >= 0.0f && uY <= 1.0f)
			{
				// Collision detected
				Vector2 diff = aEnd - aStart;
				collisionData->hitPoint = aStart + (uY * diff);
				return true;
			}
			else
			{
				return false;
			}
		}
		
		return (uX >= 0.0f && uX <= 1.0f && uY >= 0.0f && uY <= 1.0f);
		*/
	}

	bool Collider::CollideRectRect(RectangleCollider* a, RectangleCollider* b, CollisionData *collisionData)
	{
		//TODO: store data in collisionData!
        

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

	bool Collider::CollideCircleCircle(CircleCollider* a, CircleCollider* b, CollisionData *collisionData)
	{
		Vector2 diff = a->GetCenter() - b->GetCenter();
		if (diff.IsInRange(a->GetRadius() + b->GetRadius()))
		{
			if (collisionData)
			{
				collisionData->normal = diff.GetNormalized();
				collisionData->penetration = (a->GetRadius() + b->GetRadius()) - diff.GetMagnitude();
				collisionData->hitPoint = diff * 0.5f + b->GetCenter();
			}
			return true;
		}
		return false;
	}

	bool Collider::CollidePolygonPolygon(PolygonCollider* a, PolygonCollider* b, CollisionData *collisionData)
	{
		//TODO: store data in collisionData!

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

	bool Collider::CollideRectCircle(RectangleCollider* a, CircleCollider* b, CollisionData *collisionData)
	{
		//TODO: store data in collisionData!
		//printf("CollideRectCircle\n");

		//The center of the circle is within the rectangle
		if (a->IntersectsPoint(b->GetCenter()))
		{
			//printf("collided\n");
			return true;
		}

		//Check the circle against the four edges of the rectangle
		Vector2 pA = a->GetTopLeft();
		Vector2 pB = a->GetTopRight();
		Vector2 pC = a->GetBottomRight();
		Vector2 pD = a->GetBottomLeft();
		if (b->IntersectsLine(pA, pB, 0.0f, collisionData) || b->IntersectsLine(pB, pC, 0.0f, collisionData) || b->IntersectsLine(pC, pD, 0.0f, collisionData) || b->IntersectsLine(pD, pA, 0.0f, collisionData))
		{
			//printf("collided\n");
			if (collisionData)
			{
				//printf("collisionData.normal (%f, %f) penetration: %f hitPoint: (%f, %f)\n", collisionData->normal.x, collisionData->normal.y, collisionData->penetration, collisionData->hitPoint.x, collisionData->hitPoint.y);
			}
			return true;
		}

		return false;
	}

	bool Collider::CollideRectPolygon(RectangleCollider* a, PolygonCollider* b, CollisionData *collisionData)
	{
		//TODO: store data in collisionData!

		//Check if it's even possible for them to collide
		if (a->GetLeft() > b->GetRightmost())
			return false;
		if (a->GetRight() < b->GetLeftmost())
			return false;
		if (a->GetTop() > b->GetBottommost())
			return false;
		if (a->GetBottom() < b->GetTopmost())
			return false;

		//Check if the first point of either is inside the other
		//(accounts for the case where one is completely inside the other)
		if (a->IntersectsPoint(b->GetPoint(0)) || b->IntersectsPoint(a->GetTopLeft()))
			return true;

		//Compare lines for intersections
		Vector2 rectPoints[4];
		rectPoints[0] = a->GetTopLeft();
		rectPoints[1] = a->GetTopRight();
		rectPoints[2] = a->GetBottomRight();
		rectPoints[3] = a->GetBottomLeft();

		int totalB = b->GetPointCount();
		for (int i = 0; i < totalB; ++i)
		{
			Vector2 startB	= b->GetPoint(i);
			Vector2 endB	= b->GetPoint((i + 1) % totalB);
			for (int j = 0; j < 4; ++j)
			{
				if (LinesIntersect(startB, endB, rectPoints[j], rectPoints[(j + 1) % 4]))
					return true;
			}
		}

		return false;
	}

	bool Collider::CollideCirclePolygon(CircleCollider* a, PolygonCollider* b, CollisionData *collisionData)
	{
		//TODO: store data in collisionData!

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

	bool Collider::CollideCirclePath(CircleCollider *a, PathCollider *b, CollisionData *collisionData)
	{
		Node *node = b->startNode;
		Node *prevNode = NULL;
		Vector2 start, end;
		Vector2 avgNormal;
		int numNormals = 0;
		float maxPenetration = 0.0f;

		while (node)
		{
			if (prevNode && prevNode->variant != -1)
			{
				start = prevNode->GetWorldPosition();
				end = node->GetWorldPosition();
				if (a->IntersectsLine(start, end, b->radius, collisionData))
				{
					if (collisionData)
					{
						avgNormal += collisionData->normal;
						maxPenetration = MAX(collisionData->penetration, maxPenetration);
						numNormals++;
					}
					else
					{
						return true;
					}
				}
			}
			prevNode = node;
			node = node->GetNext();
		}

		if (numNormals > 0)
		{
			collisionData->normal = avgNormal/(float)numNormals;
			collisionData->penetration = maxPenetration;
			
			return true;
		}

		return false;
	}
}
