#include "Assets.h"

#include <stdio.h> // for NULL

#include "Audio/AudioAsset.h"
#include "TextureAsset.h"
#include "TTFFontAsset.h"
#include "Debug.h"
#include "Platform.h"

namespace Monocle
{
    class TextureAsset;
    class FontAsset;
    
	Assets *Assets::instance = NULL;

	Assets::Assets()
	{
		instance = this;
	}

	void Assets::Init()
	{
        SetContentPath(Platform::GetDefaultContentPath());
	}

	TextureAsset *Assets::RequestTexture(const std::string &filename, FilterType filter, bool repeatX, bool repeatY, bool premultiply)
	{
		TextureAsset *asset = NULL;
		std::string fullFilename = instance->contentPath + filename;

		//Debug::Log("instance->contentPath + filename: " + fullFilename);

		// check to see if we have one stored already
		asset = (TextureAsset*)instance->GetAssetByFilename(fullFilename);

		// if not, load it and store it
		if (!asset)
		{
			asset = new TextureAsset();
			if(asset->Load(fullFilename, filter, repeatX, repeatY, premultiply))
			{
			    instance->StoreAsset(asset);
			}
			else
			{
			    delete asset;
			    asset = NULL;
			}
		}

		if (asset)
		{
			asset->AddReference();
		}

		// return what we found
		return asset;
	}
    
    FontAsset *Assets::RequestFont(const std::string &filename, float size, int textureWidth, int textureHeight)
    {
		TTFFontAsset *asset = NULL;
		std::string fullFilename = instance->contentPath + filename;
        
		//Debug::Log("instance->contentPath + filename: " + fullFilename);
        
		// check to see if we have one stored already
		asset = (TTFFontAsset*)instance->GetAssetByFilename(fullFilename);
        
		// if not, load it and store it
		if (!asset)
		{
			asset = new TTFFontAsset();
			if (asset->Load(fullFilename, size, textureWidth, textureHeight))
            {
                instance->StoreAsset((Asset*)asset);
            }
            else
            {
                delete asset;
                asset = NULL;
            }
		}
        
		if (asset)
		{
			asset->AddReference();
		}
        
		// return what we found
		return asset;
    }
    
    AudioAsset *Assets::RequestAudio(const std::string &filename, bool streamFromDisk, std::string decodeString)
	{
		AudioAsset *asset = NULL;
		std::string fullFilename = instance->contentPath + filename;
        
		//Debug::Log("instance->contentPath + filename: " + fullFilename);
        
		// check to see if we have one stored already
		asset = (AudioAsset*)instance->GetAssetByFilename(fullFilename);
        
		// if not, load it and store it
		if (!asset)
		{
			asset = new AudioAsset();
            asset->SetDecodeString(decodeString);
			asset->Load(fullFilename, streamFromDisk);
            
            // Return NULL if there was no asset...
            if (!asset->GetDataSize()){
                delete asset;
                return NULL;
            }
            
			instance->StoreAsset((Asset*)asset);
		}
        
		if (asset)
		{
			asset->AddReference();
		}
        
		// return what we found
		return asset;
	}

	void Assets::StoreAsset(Asset *asset)
	{
		// put in some list
		assets.push_back(asset);
	}

	void Assets::RemoveAsset(Asset *asset)
	{
		instance->assets.remove(asset);
	}

	Asset *Assets::GetAssetByFilename(const std::string &filename)
	{
		for(std::list<Asset*>::iterator i = assets.begin(); i != assets.end(); ++i)
		{
			if ((*i)->filename.compare(filename) == 0)
			{
				return (*i);
			}
		}
		return NULL;
	}

	void Assets::SetContentPath(const std::string &contentPath)
	{
		instance->contentPath = contentPath;

		/*
		if (instance->contentPath[instance->contentPath.length()-1] != '/' && instance->contentPath[instance->contentPath.length()-1] != '\\')
		{
			instance->contentPath += '/';
		}
		*/
	}

	const std::string &Assets::GetContentPath()
	{
		return instance->contentPath;
	}


	//generic request asset, not sure if we need this yet
	/*
	Asset* Assets::RequestAsset(AssetType assetType, const char *filename)
	{
		Asset *newAsset = NULL;

		switch(assetType)
		{
		case AT_TEXTURE:
			newAsset = (Asset*)new TextureAsset();
			newAsset->Load(filename);
			instance->StoreAsset((Asset*)newAsset);
			break;
		}

		return newAsset;
	}
	*/

}
