#include "Sprite.h"
#include <list>

#pragma once

namespace Monocle
{

	class Anim
	{
	public:
		Anim(const std::string &name, int start, int end, float speed);
		Anim();
		bool isPlaying;
		const std::string name;
		float frame;
		float speed;
		int start;
		int end;
	};

	class SpriteAnimation : public Sprite
	{
	public:
		SpriteAnimation(const std::string &filename, FilterType filter, float width, float height);
		void Add(const std::string &name, int start, int end, float speed);
		void Play(const std::string &name);
		void Stop(const std::string &name);
		Anim* GetAnim(const std::string &name);
		void Update();
		void Render(Entity *entity);

		std::list<Anim> animations;
		Anim* animation;
	};
}