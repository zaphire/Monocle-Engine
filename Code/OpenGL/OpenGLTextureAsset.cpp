#ifdef MONOCLE_OPENGL

#include "../TextureAsset.h"
#include "../Debug.h"

#include "glpng/glpng.h"

#ifdef MONOCLE_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#ifdef MONOCLE_LINUX
#   include <GL/gl.h>
#   include <GL/glu.h>
#else
#   include <gl/GL.h>
#   include <gl/GLU.h>
#endif

namespace Monocle
{
	TextureAsset::TextureAsset()
		: Asset(AT_TEXTURE)
	{
	}

	void TextureAsset::Load(const std::string &filename)
	{
		this->filename = filename;
		// only load png for now
		pngInfo info;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		// choose GL_NEAREST or PNG_NOMIPMAPS
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		if (pngLoad(filename.c_str(), PNG_BUILDMIPMAPS, PNG_ALPHA, &info))
		{
			width = info.Width;
			height = info.Height;

			Debug::Log("Loaded texture:");
			Debug::Log(filename);
			Debug::Log((int)width);
			Debug::Log((int)height);


			//printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);
		}
		else
		{
			Debug::Log("Can't load texture:");
			Debug::Log(filename);

			width = height = 64;
		}
	}

	void TextureAsset::Unload()
	{
	}

	void TextureAsset::Bind()
	{

	}
}

#endif
