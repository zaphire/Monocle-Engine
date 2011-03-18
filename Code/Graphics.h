#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Color.h"

namespace Monocle
{
	class TextureAsset;
	class Entity;

	class Graphic
	{
	public:
		Vector2 position;
		virtual void Render()=0;
	};

	class Graphics
	{
	public:
		Graphics();
		void Init();
		bool SetResolution(int w, int h, int bits, bool full);

		void BeginFrame();
		void EndFrame();
		void ShowBuffer();


		static void Set2D(int virtualWidth, int virtualHeight);
		static void Set3D();

		static void Resize(int w, int h);

		static void BindTexture(TextureAsset* textureAsset);
		static void SetCameraPosition(const Vector2 &position);

		static void Translate(float x, float y, float z);
		static void Translate(Vector2 pos);
		static void Scale(const Vector2 &pos);
		static void Rotate(float r, float ax, float ay, float az);

		static void RenderTriangle(float size);
		//static void RenderQuad(float size);
		static void RenderQuad(float width, float height, Vector2 textureOffset=Vector2::zero, Vector2 textureScale=Vector2::one);
		static void RenderLineRect(float x, float y, float w, float h);
		static void PushMatrix();
		static void PopMatrix();
		static void Blend();
		static void SetColor(const Color &color);
		static void SetBackgroundColor(const Color &color);

		static Vector2 screenCenter;

	private:
		static Graphics *instance;
		Vector2 cameraPosition;
		Vector2 resolutionScale;
		int virtualWidth, virtualHeight;
		unsigned int lastBoundTextureID;
	};

}