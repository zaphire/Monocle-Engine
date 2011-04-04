#pragma once

#include "Transform.h"

namespace Monocle
{
	class Camera : public Transform
	{
	public:
		Camera();
		Camera(const Camera& camera);

		void ApplyMatrix();
	};
}
