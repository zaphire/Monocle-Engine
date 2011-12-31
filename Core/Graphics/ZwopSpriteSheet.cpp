
#include "ZwopSpriteSheet.h"
#include "../Assets.h"
#include "../TextureAsset.h"

#include <XML/XMLFileNode.h>
#include <TinyXML/tinyxml.h>

#include <sstream>
#include <iostream>

#include <assert.h>

namespace Monocle
{
    ZwopSprite::ZwopSprite( ZwopSpriteSheet *sheet, int index )
    {
        this->sheet = sheet;
        this->spriteIndex = index;
    }
    
    ZwopSprite::~ZwopSprite()
    {
        
    }
    
    void ZwopSprite::Load(TiXmlElement *element)
    {
        // This is <Sprite>
        // <Sprite name="tree.png" size="{64, 116}" sourcesize="{64, 128}" rect="{{2, 2}, {64, 116}}" />
        
        XMLFileNode zwopSprite(element);
        zwopSprite.Read("name",name);
        size = ZwopSpriteSheet::XMLToVector(&zwopSprite,"size");
        sourceSize = ZwopSpriteSheet::XMLToVector(&zwopSprite,"sourceSize");
        rect = ZwopSpriteSheet::XMLToRect(&zwopSprite,"rect");
        colorRect = ZwopSpriteSheet::XMLToRect(&zwopSprite, "colorRect");
        spriteOffset = ZwopSpriteSheet::XMLToVector(&zwopSprite, "offset");
    } 
    
    Rect ZwopSprite::GetColorRect()
    {
        return colorRect;
    }
    
    Vector2 ZwopSprite::GetTextureOffset()
    {
        // TODO: Trimming?
        return Vector2( ( rect.topLeft.x )  / sheet->size.x, ( rect.topLeft.y )  / sheet->size.y );
    }
    
    Vector2 ZwopSprite::GetTextureScale()
    {
        return Vector2( size.x / sheet->size.x, size.y / sheet->size.y );
    }
    
    Vector2 ZwopSprite::GetSize()
    {
        return size;
    }
    
    Vector2 ZwopSprite::GetSourceSize()
    {
        return sourceSize;
    }
    
    std::string ZwopSprite::GetName()
    {
        return name;
    }
    
    Rect ZwopSprite::GetRect()
    {
        return rect;
    }
    
    Vector2 ZwopSprite::GetSpriteOffset()
    {
        return spriteOffset;
    }
    
    int ZwopSprite::GetSpriteIndex()
    {
        return spriteIndex;
    }
    
    ZwopSpriteSheet *ZwopSprite::GetSheet()
    {
        return sheet;
    }
    
    Vector2 ZwopSpriteSheet::XMLToVector( XMLFileNode *node, std::string key )
    {
        std::string instr;
        node->Read(key,instr);
        std::istringstream read(instr);
        
        // {64, 64}
        int x,y;
        char br;
        read >> br;
        
        assert(br == '{');
        
        read >> x;
        
        read >> br;
        
        assert(br == ',');
        
        read >> y;
        
        return Vector2(x,y);
    }
    
    Rect ZwopSpriteSheet::XMLToRect( XMLFileNode *node, std::string key )
    {
        std::string instr;
        node->Read(key,instr);
        std::istringstream read(instr);
        
        // {{64, 64}, {64, 64}}
        Rect out;
        int x,y;
        char br;
        read >> br;
        assert(br == '{');
        read >> br;
        assert(br == '{');
        
        read >> x;
        read >> br;
        assert(br == ',');
        read >> y;
        out.topLeft = Vector2(x,y);
        
        read >> br;
        assert(br == '}');
        read >> br;
        assert(br == ',');
        read >> br;
        assert(br == '{');
        
        read >> x;
        read >> br;
        assert(br == ',');
        read >> y;
        out.bottomRight = Vector2(x,y);
        
        return out;
    }
    
    void ZwopSpriteSheet::Load(TiXmlElement *element, const std::string &textureName)
	{
        // This is <ZwopSpriteSheet>
        XMLFileNode zwopSheetNode(element);
        size = XMLToVector( &zwopSheetNode, "size" );
        
        TiXmlElement *xmlSprite = element->FirstChildElement("Sprite");
		while (xmlSprite)
		{
			XMLFileNode zwopSpriteNode(xmlSprite);
            
			ZwopSprite *zSprite = new ZwopSprite(this,orderedSprites.size());
			zSprite->Load(xmlSprite);
			if (zSprite->name != "")
				entries[zSprite->name] = zSprite;
            
            orderedSprites.push_back(zSprite);
            
			xmlSprite = xmlSprite->NextSiblingElement("Sprite");
		}
        
        this->textureName = textureName;
        this->texture = Assets::RequestTexture(textureName);
    }
    
    ZwopSpriteSheet::~ZwopSpriteSheet()
    {
        for (std::map<std::string, ZwopSprite*>::iterator i = entries.begin(); i != entries.end(); ++i)
		{
			delete (*i).second;
		}
		entries.clear();
        orderedSprites.clear();
        
        this->texture->RemoveReference();
    }
    
    std::string ZwopSpriteSheet::GetTextureName()
    {
        return textureName;
    }
    
    ZwopSprite* ZwopSpriteSheet::GetSpriteByName(const std::string &name)
    {
        return entries[name];
    }
    
    ZwopSprite* ZwopSpriteSheet::GetSpriteByIndex(int index)
    {
        return orderedSprites[index];
    }
    
    ZwopSpriteSheet *ZwopSpriteSheet::GetSheet( const std::string &sheetFilename, const std::string &textureFilename )
    {
        const std::string cp = Assets::GetContentPath();
        TiXmlDocument ss(cp + sheetFilename);
        bool isLoaded = ss.LoadFile();
        
        if (!isLoaded) return NULL;
        
        TiXmlElement *xmlZwop = ss.FirstChildElement("ZwopSpriteSheet");
        
        if (!xmlZwop) return NULL;
        
        ZwopSpriteSheet *zss = new ZwopSpriteSheet();
        zss->Load(xmlZwop,textureFilename);
        
        return zss;
    }
    
    ZwopSpriteSheet::ZwopSpriteSheet()
    {
        
    }
    
    Vector2 ZwopSpriteSheet::GetSize()
    {
        return size;
    }
}

/**

 <?xml version="1.0" encoding="UTF-8"?>
 <ZwopSpriteSheet size="{{ metadata.size }}">
 {% for sprite in sprites %}
 <Sprite name="{{ sprite.name }}" size="{{ sprite.size }}" sourceSize="{{ sprite.sourceSize }}" rect="{{ sprite.textureRect }}" colorRect="{{ sprite.sourceColorRect }}" offset="{{ sprite.offset }}" />
 {% /for %}
 </ZwopSpriteSheet>

**/