#ifdef MONOCLE_OPENGL

#include "../Graphics.h"
#include "../Debug.h"
#include "../Platform.h"
#include "../TextureAsset.h"
#include "../FontAsset.h"
#include "../Color.h"
#include "../Tween.h"
#include "../LevelEditor/Node.h"

#include <cstdlib>
#include <cstdio>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#define GLEW_STATIC
#include <GL/glew.h>

// some opengl/windows init code based on http://nehe.gamedev.net

namespace Monocle
{
	Graphics *Graphics::instance = NULL;

	Graphics::Graphics()
	{
		instance = this;
		lastBoundTextureID = 0;
        bgReset = true;
        currentBlend = BLEND_NONE;
	}

	void Graphics::Init()
	{
		Debug::Log("Graphics::Init");
		
		if (GLEW_OK != glewInit())
		{
			// GLEW failed!
			Debug::Log("glewInit failed!");
			exit(1);
		}

		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glCullFace(GL_BACK);
		//glShadeModel(GL_SMOOTH);	
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glDisable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);
		//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		//clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ShowBuffer();

        currentColor = Color::white;
        glColor4f(1.0,1.0,1.0,1.0);

		Set2D(800,600);

		//cameraPosition = screenCenter;
		//cameraZoom = Vector2::one;
	}

	void Graphics::Viewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void Graphics::SetBackgroundColor(const Color &color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	Color Graphics::GetBackgroundColor()
	{
		GLfloat *clearColor = new GLfloat[4];
		glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
		Color color(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
		delete[] clearColor;
		return color;
	}

	bool Graphics::SetResolution(int w, int h, int bits, bool full)
	{
		//TODO: tell platform to change window size
		Resize(w, h);
		return true;
	}

	void Graphics::SetBlend(BlendType blend)
	{
		if (blend != instance->currentBlend)
		{
			switch (blend)
			{
			case BLEND_ALPHA:
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
                break;
            case BLEND_ALPHA_PREMULTIPLIED:
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case BLEND_ADDITIVE:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			case BLEND_MULTIPLY:
				glBlendFunc(GL_ZERO, GL_SRC_COLOR);
				break;
            default:
                break;
			}
			instance->currentBlend = blend;
		}
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

//		printf("Set2D: resScale: (%f, %f)\n window (%d, %d)\n", instance->resolutionScale.x, instance->resolutionScale.y, Platform::GetWidth(), Platform::GetHeight());
		instance->screenCenter = Vector2(virtualWidth/2, virtualHeight/2);

//		printf("Set2D: center: (%f, %f)\n", instance->screenCenter.x, instance->screenCenter.y);
	}

	//void Graphics::Set3D()
	//{
	//}

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

	//void Graphics::SetCameraPosition(const Vector2 &position)
	//{
	//	instance->cameraPosition = position;
	//}

	//void Graphics::AdjustCameraPosition(const Vector2 &adjustment)
	//{
	//	instance->cameraPosition += adjustment;
	//}

	//void Graphics::SetCameraZoom(const Vector2 &zoom)
	//{
	//	instance->cameraZoom = zoom;
	//}
	//
	//void Graphics::AdjustCameraZoom(const Vector2 &adjustment)
	//{
	//	instance->cameraZoom += adjustment;

	//	if (instance->cameraZoom.x < 0.0f)
	//		instance->cameraZoom.x = 0.0f;
	//	if (instance->cameraZoom.y < 0.0f)
	//		instance->cameraZoom.y = 0.0f;
	//}

	//const Vector2 &Graphics::GetCameraPosition()
	//{
	//	return instance->cameraPosition;
	//}

	//const Vector2 &Graphics::GetCameraZoom()
	//{
	//	return instance->cameraZoom;
	//}

	//void Graphics::MoveCameraPosition(const Vector2 &position, float time, EaseType easeType)
	//{
	//	Tween::To(&instance->cameraPosition, position, time, easeType);
	//}

	const Vector2 &Graphics::GetResolutionScale()
	{
		return instance->resolutionScale;
	}

	void Graphics::Translate(float x, float y, float z)
	{
		glTranslatef(x, y, z);
	}

	void Graphics::Translate(const Vector2 &pos)
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

	void Graphics::Scale(const Vector2 &scale)
	{
		glScalef(scale.x, scale.y, 1.0f);
	}

	void Graphics::MultiplyMatrix(float *m)
	{
		glMultMatrixf(m);
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


	void Graphics::RenderLine(const Vector2 &pos1, const Vector2 &pos2)
	{
		glBegin(GL_LINES);
			glVertex3f(pos1.x, pos1.y, 0.0f);
			glVertex3f(pos2.x, pos2.y, 0.0f);
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
        if (instance->currentColor != color){
            glColor4f(color.r, color.g, color.b, color.a);
            instance->currentColor = color;
        }
	}

	int Graphics::GetVirtualWidth()
	{
		return instance->virtualWidth;
	}

	int Graphics::GetVirtualHeight()
	{
		return instance->virtualHeight;
	}

	Vector2 Graphics::GetScreenCenter()
	{
		return instance->screenCenter;
	}

	void Graphics::RenderQuadCustom(const Vector2 &ul, const Vector2 &ur, const Vector2 &lr, const Vector2 &ll, const Vector2 &textureOffset, const Vector2 &textureScale)
	{
		glBegin(GL_QUADS);
			// UL
			glTexCoord2f(textureOffset.x, textureOffset.y);
			glVertex3f(ul.x, ul.y, 0.0f);

			//UR
			glTexCoord2f(textureOffset.x + textureScale.x, textureOffset.y);
			glVertex3f(ur.x, ur.y, 0.0f);

			//LR
			glTexCoord2f(textureOffset.x + textureScale.x, textureOffset.y + textureScale.y);
			glVertex3f(lr.x, lr.y, 0.0f);

			//LL
			glTexCoord2f(textureOffset.x, textureOffset.y + textureScale.y);
			glVertex3f(ll.x, ll.y, 0.0f);
		glEnd();
	}

	void Graphics::RenderQuad(float width, float height, const Vector2 &textureOffset, const Vector2 &textureScale, const Vector2 &position)
	{
		float halfWidth = width*0.5f;
		float halfHeight = height*0.5f;

		glBegin(GL_QUADS);
			// UL
			glTexCoord2f(textureOffset.x, textureOffset.y);
			glVertex3f(-halfWidth + position.x, -halfHeight + position.y, 0.0f);
			
			//UR
			glTexCoord2f(textureOffset.x + textureScale.x, textureOffset.y);
			glVertex3f(halfWidth + position.x, -halfHeight + position.y, 0.0f);
			
			//LR
			glTexCoord2f(textureOffset.x + textureScale.x, textureOffset.y + textureScale.y);
			glVertex3f(halfWidth + position.x, halfHeight + position.y, 0.0f);
			
			//LL
			glTexCoord2f(textureOffset.x, textureOffset.y + textureScale.y);
			glVertex3f(-halfWidth + position.x, halfHeight + position.y, 0.0f);
		glEnd();
	}
    
    void Graphics::RenderText(const FontAsset& font, const std::string& text, float x, float y, TextAlign x_align)
    {
        Rect verts, texCoords;
        float width;
        
        width = font.GetTextWidth(text);
        
        glPushMatrix();
        
        if (x_align == TEXTALIGN_RIGHT){
            glTranslatef( -width, 0.0, 0.0 );
        }else if (x_align == TEXTALIGN_CENTER){
            glTranslatef( width/-2.0, 0.0, 0.0 );
        }
        
        glBegin(GL_QUADS);
        for (int i = 0; i < text.size(); i++)
        {
            char c = text[i];
			if ((c >= 32) && (c < 128))
			{
				font.GetGlyphData(c, &x, &y, verts, texCoords);

				glTexCoord2f(texCoords.topLeft.x, texCoords.topLeft.y);
				glVertex2f(verts.topLeft.x, verts.topLeft.y);

				glTexCoord2f(texCoords.bottomRight.x, texCoords.topLeft.y);
				glVertex2f(verts.bottomRight.x, verts.topLeft.y);

				glTexCoord2f(texCoords.bottomRight.x, texCoords.bottomRight.y);
				glVertex2f(verts.bottomRight.x, verts.bottomRight.y);

				glTexCoord2f(texCoords.topLeft.x, texCoords.bottomRight.y);
				glVertex2f(verts.topLeft.x, verts.bottomRight.y);
			}
        }
        glEnd();
        
        glPopMatrix();
    }
    
	void Graphics::BeginFrame()
	{
        if (instance->bgReset)
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	}

	void Graphics::DefaultMatrix()
	{
		glLoadIdentity();
		glScalef(instance->resolutionScale.x, instance->resolutionScale.y, 0.0f);
		glTranslatef(instance->screenCenter.x, instance->screenCenter.y, 0.0f);
	}

	void Graphics::ResolutionMatrix()
	{
		glLoadIdentity();
		glScalef(instance->resolutionScale.x, instance->resolutionScale.y, 0.0f);
		//glTranslatef(instance->screenCenter.x, instance->screenCenter.y, 0.0f);
	}

	void Graphics::IdentityMatrix()
	{
		glLoadIdentity();
	}

	//void Graphics::SceneMatrix()
	//{
	//	glLoadIdentity();
	//	glScalef(instance->resolutionScale.x, instance->resolutionScale.y, 0.0f);
	//	glTranslatef(instance->screenCenter.x, instance->screenCenter.y, 0.0f);
	//	glScalef(instance->cameraZoom.x, instance->cameraZoom.y, 0.0f);
	//	glTranslatef(-1.0f * instance->cameraPosition.x, -1.0f * instance->cameraPosition.y, 0.0f);
	//}

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
    
    void Graphics::BindFont(FontAsset* fontAsset)
	{
		if (fontAsset != NULL)
		{
			if (instance->lastBoundTextureID != fontAsset->texID)
			{
				glBindTexture(GL_TEXTURE_2D, fontAsset->texID);
				instance->lastBoundTextureID = fontAsset->texID;
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

	Vector2 Graphics::GetMatrixPosition()
	{
		float m[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, m);
		float x = m[12];
		float y = m[13];
		//float z = m[14];
		return Vector2(x, y);
	}

	void Graphics::BeginLine()
	{
		glBegin(GL_LINES);
	}

	void Graphics::BeginLineStrip()
	{
		glBegin(GL_LINE_STRIP);
	}

	void Graphics::Vertex(Vector2 vertex)
	{
		glVertex2f(vertex.x, vertex.y);
	}

	void Graphics::EndLine()
	{
		glEnd();
	}

	void Graphics::RenderPathMesh(const std::vector<Node*> &nodes, int cells, float size, bool flipX, bool flipY, Vector2 textureOffset, Vector2 textureScale)
	{
		glBegin(GL_QUADS);
		for (int i = 0; i < nodes.size()-1; i++)
		{
			if (nodes[i]->variant != -1 && nodes[i]->variant < cells)
			{
				Vector2 diff1;
				Vector2 perp1;

				/*
				if (i-1 >= 0)
				{
					diff1 = points[i] - points[i-1];
					perp1 = diff1.GetNormalized().GetPerpendicularLeft();
					//diff1 = points[i + 1] - points[i];
					//perp1 = perp1*0.5f + diff1.GetNormalized().GetPerpendicularLeft()*0.5f;
				}
				else
				{
					diff1 = points[i + 1] - points[i];
					perp1 = diff1.GetNormalized().GetPerpendicularLeft();
				}
				*/

				diff1 = nodes[i + 1]->position - nodes[i]->position;
				perp1 = diff1.GetNormalized().GetPerpendicularLeft();

				Vector2 diff2;
				Vector2 perp2 = perp1;
			
				if (i+2 < nodes.size())
				{
					diff2 = nodes[i+2]->position - nodes[i+1]->position;
					perp2 = diff2.GetNormalized().GetPerpendicularLeft();
				}
				else
				{
					perp2 = perp1;
				}
			
				Vector2 pos1 = nodes[i]->position;
				Vector2 pos2 = nodes[i+1]->position;
			
				Vector2 texOffset = textureOffset;
				Vector2 texScale = textureScale * 1.0f/(float)cells;
				texOffset.x += (nodes[i]->variant % (cells)) * texScale.x;
				texOffset.y += (int)(nodes[i]->variant / (cells)) * texScale.y;

				if (flipY)
				{
					texOffset.y = (textureOffset.y+textureScale.y) - texOffset.y;
					texScale.y = - texScale.y;
					//printf("%f, %f\n", texOffset.y, texScale.y);
				}
			
				Graphics::SetColor(nodes[i]->color);
				glTexCoord2f(texOffset.x, texOffset.y);
				Vertex(pos1 - perp1 * nodes[i]->scale.y * size * 0.5f);

				Graphics::SetColor(nodes[i+1]->color);
				glTexCoord2f(texOffset.x + texScale.x, texOffset.y);
				Vertex(pos2 - perp2 * nodes[i+1]->scale.y * size * 0.5f);

				Graphics::SetColor(nodes[i+1]->color);
				glTexCoord2f(texOffset.x + texScale.x, texOffset.y + texScale.y);
				Vertex(pos2 + perp2 * nodes[i+1]->scale.y * size * 0.5f);

				Graphics::SetColor(nodes[i]->color);
				glTexCoord2f(texOffset.x, texOffset.y + texScale.y);
				Vertex(pos1 + perp1 * nodes[i]->scale.y * size * 0.5f);
			}
		}
		glEnd();
	}
    
    void Graphics::EnableBackgroundReset( bool bgReset )
    {
        instance->bgReset = bgReset;
    }
    
    void Graphics::ScreenToImage(const std::string &filename, ImageType type)
    {
        int w = Platform::GetWidth(),
            h = Platform::GetHeight();
        char *data = new char[w * h * 3];
        glReadPixels(0,0, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
        
        char* tmpline = new char[w*3];
        
        const int linewidth = w * 3;
        
        //flip the image
        for(int y = 0; y < (h/2); y++)
        {
            std::copy(data + y * linewidth, data + y * linewidth + linewidth, tmpline);
            std::copy(data + (h-y) * linewidth, data + (h-y) * linewidth + linewidth, data + y * linewidth);
            std::copy(tmpline, tmpline + linewidth, data + (h-y) * linewidth);
        }
        
        switch(type)
        {
            case IMAGE_PNG:
                stbi_write_png( filename.data(), w, h, 3, data, w * 3 ); break;
            case IMAGE_BMP:
                stbi_write_bmp( filename.data(), w, h, 3, data); break;
            case IMAGE_TGA:
                stbi_write_tga( filename.data(), w, h, 3, data); break;
        }
        
        delete tmpline;
        delete data;
    }
    
    void Graphics::CheckErrors()
    {
        GLenum err = glGetError();
        if (err == GL_NO_ERROR)
            return;
        
        Debug::Log("GL ERROR: " + std::string((char*)gluErrorString(err)));
    }
}

#endif
