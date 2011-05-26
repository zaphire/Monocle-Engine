#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Color.h"
#include "Tween.h"
#include <vector>
#include <string>

namespace Monocle
{
	class Node;
	class Entity;
    class TextureAsset;
    class FontAsset;
    
	enum BlendType
	{
		BLEND_NONE=0,
		BLEND_ALPHA,
		BLEND_ADDITIVE,
		BLEND_MULTIPLY,
	};

	class Graphic
	{
	public:
		Vector2 position;
		//virtual void Update()=0;
		virtual void Render(Entity *entity)=0;
		virtual void GetWidthHeight(int *width, int *height)=0;
	};

	class Graphics
	{
	public:
		Graphics();
		void Init();

		///TODO: move to platform?
		bool SetResolution(int w, int h, int bits, bool full);

		void BeginFrame();
		void EndFrame();
		void ShowBuffer();

		static void Set2D(int virtualWidth, int virtualHeight);
		static void Set3D();

		static void Resize(int w, int h);

		static void BindTexture(TextureAsset* textureAsset);
        static void BindFont(FontAsset* fontAsset);

		//static void SetCameraPosition(const Vector2 &position);
		//static void AdjustCameraPosition(const Vector2 &adjustment);
		//static void SetCameraZoom(const Vector2 &zoom);
		//static void AdjustCameraZoom(const Vector2 &adjustment);
		//
		//static const Vector2 &GetCameraPosition();
		//static const Vector2 &GetCameraZoom();
		//

		static const Vector2 &GetResolutionScale();

		//static void MoveCameraPosition(const Vector2 &position, float time, EaseType easeType);

		static void Translate(float x, float y, float z);
		static void Translate(const Vector2 &pos);
		static void Scale(const Vector2 &pos);
		static void Rotate(float r, float ax, float ay, float az);
		static void MultiplyMatrix(float *m);

		static void RenderTriangle(float size);
		//static void RenderQuad(float size);
		static void RenderQuad(float width, float height, const Vector2 &textureOffset=Vector2::zero, const Vector2 &textureScale=Vector2::one, const Vector2 &position=Vector2::zero);
		static void RenderQuadCustom(const Vector2 &ul, const Vector2 &ur, const Vector2 &lr, const Vector2 &ll, const Vector2 &textureOffset, const Vector2 &textureScale);
        static void RenderText(const FontAsset& font, const std::string& text, float x, float y);
		static void RenderLineRect(float x, float y, float w, float h);
		static void PushMatrix();
		static void PopMatrix();
		static void SetBlend(BlendType blend);
		static void SetColor(const Color &color);
		static void SetBackgroundColor(const Color &color);
        static void EnableBackgroundReset( bool bgReset = true );

		static Color GetBackgroundColor();
		//static void SceneMatrix();

		static Vector2 GetMatrixPosition();

		static Vector2 GetScreenCenter();
		
		static int GetVirtualWidth();
		static int GetVirtualHeight();

		static void ResolutionMatrix();
		static void IdentityMatrix();
		static void DefaultMatrix();

		static void RenderLine(const Vector2 &pos1, const Vector2 &pos2);

		static void BeginLine();
		static void BeginLineStrip();
		static void Vertex(Vector2 vertex);
		static void EndLine();

		static void RenderPathMesh(const std::vector<Node*> &nodes, int cells, float size, bool flipX=false, bool flipY=false);

	private:
		
		static Graphics *instance;

		Vector2 resolutionScale;
		Vector2 screenCenter;
		int virtualWidth, virtualHeight;
		unsigned int lastBoundTextureID;
		BlendType currentBlend;
        
        bool bgReset;
	};

}
