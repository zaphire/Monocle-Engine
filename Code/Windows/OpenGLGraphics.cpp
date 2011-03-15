#ifdef MONOCLE_OPENGL

#include "../Graphics.h"
#include "../Debug.h"
#include "../Platform.h"
#include "../TextureAsset.h"
#include "../Color.h"

#ifdef MONOCLE_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

#endif

#include <gl/GL.h>
#include <gl/GLU.h>

// opengl/windows init code borrowed from http://nehe.gamedev.net

namespace Monocle
{
	Graphics *Graphics::instance = NULL;

	Graphics::Graphics()
	{
		instance = this;
	}

	void Graphics::Init()
	{
		Debug::Log("Graphics::Init");

		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping ( NEW )
		glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);					// Black Background
		glClearDepth(1.0f);							// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);							// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations	
	}

	bool Graphics::SetResolution(int w, int h, int bits, bool full)
	{
		//TODO: tell platform to change window size
		Resize(w, h);
		return true;
	}

	// temporary
	void Graphics::Blend()
	{
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	}

	void Graphics::Resize(int width, int height)
	{
		if (height==0)										// Prevent A Divide By Zero By
		{
			height=1;										// Making Height Equal One
		}

		glViewport(0,0,width,height);						// Reset The Current Viewport

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		// Calculate The Aspect Ratio Of The Window
		gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix
	}

	void Graphics::SetCameraPosition(const Vector3 &position)
	{
		instance->cameraPosition = position;
	}

	void Graphics::Translate(float x, float y, float z)
	{
		glTranslatef(x, y, z);
	}

	void Graphics::Translate(Vector2 pos)
	{
		glTranslatef(pos.x, pos.y, 0.0f);
	}

	void Graphics::PushMatrix()
	{
		glPushMatrix();
	}

	void Graphics::PopMatrix()
	{
		glPopMatrix();
	}

	void Graphics::Rotate(float ax, float ay, float az, float r)
	{
		glRotatef(ax, ay, az, r);
	}

	void Graphics::RenderTriangle(float size)
	{
		float halfSize = size*0.5f;

		glBegin(GL_TRIANGLES);						// Drawing Using Triangles
			glVertex3f( 0.0f, halfSize, 0.0f);				// Top
			glVertex3f(-halfSize,-halfSize, 0.0f);				// Bottom Left
			glVertex3f( halfSize,-halfSize, 0.0f);				// Bottom Right
		glEnd();
	}

	void Graphics::RenderQuad(float size)
	{
		float halfSize = size*0.5f;

		glBegin(GL_QUADS);
			glVertex3f(-halfSize, -halfSize, 0.0f);
			glTexCoord2f(1, 1);
			glVertex3f(halfSize, -halfSize, 0.0f);
			glTexCoord2f(1, 0);
			glVertex3f(halfSize, halfSize, 0.0f);
			glTexCoord2f(0, 0);
			glVertex3f(-halfSize, halfSize, 0.0f);
			glTexCoord2f(0, 1);
		glEnd();
	}

	void Graphics::RenderLineRect(float x, float y, float w, float h)
	{
		float hw = w*0.5f;
		float hh = h*0.5f;

		glBegin(GL_LINES);
			glVertex3f(x-hw, y-hh, 0.0f);
			glVertex3f(x+hw, y-hh, 0.0f);

			glVertex3f(x+hw, y-hh, 0.0f);
			glVertex3f(x+hw, y+hh, 0.0f);
			
			glVertex3f(x+hw, y+hh, 0.0f);
			glVertex3f(x-hw, y+hh, 0.0f);

			glVertex3f(x-hw, y+hh, 0.0f);
			glVertex3f(x-hw, y-hh, 0.0f);
		glEnd();
	}

	void Graphics::SetColor(const Color &color)
	{
		glColor4f(color.r, color.g, color.b, color.a);
	}

	void Graphics::RenderQuad(float width, float height)
	{
		float halfWidth = width*0.5f;
		float halfHeight = height*0.5f;

		glBegin(GL_QUADS);
			glVertex3f(-halfWidth, -halfHeight, 0.0f);
			glTexCoord2f(1, 1);
			glVertex3f(halfWidth, -halfHeight, 0.0f);
			glTexCoord2f(1, 0);
			glVertex3f(halfWidth, halfHeight, 0.0f);
			glTexCoord2f(0, 0);
			glVertex3f(-halfWidth, halfHeight, 0.0f);
			glTexCoord2f(0, 1);
		glEnd();
	}

	void Graphics::BeginFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();									// Reset The Current Modelview Matrix

		glTranslatef(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	}

	void Graphics::EndFrame()
	{
	}

	void Graphics::ShowBuffer()
	{
		Platform::ShowBuffer();
	}

	void Graphics::BindTexture(TextureAsset* textureAsset)
	{
		if (textureAsset != NULL)
		{
			//Debug::Log("bound texture");
			//Debug::Log((int)textureAsset->texID);
			glBindTexture(GL_TEXTURE_2D, textureAsset->texID);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, NULL);
		}
	}
}

#endif