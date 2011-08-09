#include "SpriteAnimation.h"
#include "Sprite.h"
#include "../MonocleToolkit.h"
#include "../Debug.h"

namespace Monocle
{

	Anim::Anim(const std::string &name, int start, int end, float speed)
		: name(name), start(start), end(end), speed(speed), frame(start), isPlaying(false)
	{

	}

	SpriteAnimation::SpriteAnimation(const std::string &filename, FilterType filter, float width, float height)
		: Sprite(filename, filter, width, height)
	{

	}
    
    SpriteAnimation::SpriteAnimation(ZwopSprite *zs, FilterType filter, float width, float height)
    : Sprite(zs, filter, width, height)
	{
        
	}

	void SpriteAnimation::Add(const std::string &name, int start, int end, float speed)
	{
		Anim a = Anim(name, start, end, speed);
		animations.push_back(a);
	}

	void SpriteAnimation::Play(const std::string &name)
	{
		animation = GetAnim(name);
		if (animation)
		{
			animation->isPlaying = true;
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
				if(animation->frame > animation->end + 1) { animation->frame = animation->start; }
			}
		}
	}

	void SpriteAnimation::Render(Entity *entity)
	{
		int x = 0;
		int y = 0;
        
        float tw = texture->width*textureScaleModifier.x;
        float th = texture->height*textureScaleModifier.y;

		if (animation)
		{
			x = (int) animation->frame % (int) (tw / width);
			y = (int) animation->frame / (tw / width);
		}

		textureOffset = Vector2((x * width) / tw, (y * height) / th);
		textureScale = Vector2(width / tw, height / th);
        
        textureOffset = textureOffset*textureScaleModifier;
        textureOffset += textureOffsetModifier;
        textureScale = textureScale * textureScaleModifier;

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