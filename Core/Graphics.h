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
    
	//! The type of blending to use between layers when a layer is drawn over another.
	enum BlendType
	{
		BLEND_NONE=0,
		BLEND_ALPHA,
		BLEND_ADDITIVE,
		BLEND_MULTIPLY,
	};
	
	//! Output image types available to write screen data to.
	enum ImageType
	{
	    IMAGE_BMP,
	    IMAGE_PNG,
	    IMAGE_TGA
	};

	//! Base class for graphic types that may be attached to and rendered with entities.
	class Graphic
	{
	public:
		Graphic() : rotation(0.0f)
		{}
		Vector2 position;
		float rotation;
		virtual void Update()=0;
		virtual void Render(Entity *entity)=0;
		virtual void GetWidthHeight(int *width, int *height)=0;
	};

	//! \brief Static class responsble for rendering to the screen.
	//! \remark This class is meant to be generic enough so as to allow it to serve as
	//! an interface to several different rendering engines.
	class Graphics
	{
	public:
		///TODO: move to platform?
		bool SetResolution(int w, int h, int bits, bool full);

		//! Sets the virtual resolution of the window
		static void Set2D(int virtualWidth, int virtualHeight);
		//static void Set3D();

		static void Viewport(int x, int y, int width, int height);

		//! Resizes the Graphics' viewport.
		//! \remark This does not resize the platform window, it only changes
		//! the graphics' perspective.
		static void Resize(int w, int h);

		//! Causes any future Graphics calls (such as RenderQuad) to use the given TextureAsset
		//! as the texture for the rendered item.  A NULL pointer parameter unbinds the current
		//! texture and future render calls will not use a texture.
		static void BindTexture(TextureAsset* textureAsset);
		//! Causes any future Graphics calls to use the texture for this font.  This function
		//! shares internal state with BindTexture; Calling BindTexture, then BindFont, then attempting
		//! to render a quad with the first texture will not produce expected results.  A NULL 
		//! pointer parameter unbinds the current texture and future render calls will not use a 
		//! texture.
        static void BindFont(FontAsset* fontAsset);

		//! Returns the ratios between real and virtual widths and heights.
		static const Vector2 &GetResolutionScale();

		//! Translates the renderer the given amounts.
		static void Translate(float x, float y, float z);
		//! Translates the renderer the given amount.
		static void Translate(const Vector2 &pos);
		//! Scales the renderer the given amount.  Even scale factors will provide uniform scaling,
		//! while uneven scale will stretch renders on an axis.
		static void Scale(const Vector2 &pos);
		//! Rotates the renderer on the three axes.
		//! \param r [in] The angle of rotation in degrees
		//! \param ax [in] The rotation factor for the x axis (from 0 to 1, with 1 being a rotation of r degrees and 0 being a rotation of 0)
		//! \param ay [in] The rotation factor for the y axis (from 0 to 1)
		//! \param az [in] The rotation factor for the z axis (from 0 to 1)
		static void Rotate(float r, float ax, float ay, float az);
		//! Multiplies the Graphics view matrix with the given matrix.
		//! \param m [in] A pointer to an array of 16 floats that represent a 4x4 matrix
		static void MultiplyMatrix(float *m);

		//! Renders a triangle (points at 0, 4, and 8 o'clock) at the renderer's current position
		//! \param size [in] The size of each side of the bounding square of the rendered triangle.
		static void RenderTriangle(float size);
		//static void RenderQuad(float size);
		//! \brief Renders a quad at the renderer's position
		//! This function renders a quad where the renderer's position is the center of the quad when not passed an offset in the position parameter.
		//! \param width [in] The width of the quad to render
		//! \param height [in] The height of the quad to render
		//! \param textureOffset [in] A vector indicating the position on the texture (if set) that corresponds to the upper left corner of the quad
		//! \param textureScale [in] A vector indicating the scale on the x and y axes which determines how much of the texture to map to the quad.
		//!		A uniform scale of one means that the bottom right hand corner will recieve the texture coordinates (textureOffset + width, textureOffset + height)
		//!	\param position [in] The offset from the current renderer location to render the quad
		//! \sa RenderQuadCustom
		static void RenderQuad(float width, float height, const Vector2 &textureOffset=Vector2::zero, const Vector2 &textureScale=Vector2::one, const Vector2 &position=Vector2::zero);
		//! \brief Renders a quad with caller-defined corner points.
		//! \param ul [in] The upper left corner position
		//! \param ur [in] The upper right corner position
		//! \param lr [in] The lower right corner position
		//! \param ll [in] The lower left corner position
		//! \param textureOffset [in] A vector indicating the position on the texture (if set) that corresponds to the upper left corner of the quad
		//! \param textureScale [in] A vector indicating the scale on the x and y axes which determines how much of the texture to map to the quad.
		//! \sa RenderQuad
		static void RenderQuadCustom(const Vector2 &ul, const Vector2 &ur, const Vector2 &lr, const Vector2 &ll, const Vector2 &textureOffset, const Vector2 &textureScale);
        //! \brief Renders the given text with the given font at the given position.
		//! \param font [in] The font to use to render
		//! \param text [in] The text to render
		//! \param x [int] The x coordinate of the origin of the text, which serves as the baseline for the text
		//! \param y [int] The y coordinate of the origin of the text
		//! \sa FontAsset
		static void RenderText(const FontAsset& font, const std::string& text, float x, float y);
		//! \brief Renders a wireframe quad
		//! \sa RenderQuad
		static void RenderLineRect(float x, float y, float w, float h);
		//! \brief Renders a line
		//! \param pos1 [in] The start position of the line
		//! \param pos2 [in] The end position of the line
		static void RenderLine(const Vector2 &pos1, const Vector2 &pos2);
		//! 
		static void RenderPathMesh(const std::vector<Node*> &nodes, int cells, float size, bool flipX=false, bool flipY=false);
		
		//! \brief Pushes a view matrix onto the stack.
		//! Coupled with PopMatrix, this function allows the caller to make rotations, translations, etc, to the matrix
		//! at the top of the stack and restore the renderer to the previous state by popping the topmost matrix off the stack.
		//! This is likely done at several levels of the rendering process to ensure changes intended to be local do not
		//! propogate to unintended parts of the system.
		//! \sa PopMatrix
		static void PushMatrix();
		//! \brief Pops a view matrix off the stack.
		//! \sa PushMatrix
		static void PopMatrix();
		//! \brief Sets the blend mode for future rendering calls.
		//! This function determines how future Graphics calls that cause layer overlap will be handled.
		//! \param blend The blending method to use
		static void SetBlend(BlendType blend);
		//! \brief Changes the color that plain vertices are rendered.
		//! This function determines the color of vertices rendered in any future Graphics calls.  This
		//! has no effect if rendering with a bound texture, unless a blend method besides BLEND_NONE is
		//! also set.
		//! \param color The color to set to
		static void SetColor(const Color &color);
		//! \brief Changes the absolute background color of the renderer.
		//! The color passed to this function will be the color of the window in areas which are not covered
		//! by any other rendered items.
		//! \param color [in] The color to set to
		//! \sa GetBackgroundColor
		static void SetBackgroundColor(const Color &color);
		//! \brief Returns the background color.
		//! \sa SetBackgroundColor
		static Color GetBackgroundColor();

		//! \brief Determines whether or not to clear the depth and color buffers for each frame.
        static void EnableBackgroundReset( bool bgReset = true );

		static Vector2 GetMatrixPosition();

		//! \brief Returns the coordinates at the center of the screen.
		static Vector2 GetScreenCenter();
		
		//! \brief Returns the virtual width of the screen.
		static int GetVirtualWidth();
		//! \brief Returns the virtual height of the screen.
		static int GetVirtualHeight();

		static void ResolutionMatrix();
		static void IdentityMatrix();
		static void DefaultMatrix();

		static void BeginLine();
		static void BeginLineStrip();
		static void Vertex(Vector2 vertex);
		static void EndLine();
		
		//! \brief Outputs the contents of the screen to an image file
		//! \param filename [in] The filename of the image file to save to.  Relative to the running directory, not ContentPath.
		//! \param type [in] The type of file to write
		static void ScreenToImage(const std::string &filename, ImageType type);
        
        static void CheckErrors();
	protected:
		friend class Game;
		Graphics();
		void Init();

		void BeginFrame();
		void EndFrame();
		void ShowBuffer();
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
