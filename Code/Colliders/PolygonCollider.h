#pragma once

#include "Collider.h"
#include "../Vector2.h"
#include "../Entity.h"
#include <vector>

namespace Monocle
{
	// Arbitrary, possibly concave polygon
	class PolygonCollider : public Collider
	{
	public:
		std::vector<Vector2> points;
		PolygonCollider(Vector2 offset=Vector2::zero);
		ColliderType GetColliderType();
		bool IntersectsPoint(const Vector2& point);
		bool IntersectsLine(const Vector2& start, const Vector2& end, float lineRadius = 1.0f);

		bool IsEmpty();
		void AddPoint(Vector2 point);
		int GetPointCount();
		Vector2 GetPoint(int index, bool relativeToEntity = false);

		float GetLeftmost(bool relativeToEntity = false);
		float GetRightmost(bool relativeToEntity = false);
		float GetTopmost(bool relativeToEntity = false);
		float GetBottommost(bool relativeToEntity = false);

	private:
		float leftmost;
		float rightmost;
		float topmost;
		float bottommost;
	};
}