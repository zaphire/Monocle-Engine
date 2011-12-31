#include "ZwopSpriteSheet.h"
#include "SpriteAnimation.h"
#include "Sprite.h"
#include "../MonocleToolkit.h"
#include "../Debug.h"

#include <math.h>

namespace Monocle
{

	Anim::Anim(const std::string &name, int start, int end, float speed, bool loop)
		: name(name), start(start), end(end), speed(speed), frame(start), isPlaying(false), firstSprite(NULL), loop(loop)
	{
        
	}
    
    Anim::Anim(const std::string &name, ZwopSprite *firstSprite, int numFrames, float speed, bool loop)
        : name(name), start(0), end(numFrames-1), speed(speed), frame(0), isPlaying(false), firstSprite(firstSprite), loop(loop)
    {
        
    }

	SpriteAnimation::SpriteAnimation(const std::string &filename, FilterType filter, float width, float height)
		: Sprite(filename, filter, width, height)
	{
        zSheet=NULL;
	}
    
    SpriteAnimation::SpriteAnimation(ZwopSprite *zs, FilterType filter, float width, float height)
    : Sprite(zs, filter, width, height)
	{
        zSheet=NULL;
	}
    
    SpriteAnimation::SpriteAnimation(ZwopSpriteSheet *zSheet, FilterType filter, float width, float height)
    : Sprite(zSheet->GetSpriteByIndex(0), filter, width, height)
	{
        this->zSheet = zSheet;
	}

	void SpriteAnimation::Add(const std::string &name, int start, int end, float speed, bool loop)
	{
		Anim a = Anim(name, start, end, speed, loop);
		animations.push_back(a);
	}
    
    void SpriteAnimation::Add(const std::string &name, std::string startSprite, int numFrames, float speed, bool loop)
    {
        if (!zSheet) return;
        
        Anim a = Anim(name, zSheet->GetSpriteByName(startSprite), numFrames, speed, loop);
        animations.push_back(a);
    }

	void SpriteAnimation::Play(const std::string &name)
	{
		Anim *anim = GetAnim(name);
        
        if (anim == animation && animation->isPlaying == true) return;
        animation = anim;
        
		if (animation)
		{
			animation->isPlaying = true;
            animation->frame = animation->start;
		}

	}

	void SpriteAnimation::Stop(const std::string &name)
	{
		animation = GetAnim(name);
		if (animation)
		{
			animation->isPlaying = false;
		}
	}

	void SpriteAnimation::Update()
	{
		if (animation)
		{
			if (animation->isPlaying)
			{
				animation->frame += animation->speed * Monocle::deltaTime;
                
				while(animation->frame >= animation->end + 1) { 
                    if (!animation->loop){
                        animation->isPlaying = false;
                        animation->frame = animation->end;
                        break;
                    }
                    animation->frame -= (animation->end-animation->start); 
                }
                /*if (animation->frame > animation->end)
                    animation->frame = animation->start;*/
			}
		} 
	}

	void SpriteAnimation::Render(Entity *entity)
	{
		int x = 0;
		int y = 0;
        
        if (zSheet == NULL)
        {
            // Render off full spritesheet as expected
            
            float tw = texture->width*textureScaleModifier.x;
            float th = texture->height*textureScaleModifier.y;

            if (animation)
            {
                x = (int) floor(animation->frame) % (int) (tw / width);
                y = (int) floor(animation->frame) / (tw / width);
            }

            textureOffset = Vector2((x * width) / tw, (y * height) / th);
            textureScale = Vector2(width / tw, height / th);
            
            textureOffset = textureOffset*textureScaleModifier;
            textureOffset += textureOffsetModifier;
            textureScale = textureScale * textureScaleModifier;
        }
        else
        {
            // Render from an obtained ZwopSprite (from within the ZwopSpriteSheet)
            
            if (animation && animation->firstSprite)
            {
                int zIndex = floor(animation->frame) + animation->firstSprite->GetSpriteIndex();
                ZwopSprite *zs = zSheet->GetSpriteByIndex( zIndex );
                
                if (zs){
                    Vector2 sourceSize = zs->GetSourceSize();
                    Vector2 baseSize = Vector2(width,height);
                    
                    AdjustForZwopSprite(zs);
                    
                    if (sourceSize != baseSize)
                    {
                        // We have an animation frame where the size of the sprite changes.
                        trimScale = trimScale * (sourceSize / baseSize);
                        
                        // Render offset TODO: HOTSPOT
                        renderOffset = (sourceSize - baseSize) * 0.5;
                    }
                    else
                        renderOffset = Vector2::zero;
                }
            }
        }

		Sprite::Render(entity);
	}

	Anim* SpriteAnimation::GetAnim(const std::string &name)
	{
		for (std::list<Anim>::iterator i = animations.begin(); i != animations.end(); ++i)
		{
			if(i->name == name)
			{
				return &(*i);
			}
		}
		Debug::Log("Warning: Could not find animation named: " + name);
		return NULL;
	}


}