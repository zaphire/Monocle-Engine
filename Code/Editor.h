#pragma once

#include "Vector2.h"
#include "Entity.h"

namespace Monocle
{
	class Editor : public Entity
	{
	public:
		Editor();
	};

	class CameraEditor
	{
	public:
	protected:
		void UpdateCamera();
		Vector2 lastWorldMousePosition;
	};
}
