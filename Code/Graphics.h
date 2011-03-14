#pragma once

#include "Vector3.h"

namespace Monocle
{
	class Graphics
	{
	public:
		Graphics();
		void Init();
		bool SetResolution(int w, int h, int bits, bool full);

		void BeginFrame();
		void EndFrame();
		void ShowBuffer();

		void SetCameraPosition(const Vector3 &position);
		void Translate(float x, float y, float z);
		void Rotate(float ax, float ay, float az, float r);
		void RenderTriangle(float size);
		void RenderQuad(float size);

		Vector3 cameraPosition;
	};

}