#pragma once

#include "Asset.h"
#include <list>
#include <string>

namespace Monocle
{
	class TextureAsset;
    class FontAsset;
    class AudioAsset;
    class AudioDecoder;

	//!
	//! \brief Loads and manages TextureAsset, FontAsset, AudioAsset
	//!
	//! Call Assets::SetContentPath to initialize the path that all content will be loaded out of.
	//!
	//! Call Assets::RequestTexture, Assets::RequestFont, Assets::RequestAudio to load different each type of Asset.
	//!
	class Assets
	{
	public:
		//! \brief Requests a texture asset
		//! Returns a pointer to the requested Texture Asset.  Calling this function multiple times with the same filename will return
		//! a pointer to the same Texture.  Pointers returned from this function should not be deleted by the caller.
		//! \param filename The path to the requested file, relative to the current content path.
		//! \param FilterType The image filter type to use on the generated texture.
		//! \param repeatX Whether the texture should be repeated horizontally if necessary when rendered.
		//! \param repeatY Whether the texture should be repeated vertically if necessary when rendered.
		//! \sa RequestAudio, RequestFont, FilterType
		static TextureAsset *RequestTexture(const std::string &filename, FilterType filter = FILTER_LINEAR, bool repeatX = false, bool repeatY = false);
		//! \brief Requests a font asset
		//! Returns a pointer to the requested Font Asset.  Calling this function multiple times with the same filename will return
		//! a pointer to the same Font.  Pointers returned from this function should not be deleted by the caller.
		//! \param filename The path to the requested file, relative to the current content path.
		//! \param size The point-size of the font.
		//! \param textureWidth The width of the texture that is generated for the bitmap character sheet.
		//! \param textureHeight The height of the texture that is generated for the bitmap character sheet.
		//! \sa RequestAudio, RequestTexture
        static FontAsset *RequestFont(const std::string &filename, float size, int textureWidth=-1, int textureHeight=-1);
        //! \brief Requests an audio asset
		//! Returns a pointer to the requested Audio Asset.  Calling this function multiple times with the same filename will return
		//! a pointer to the same Audio.  Pointers returned from this function should not be deleted by the caller.
		//! \param filename The path to the requested file, relative to the current content path.
		//! \param streamFromDisk If true, the audio is not pre-loaded into memory
		//! \param decodeString The string used to decode the audio if it has been encoded. Optional, defaults to no encoding.
		//! \sa RequestFont, RequestTexture
		static AudioAsset *RequestAudio(const std::string &filename, bool streamFromDisk=false, std::string decodeString = "");

        //! Changes the root directory that is searched for requested assets.  All filenames given to the RequestTYPE functions are relative
        //! to this path.
        //! \param contentPath The new content path.
        //! \sa GetContentPath
		static void SetContentPath(const std::string &contentPath);

        //! Returns the current Content Path
        //! \return The current content path.
        //! \sa SetContentPath
		static const std::string &GetContentPath();

	protected:
		friend class Asset;
		friend class Game;
		
		Assets();
		void Init();
		
		static void RemoveAsset(Asset *asset);

	private:
		void StoreAsset(Asset *asset);
		Asset* GetAssetByFilename(const std::string &filename);


		static Assets *instance;
		std::list<Asset*> assets;

		std::string contentPath;
	};
}
