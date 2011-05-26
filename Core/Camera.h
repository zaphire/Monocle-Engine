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

		void SetLayers(int backLayer, int frontLayer);
		int GetBackLayer();
		int GetFrontLayer();

		bool isVisible;

	private:
		int backLayer, frontLayer;
	};
}
