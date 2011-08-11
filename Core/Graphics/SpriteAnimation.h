#include "Sprite.h"
#include <list>

#pragma once

namespace Monocle
{
    class ZwopSpriteSheet;
    class ZwopSprite;

	class Anim
	{
	public:
		Anim(const std::string &name, int start, int end, float speed, bool loop=true);
        Anim(const std::string &name, ZwopSprite *firstSprite, int numFrames, float speed, bool loop=true);
		Anim();
		bool isPlaying;
		const std::string name;
		float frame;
		float speed;
		int start;
		int end;
        bool loop;
        
        ZwopSprite *firstSprite;
	};

	class SpriteAnimation : public Sprite
	{
	public:
		SpriteAnimation(const std::string &filename, FilterType filter, float width, float height);
        
        // If the sheet ITSELF is contained inside of a ZwopSpriteSheet
        SpriteAnimation(ZwopSprite *zs, FilterType filter, float width, float height);
        
        // If the sheet is the animation table
        SpriteAnimation(ZwopSpriteSheet *zSheet, FilterType filter, float width, float height);
        
        // Adding normally
		void Add(const std::string &name, int start, int end, float speed, bool loop = true);
        
        // Adding an animation from consecutive sprites in the sprite sheet
        void Add(const std::string &name, std::string startSprite, int numFrames, float speed, bool loop = true);
        
		void Play(const std::string &name);
		void Stop(const std::string &name);
		Anim* GetAnim(const std::string &name);
		void Update();
		void Render(Entity *entity);

		std::list<Anim> animations;
		Anim* animation;
        
        ZwopSpriteSheet *zSheet;
	};
}