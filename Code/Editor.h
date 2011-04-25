#pragma once

#include "Vector2.h"

namespace Monocle
{
	class Editor
	{
	public:
		virtual void Enable()=0;
		virtual void Disable()=0;
		virtual void Update()=0;
	};

	class CameraEditor : public Editor
	{
	public:
	protected:
		void UpdateCamera();
		Vector2 lastWorldMousePosition;
	};
}
