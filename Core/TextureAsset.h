#pragma once

#include "Asset.h"
#include "Vector2.h"

namespace Monocle
{
	class TextureAsset : public Asset
	{
	public:
		TextureAsset();
		//Loads a texture from a pointer.
		//The data pointer may be null, in which case UpdateRect can later be used to fill the texture with
		//an image.
		void Load(const unsigned char* data, int w, int h, FilterType filter, bool repeatX, bool repeatY);
		bool Load(const std::string &filename, FilterType filter, bool repeatX, bool repeatY);
        //Replaces the pixels in a rectangular area of the texture using the data pointed to.
		void UpdateRect(const unsigned char *data, Monocle::Vector2 position, Monocle::Vector2 size);
        //Copies a portion of the texture into another area of the same texture.
		void CopyRect(Monocle::Vector2 srcPos, Monocle::Vector2 dstPos, Monocle::Vector2 copysize);
		void Reload();
		void Unload();

        //Copies a portion of the texture into a buffer, and returns it.
        unsigned char *ReadRect(Monocle::Vector2 srcPos, Monocle::Vector2 size);

		FilterType filter;
		bool repeatX, repeatY;
		unsigned int width, height;
		unsigned int texID;

    private:
	};
}
