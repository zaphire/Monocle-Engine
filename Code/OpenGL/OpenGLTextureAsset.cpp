#ifdef MONOCLE_OPENGL

#include "../TextureAsset.h"
#include "../Debug.h"

#ifdef MONOCLE_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#if defined(MONOCLE_MAC)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#elif defined(MONOCLE_LINUX)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <gl/GL.h>
#include <gl/GLu.h>
#endif

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

		// choose GL_NEAREST
		unsigned int glFilter = GL_NEAREST;

		if (filter == FILTER_LINEAR)
			glFilter = GL_LINEAR;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);

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
			//glGenerateMipmap(GL_TEXTURE_2D);

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