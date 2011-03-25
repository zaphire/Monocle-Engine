#ifdef MONOCLE_OPENGL

#include "../TextureAsset.h"
#include "../Debug.h"

#include "glpng/glpng.h"

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
		// only load png for now
		pngInfo info;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		// choose GL_NEAREST or PNG_NOMIPMAPS
		unsigned int glFilter = GL_NEAREST;

		if (filter == FILTER_LINEAR)
			glFilter = GL_LINEAR;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);

		unsigned int glRepeatX = repeatX?GL_REPEAT:GL_CLAMP;
		unsigned int glRepeatY = repeatY?GL_REPEAT:GL_CLAMP;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glRepeatX);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glRepeatY);

		if (pngLoad(filename.c_str(), PNG_BUILDMIPMAPS, PNG_ALPHA, &info))
		{
			width = info.Width;
			height = info.Height;

			Debug::Log("Loaded texture: " + filename);
			//printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);
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
