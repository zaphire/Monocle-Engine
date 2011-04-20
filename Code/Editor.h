#pragma once

#include "Vector2.h"

namespace Monocle
{
	class Editor
	{
	public:
		Editor();
		virtual void Update();
	};

	class CameraEditor : public Editor
	{
	public:
	protected:
		void UpdateCamera();
		Vector2 lastWorldMousePosition;
	};

	class EntityEditor : public Editor
	{
	public:
	protected:
	};
}