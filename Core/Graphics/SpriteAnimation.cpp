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

		if (animation)
		{
			x = (int) animation->frame % (int) (texture->width / width);
			y = (int) animation->frame / (texture->width / width);
		}

		textureOffset = Vector2((x * width) / texture->width, (y * height) / texture->height);
		textureScale = Vector2(width / texture->width, height / texture->height);

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