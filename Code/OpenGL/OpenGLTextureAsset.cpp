#ifdef MONOCLE_OPENGL

#include "../TextureAsset.h"
#include "../Debug.h"

#ifdef MONOCLE_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "GL/glew.h"

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

namespace Monocle
{
	TextureAsset::TextureAsset()
		: Asset(ASSET_TEXTURE)
	{
	}

	void TextureAsset::Load(const std::string &filename, FilterType filter, bool repeatX, bool repeatY)
	{
		this->filter = filter;
		this->repeatX = repeatX;
		this->repeatY = repeatY;
		this->filename = filename;

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

		unsigned int glRepeatX = repeatX?GL_REPEAT:GL_CLAMP;
		unsigned int glRepeatY = repeatY?GL_REPEAT:GL_CLAMP;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glRepeatX);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glRepeatY);

		int w,h,n;
		unsigned char* data = stbi_load(filename.c_str(), &w, &h, &n, STBI_rgb_alpha);

		if (data)
		{
			width = (unsigned int)w;
			height = (unsigned int)h;

			// mipmaps: OpenGL 1.4 version
			//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			// mipmaps: OpenGL 3.0 version
			glGenerateMipmap(GL_TEXTURE_2D);

			//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

			Debug::Log("Loaded texture: " + filename);
		}
		else
		{
			Debug::Log("Can't load texture:");
			Debug::Log(filename);

			width = height = 64;
		}
	}

	void TextureAsset::Reload()
	{
		Unload();
		Load(filename, filter, repeatX, repeatY);
	}

	void TextureAsset::Unload()
	{
		Debug::Log("Freeing texture memory for: " + filename);
		glDeleteTextures(1, &texID);
	}
}

#endif