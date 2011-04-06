#pragma once

#include "Vector2.h"

namespace Monocle
{
	class Transform
	{
	public:
		Transform();
		Transform(const Transform &transform);

		void LerpTransform(Transform *prev, Transform *next, float percent);

		Vector2 position;
		float rotation;
		Vector2 scale;
	};
}