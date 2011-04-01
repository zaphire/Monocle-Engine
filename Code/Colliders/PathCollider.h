#include "Collider.h"
#include "../LevelEditor/Node.h"

namespace Monocle
{
	class PathCollider : public Collider
	{
	public:
		PathCollider();
		ColliderType GetColliderType();
		bool IntersectsPoint(const Vector2& point);
		bool IntersectsLine(const Vector2& start, const Vector2& end, float lineRadius = 1.0f);

		Node *start;
	};
}