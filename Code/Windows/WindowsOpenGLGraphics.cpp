#include "../Graphics.h"
#include "../Debug.h"
#include "WindowsPlatform.h"

#include <gl/GL.h>
#include <gl/GLU.h>

// opengl/windows init code borrowed from http://nehe.gamedev.net

namespace Monocle
{
	class WindowsGraphics
	{
	public:
		static WindowsGraphics *instance;
		Graphics *graphics;

		static void Init()
		{
			glShadeModel(GL_SMOOTH);								// Enable Smooth Shading
			glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
			glClearDepth(1.0f);									// Depth Buffer Setup
			glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
			glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
		}

		//test
		static void BeginFrame()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
			glLoadIdentity();									// Reset The Current Modelview Matrix

			glTranslatef(instance->graphics->cameraPosition.x, instance->graphics->cameraPosition.y, instance->graphics->cameraPosition.z);
		}

		static void ShowBuffer()
		{
			SwapBuffers(WindowsPlatform::instance->hDC);
		}

		static void SetResolution(int width, int height)		// Resize And Initialize The GL Window
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
	};

	WindowsGraphics *WindowsGraphics::instance = NULL;
	Graphics *Graphics::instance = NULL;

	Graphics::Graphics()
	{
		WindowsGraphics::instance = new WindowsGraphics();
		WindowsGraphics::instance->graphics = this;

		instance = this;
		//WindowsPlatform::instance->hWnd;
	}

	void Graphics::Init()
	{
		Debug::Log("Graphics::Init");
		WindowsGraphics::Init();
		WindowsGraphics::SetResolution(800, 600);
	}

	bool Graphics::SetResolution(int w, int h, int bits, bool full)
	{
		return true;
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
			glVertex3f(halfSize, -halfSize, 0.0f);
			glVertex3f(halfSize, halfSize, 0.0f);
			glVertex3f(-halfSize, halfSize, 0.0f);
		glEnd();
	}

	void Graphics::BeginFrame()
	{
		WindowsGraphics::BeginFrame();
	}

	void Graphics::EndFrame()
	{
	}

	void Graphics::ShowBuffer()
	{
		WindowsGraphics::ShowBuffer();
	}
}