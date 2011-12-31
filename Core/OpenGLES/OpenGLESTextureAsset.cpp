#ifdef MONOCLE_OPENGLES

#include "../TextureAsset.h"
#include "../Debug.h"
#include "../MonocleToolkit.h"

#ifdef MONOCLE_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#define STBI_HEADER_FILE_ONLY
#include <stb/stb_image.c>

#include "OpenGLESFrameBuffer.h"
#include "../Graphics.h"

namespace Monocle
{
	TextureAsset::TextureAsset()
    : Asset(ASSET_TEXTURE)
	{
	}
    
    bool TextureAsset::IsPremultiplied()
    {
        return this->premultiplied;
    }
    
	void TextureAsset::Load(const unsigned char* data, int w, int h, FilterType filter, bool repeatX, bool repeatY, bool premultiply)
	{
		this->filter = filter;
		this->repeatX = repeatX;
		this->repeatY = repeatY;
        this->premultiplied = premultiply;
        
        if (premultiply)
            PremultiplyAlpha((unsigned char*)data,w,h);
        
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
        
		// try to avoid ATI driver bug, see: http://www.opengl.org/wiki/Common_Mistakes#Automatic_mipmap_generation
		// "glGenerateMipmap doesn't work on ATI as of 2011" hmmm...
		glEnable(GL_TEXTURE_2D);
        
		// choose GL_NEAREST
		unsigned int glMagFilter = GL_NEAREST;
		unsigned int glMinFilter = GL_NEAREST;
        
		if (filter == FILTER_LINEAR)
		{
			glMagFilter = GL_LINEAR;
			glMinFilter = GL_LINEAR_MIPMAP_LINEAR;
		}
        
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMinFilter);
        
