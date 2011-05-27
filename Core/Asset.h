#pragma once

#include <string>

namespace Monocle
{
	enum AssetType
	{
		ASSET_UNDEFINED,
		ASSET_TEXTURE,
		ASSET_AUDIO,
        ASSET_FONT,
		ASSET_MAX,
	};

    //! Filter type for TextureAssets.
	enum FilterType
	{
		FILTER_NONE,
		FILTER_LINEAR
	};

    //! \brief Base class for Asset types.
    //! 
    //! Provides functions to manages filename and extensions, and reference counting for all derived AssetTypes.
    //! Derived classes must override void Reload(), void Unload(), and it is expected that they will have a
    //! void Load(const std::string &filename, ...) where ... is any additional necessary parameters.
	class Asset
	{
	public:
        //! Creates an Asset of the given type and initilizes the reference count to 0.  Intended to be called by
        //! constructors of derived classes.
		Asset(AssetType type);
        virtual ~Asset() {}
        
        //! The filename associated with this asset.
		std::string filename;

        //! The type of this asset
		AssetType type;

        //! The number of references to this asset currently in use throughout the program.
        //! This number is incremented when this asset is requested through Assets::RequestTYPE()
		int referenceCount; // <- how many things use this

        //! 
		bool isCached; // <- should we free ourselves?

        //! Increments the reference count of this asset.
		void AddReference();
		//! Decrements the reference count of this asset.
		void RemoveReference();
		//! Reloads this asset. This function is to be overridden by derived classes.
		//! Typically takes the form { Unload(); Load(); }
		virtual void Reload()=0;

        //! Returns the file path relative to the content path.
		std::string GetName();
        //! Returns the file extension.
        std::string GetExtension(); // after the . : "booty.ogg" would return "ogg"

	protected:
		friend class Assets;
		// allow derived classes to add whatever params they want to load
		//virtual void Load(const std::string &filename)=0;
		//! Unloads the asset.  Must be overriden by derived classes.
		virtual void Unload() = 0;
	};
}
