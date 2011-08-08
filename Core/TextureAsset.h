#pragma once
 
#include "Asset.h"
 
namespace Monocle
{
	class Vector2;
	
	class TextureAsset : public Asset
	{
	public:
		TextureAsset();
		//! \brief Loads a texture from a pointer.
		//! \param data [in] A pointer to the data to load into the texture.  May be null, and if so, a buffer large enough to fit a w*h image is allocated and may
		//!     be filled at a later time using UpdateRect
		//! \param w [in] The width of the image to load
		//! \param h [in] The height of the image to load
		//! \param filter [in] The type of filter to use on the image
		//! \param repeatX [in] Whether or not the texture should be repeated on the x axis if necessary when rendered
		//! \param repeatY [in] Whether or not the texture should be repeated on the y axis if necessary when rendered
        //! \param premultiply [in] Whether or not to apply premulitplication to the texture as it loads
		void Load(const unsigned char* data, int w, int h, FilterType filter, bool repeatX, bool repeatY, bool premultiply = false);
		//! \brief Loads a texture from a file
		//! \return Whether the load was successful.  If this is false, the texture is invalid and should not be used except to attempt another Load
		//! \param filename [in] The filename to load from, relative to the current ContentPath
		//! \param filter [in] The filter to use on the texture.
		//! \param repeatX [in] Whether or not the texture should be repeated on the x axis if necessary when rendered
		//! \param repeatY [in] Whether or not the texture should be repeated on the y axis if necessary when rendered
        //! \param premultiply [in] Whether or not to apply premulitplication to the texture as it loads
		//! \sa Assets::GetContentPath
		bool Load(const std::string &filename, FilterType filter, bool repeatX, bool repeatY, bool premultiply = false);
		//! \brief Replaces the pixels in a rectangular area of the texture using the data pointed to.
		//! \param data [in] The pointer to the data to fill the rectangle with
		//! \param position [in] The top left hand corner of the rect to update
		//! \param size [in] The size of the rect to update
		void UpdateRect(const unsigned char *data, Monocle::Vector2 position, Monocle::Vector2 size);
		//! \brief Copies a portion of the texture into another area of the same texture.
		//! \param srcPos [in] The top left corner of the rect to copy from
		//! \param dstPos [in] The top left corner of the rect to copy to
		//! \param size [in] The size of the rect to copy
		//! \remarks This function produces a 1:1 scale copy.
		void CopyRect(Monocle::Vector2 srcPos, Monocle::Vector2 dstPos, Monocle::Vector2 copysize);
		//! \brief Copies a portion of the texture into a buffer, and returns it.
		unsigned char *ReadRect(Monocle::Vector2 srcPos, Monocle::Vector2 size);
		//! \brief Reloads the texture
		void Reload();
		//! \brief Unloads the texture
		void Unload();
        
        //! \brief Does the texture contain premultiplied alpha?
        bool IsPremultiplied();
 
		//should have getter functions and be private
		unsigned int width, height;
		unsigned int texID;
		bool repeatX, repeatY;
        bool premultiplied;
 
	private:
		FilterType filter;
		
	};
}
 