		unsigned int glRepeatX = repeatX?GL_REPEAT:GL_CLAMP_TO_EDGE;
		unsigned int glRepeatY = repeatY?GL_REPEAT:GL_CLAMP_TO_EDGE;
        
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glRepeatX);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glRepeatY);
        
		width = (unsigned int)w;
		height = (unsigned int)h;
        
		// mipmaps: OpenGL 1.4 version
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        // Todo: Mipmaps?
//        glGenerateMipmap(GL_TEXTURE_2D);
        
		Debug::Log("Loaded texture from data");
	}
    
	bool TextureAsset::Load(const std::string &filename, FilterType filter, bool repeatX, bool repeatY, bool premultiply)
	{
		this->filter = filter;
		this->repeatX = repeatX;
		this->repeatY = repeatY;
		this->filename = filename;
        this->premultiplied = premultiply;
        
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
        
		// try to avoid ATI driver bug, see: http://www.opengl.org/wiki/Common_Mistakes#Automatic_mipmap_generation
		// "glGenerateMipmap doesn't work on ATI as of 2011" hmmm...
		glEnable(GL_TEXTURE_2D);
        
		// choose GL_NEAREST
		unsigned int glMagFilter = GL_NEAREST;
		unsigned int glMinFilter = GL_NEAREST;
        
		if (filter == FILTER_LINEAR)
		{
			glMagFilter = GL_LINEAR;
			glMinFilter = GL_LINEAR_MIPMAP_LINEAR;
		}
        
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMinFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

        
		unsigned int glRepeatX = repeatX?GL_REPEAT:GL_CLAMP_TO_EDGE;
		unsigned int glRepeatY = repeatY?GL_REPEAT:GL_CLAMP_TO_EDGE;
        
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glRepeatX);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glRepeatY);
        
		int w,h,n;
		unsigned char* data = stbi_load(filename.c_str(), &w, &h, &n, STBI_rgb_alpha);
        
        if (premultiply)
            PremultiplyAlpha(data,w,h);
        
		if (data)
		{
			width = (unsigned int)w;
			height = (unsigned int)h;
            
			// mipmaps: OpenGL 1.4 version
			//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
            
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            
			// mipmaps: OpenGL 3.0 version

            // TODO: Mipmaps?
//			glGenerateMipmap(GL_TEXTURE_2D);
            
            Graphics::CheckErrors();
            
			//Debug::Log("Loaded texture: " + filename);
			return true;
		}
		else
		{
			Debug::Log("Can't load texture:");
			Debug::Log(filename);
            
			width = height = 64;
			return false;
		}
	}
    
	void TextureAsset::UpdateRect(const unsigned char *data, Monocle::Vector2 position, Monocle::Vector2 size)
	{
		//allows updating of a portion of the texture.
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, position.x, position.y, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
    
	void TextureAsset::CopyRect(Monocle::Vector2 srcPos, Monocle::Vector2 dstPos, Monocle::Vector2 copysize)
	{
		unsigned char *cpyrect = ReadRect(srcPos, copysize);
		UpdateRect(cpyrect, dstPos, copysize);
		delete cpyrect;
	}
    
    void _getTextureImageData( TextureAsset *texture, unsigned char *data )
    {
        Debug::Log("_getTextureImageData called");
        
        OpenGLFrameBuffer *fb = new OpenGLFrameBuffer(texture->width,texture->height,true);
        
        fb->Use();
        
        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, texture->texID);
        
        // Render it
        Graphics::PushMatrix();
        Graphics::Translate(Vector2(texture->width*0.5,texture->height*0.5));
        Graphics::RenderQuad(texture->width,texture->height);
        Graphics::PopMatrix();
        
        // Kinda slow...
        glReadPixels(0,0,texture->width,texture->height,GL_RGBA,GL_UNSIGNED_BYTE,data);
        
        fb->Stop();
        
        delete fb;
    }
    
	unsigned char *TextureAsset::ReadRect(Monocle::Vector2 srcPos, Monocle::Vector2 size)
	{
		if(srcPos.x < 0)
		{
			size.x += srcPos.x; //srcPos.x is negative
			srcPos.x = 0;
		}
		if(srcPos.y < 0)
		{
			size.y += srcPos.y; //srcPos.y is negative
			srcPos.y = 0;
		}
        
		if(srcPos.x + size.x > width) size.x = width - srcPos.x;
		if(srcPos.y + size.y > height) size.y = height - srcPos.y;
        
		if(	size.x	<= 0
           ||	size.y	<= 0
           ||	srcPos.x + size.x < 0
           ||	srcPos.y + size.y < 0 ) return NULL;
        
		glBindTexture(GL_TEXTURE_2D, texID);
        
		//get the image data back out of gl
		unsigned char *data = new unsigned char[width * height * 4];
        
//		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        _getTextureImageData(this,data);
        
		unsigned char *buffer = new unsigned char [(int)size.y * (int)size.x * 4];
		unsigned char *buffPos = buffer;
		for(int y = srcPos.y; y - srcPos.y < size.y; y++)
		{
			//copy a line of pixels the width of the to-be-copied area from the image.
			unsigned char *begin = data + (width * y * 4) + (int)srcPos.x * 4;
            
			std::copy(begin, begin + (int)size.x * 4, buffPos);
            
			//increment the buffer position to accept the next row
			buffPos += ((int)size.x * 4);
		}
        
		delete data;
		data = NULL;
		buffPos = NULL;
        
		//Monocle::Debug::Log("Done reading rect");
		//Monocle::Debug::Log((int)buffer[0]);
		return buffer;
	}
    
	void TextureAsset::Reload()
	{
		if(filename.length() != 0)
		{
			Unload();
			Load(filename, filter, repeatX, repeatY);
		}
		else
		{
			//loaded from pointer.
		}
	}
    
	void TextureAsset::Unload()
	{
		Debug::Log("Freeing texture memory for: " + filename);
		if (filename.find("tower-wall-rep") != std::string::npos)
		{
			int p;
			p = 55;
		}
		glDeleteTextures(1, &texID);
	}
}

#endif


