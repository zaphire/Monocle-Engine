#pragma once

#include "../Vector2.h"
// for NULL
#include <cstdio>

namespace Monocle
{
	enum ColliderType
	{
		CT_RECT=0,
		CT_CIRCLE,
		CT_POLYGON,
		CT_PATH
	};

	class RectangleCollider;
	class CircleCollider;
	class PolygonCollider;
	class LineSegmentCollider;
	class PathCollider;
	class Entity;
	class CollisionData;

	class Collider
	{
	public:
		Collider();

		void SetEntity(Entity* entity);
		Entity* GetEntity();
		Vector2 GetEntityPosition();

		Vector2 offset;

		virtual ColliderType GetColliderType() = 0;
		virtual bool IntersectsPoint(const Vector2& point, CollisionData *collisionData=NULL)=0;
		virtual bool IntersectsLine(const Vector2& start, const Vector2& end, float lineRadius = 0.0f, CollisionData *collisionData=NULL)=0;

		static bool Collide(Collider* a, Collider* b, CollisionData *collisionData = NULL);
		static bool LinesIntersect(const Vector2& aStart, const Vector2& aEnd, const Vector2& bStart, const Vector2& bEnd, CollisionData *collisionData=NULL);

	private:
		Entity *entity;
		static bool CollideRectRect(RectangleCollider* a, RectangleCollider* b, CollisionData *collisionData);
		static bool CollideCircleCircle(CircleCollider* a, CircleCollider* b, CollisionData *collisionData);
		static bool CollidePolygonPolygon(PolygonCollider* a, PolygonCollider* b, CollisionData *collisionData);

		static bool CollideRectCircle(RectangleCollider* a, CircleCollider* b, CollisionData *collisionData);
		static bool CollideRectPolygon(RectangleCollider* a, PolygonCollider* b, CollisionData *collisionData);
		static bool CollideCirclePolygon(CircleCollider* a, PolygonCollider* b, CollisionData *collisionData);

		static bool CollideCirclePath(CircleCollider *a, PathCollider *b, CollisionData *collisionData);
	};
}