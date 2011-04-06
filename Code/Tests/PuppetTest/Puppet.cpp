#include "Puppet.h"

#include "../../XML/tinyxml.h"
#include "../../Assets.h"
#include "../../Graphics/Sprite.h"
#include "../../Monocle.h"

namespace Monocle
{
	Part::Part()
		: Entity()
	{
	}

	KeyFrame::KeyFrame()
		: Transform()
	{
	}
	
	inline float KeyFrame::GetTime()
	{
		return time;
	}

	void KeyFrame::SetTime(float time)
	{
		this->time = time;
	}

	PartKeyFrames::PartKeyFrames()
		: part(NULL)
	{
	}

	void PartKeyFrames::GetKeyframeForTime(float time, KeyFrame **prev, KeyFrame **next)
	{
		// go through all the keyframes, check time
		KeyFrame *lastKeyFrame = NULL;
		for (std::list<KeyFrame>::iterator i = keyFrames.begin(); i != keyFrames.end(); ++i)
		{
			KeyFrame *keyFrame = &(*i);
			if (time > keyFrame->GetTime())
			{
				*prev = keyFrame;
			}
			else
			{
				*next = keyFrame;
				return;
			}
			lastKeyFrame = keyFrame;
		}
	}

	void PartKeyFrames::SetPart(Part *part)
	{
		this->part = part;
	}

	inline Part* PartKeyFrames::GetPart()
	{
		return part;
	}

	void PartKeyFrames::AddKeyFrame(const KeyFrame &keyFrame)
	{
		keyFrames.push_back(keyFrame);
	}


	Animation::Animation()
		: time(0.0f), length(0.0)
	{
	}

	void Animation::Update()
	{
		time += Monocle::deltaTime;


		if (time > length)
		{
			time = 0.0f;
		}

		for (std::list<PartKeyFrames>::iterator i = partKeyFrames.begin(); i != partKeyFrames.end(); ++i)
		{
			PartKeyFrames *currentPartKeyFrames = &(*i);
			if (currentPartKeyFrames)
			{
				KeyFrame *prev=NULL, *next=NULL;
				currentPartKeyFrames->GetKeyframeForTime(time, &prev, &next);
				if (prev && next)
				{
					float diff = next->GetTime() - prev->GetTime();
					float p = (time - prev->GetTime()) / diff;

					printf("LerpTransform %f\n", p);
					// adjust p by ease
					currentPartKeyFrames->GetPart()->LerpTransform(prev, next, p);
				}
			}
		}
	}

	std::string Animation::GetName()
	{
		return name;
	}

	bool Animation::IsName(const std::string &name)
	{
		return (this->name == name);
	}

	void Animation::AddPartKeyFrames(const PartKeyFrames &partKeyFrames)
	{
		this->partKeyFrames.push_back(partKeyFrames);
	}
	
	void Animation::SetLength(float length)
	{
		this->length = length;
	}

	Puppet::Puppet()
		: isPlaying(false), isPaused(false)
	{
	}

	void Puppet::Load(const std::string &filename, Entity *entity)
	{
		Part *part1 = new Part();
		part1->SetGraphic(new Sprite("graphics/whatever.png"));
		entity->Add(part1);

		Part *part2 = new Part();
		part2->SetGraphic(new Sprite("graphics/whatever.png"));
		part2->position = Vector2::down * 100;
		part1->Add(part2);

		Animation animation;
		animation.name = "run";

		PartKeyFrames partKeyFrames;
		partKeyFrames.SetPart(part1);

		KeyFrame keyFrame;
		keyFrame.position = Vector2::zero;
		keyFrame.SetTime(0.0f);
		partKeyFrames.AddKeyFrame(keyFrame);

		keyFrame.position = Vector2::right * 20;
		keyFrame.SetTime(4.0f);
		partKeyFrames.AddKeyFrame(keyFrame);

		animation.AddPartKeyFrames(partKeyFrames);
		animation.SetLength(4.0f);

		animations.push_back(animation);

		Play("run");
	}

	void Puppet::Play(const std::string &animationName, bool isLooping)
	{
		this->isPlaying = true;
		this->isLooping = isLooping;

		currentAnimation = GetAnimationByName(animationName);
	}

	void Puppet::Update()
	{
		if (currentAnimation)
		{
			if (isPlaying)
			{
				currentAnimation->Update();
			}
		}
	}

	void Puppet::Stop()
	{
		isPlaying = false;
		isPaused = false;
	}

	void Puppet::Pause()
	{
		isPaused = true;
	}

	void Puppet::Resume()
	{
		isPaused = false;
	}

	bool Puppet::IsPlaying()
	{
		return isPlaying;
	}

	bool Puppet::IsPaused()
	{
		return isPaused;
	}

	Animation *Puppet::GetAnimationByName(const std::string &name)
	{
		for (std::list<Animation>::iterator i = animations.begin(); i != animations.end(); ++i)
		{
			return &(*i);
		}
		return NULL;
	}
}