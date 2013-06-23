#pragma once

#include "Vector2.h"
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

		static void SetCameraPosition(const Vector3 &position);
		static void Translate(float x, float y, float z);
		static void Translate(Vector2 pos);
		static void Rotate(float ax, float ay, float az, float r);
		static void RenderTriangle(float size);
		static void RenderQuad(float size);
		static void RenderQuad(float width, float height);
		static void PushMatrix();
		static void PopMatrix();

		Vector3 cameraPosition;

		static Graphics *instance;
	};

}