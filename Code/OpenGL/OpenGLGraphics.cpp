#ifdef MONOCLE_OPENGL

#include "../Graphics.h"
#include "../Debug.h"
#include "../Platform.h"
#include "../TextureAsset.h"
#include "../Color.h"

#include <cstdio>

#ifdef MONOCLE_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

#endif

#if defined(MONOCLE_MAC)
	#include <OpenGL/gl.h>
	#include <OpenGL/glext.h>
	#include <OpenGL/glu.h>
#elif defined(MONOCLE_LINUX)
	#include <GL/gl.h>
	#include <GL/glu.h>
#else
	#include <gl/GL.h>
	#include <gl/GLU.h>
#endif

// some opengl/windows init code based on http://nehe.gamedev.net

namespace Monocle
{
	Graphics *Graphics::instance = NULL;

	Vector2 Graphics::screenCenter;

	Graphics::Graphics()
	{
		instance = this;
		lastBoundTextureID = 0;
	}

	void Graphics::Init()
	{
		Debug::Log("Graphics::Init");
		
		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		//glShadeModel(GL_SMOOTH);	
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glDisable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL); 
		//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		Set2D(800,600);

		cameraPosition = screenCenter;
	}

	void Graphics::SetBackgroundColor(const Color &color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	bool Graphics::SetResolution(int w, int h, int bits, bool full)
	{
		//TODO: tell platform to change window size
		Resize(w, h);
		return true;
	}

	///HACK: temporary
	void Graphics::Blend()
	{
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}

	void Graphics::Set2D(int virtualWidth, int virtualHeight)
	{
		instance->virtualWidth = virtualWidth;
		instance->virtualHeight = virtualHeight;

		GLint viewPort[4];
		glGetIntegerv(GL_VIEWPORT, viewPort);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0.0f, Platform::GetWidth(), Platform::GetHeight(), 0.0f, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//instance->resolutionScale = Vector2(float(Platform::GetWidth())/virtualWidth, float(Platform::GetHeight())/virtualHeight);

		// avoid vertical stretch:

		// if > 4:3, do something else
		instance->resolutionScale = Vector2(float(Platform::GetWidth())/virtualWidth, float(Platform::GetWidth())/virtualWidth);

		printf("Set2D: resScale: (%f, %f)\n window (%d, %d)\n", instance->resolutionScale.x, instance->resolutionScale.y, Platform::GetWidth(), Platform::GetHeight());
		screenCenter.x = virtualWidth/2;
		screenCenter.y = virtualHeight/2;
	}

	void Graphics::Set3D()
	{
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

		Set2D(instance->virtualWidth,instance->virtualHeight);
	}

	void Graphics::SetCameraPosition(const Vector2 &position)
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

	void Graphics::Rotate(float r, float ax, float ay, float az)
	{
		glRotatef(r, ax, ay, az);
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

	/*
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
	*/


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

	void Graphics::RenderQuad(float width, float height, Vector2 textureOffset, Vector2 textureScale)
	{
		float halfWidth = width*0.5f;
		float halfHeight = height*0.5f;

		glBegin(GL_QUADS);
			glVertex3f(-halfWidth, -halfHeight, 0.0f);
			glTexCoord2f(textureOffset.x + textureScale.x, textureOffset.y);
			glVertex3f(halfWidth, -halfHeight, 0.0f);
			glTexCoord2f(textureOffset.x + textureScale.x, textureOffset.y + textureScale.y);
			glVertex3f(halfWidth, halfHeight, 0.0f);
			glTexCoord2f(textureOffset.x, textureOffset.y + textureScale.y);
			glVertex3f(-halfWidth, halfHeight, 0.0f);
			glTexCoord2f(textureOffset.x, textureOffset.y);
		glEnd();
	}

	void Graphics::BeginFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();									// Reset The Current Modelview Matrix

		glTranslatef(cameraPosition.x - screenCenter.x, cameraPosition.y - screenCenter.y, 0.0f);
		glScalef(resolutionScale.x, resolutionScale.y, 0.0f);
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
			if (instance->lastBoundTextureID != textureAsset->texID)
			{
				glBindTexture(GL_TEXTURE_2D, textureAsset->texID);
				instance->lastBoundTextureID = textureAsset->texID;
			}
		}
		else
		{
			if (instance->lastBoundTextureID != 0)
			{
				glBindTexture(GL_TEXTURE_2D, 0);
				instance->lastBoundTextureID = 0;
			}
		}
	}
}

#endif
