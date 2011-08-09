#pragma once

#include "FileNode.h"
#include "Vector2.h"
#include "Rect.h"

#include <map>
#include <vector>

class TiXmlDocument;
class TiXmlElement;

namespace Monocle
{
    class XMLFileNode;
    class ZwopSpriteSheet;
    
    // (frames)
    class ZwopSprite
    {
    public:
        
        ZwopSprite( ZwopSpriteSheet *sheet, int index );
        ~ZwopSprite();
        
        void Load(TiXmlElement *element);
        
        Vector2 GetSize();
        Vector2 GetSourceSize();
        std::string GetName();
        Rect GetRect();
        
        Vector2 GetTextureOffset();
        Vector2 GetTextureScale();
        
        Rect GetColorRect();
        Vector2 GetSpriteOffset();
        
        ZwopSpriteSheet *GetSheet();
        
        int GetSpriteIndex();
        
    private:
        
        friend class ZwopSpriteSheet;
        ZwopSpriteSheet *sheet;
        
        Vector2 size;               // Size of sprite
        Vector2 sourceSize;         // Original size of sprite
        
        Vector2 spriteOffset;
        
        Rect rect;
        Rect colorRect;
        
        std::string name;
        
        int spriteIndex;
    };
    
	class ZwopSpriteSheet
	{
    public:
		
		~ZwopSpriteSheet();
		ZwopSprite* GetSpriteByName( const std::string &name );
        ZwopSprite* GetSpriteByIndex( int index );

		std::string GetTextureName();
        
        Vector2 GetSize();
        
        static Vector2 XMLToVector( XMLFileNode *node, std::string key );
        static Rect XMLToRect( XMLFileNode *node, std::string key );
        
        static ZwopSpriteSheet *GetSheet( const std::string &sheetFilename, const std::string &textureFilename );
        
	private:
        ZwopSpriteSheet();
        void Load(TiXmlElement *element, const std::string &textureName);
        
		friend class ZwopSprite;
		std::string image;
		Vector2 size;
        
        std::string textureName;
        
	private:
		std::map<std::string, ZwopSprite*> entries;
        std::vector<ZwopSprite*> orderedSprites;
        
	};
}