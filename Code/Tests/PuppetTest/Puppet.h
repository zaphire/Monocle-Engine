#pragma once

#include <list>
#include "../../Entity.h"
#include "../../Tween.h"
#include "../../Graphics/Sprite.h"

class TiXmlElement;

namespace Monocle
{
	class Part : public Entity
	{
	public:
		Part(int id, const std::string &name, const std::string &imageFilename);
		Part();
		int GetID();
		bool IsName(const std::string &name);

		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);

	private:
		friend class Puppet;
		int id;
		std::string name;
		Sprite *sprite;
	};

	class KeyFrame : public Transform
	{
	public:
		KeyFrame();
		float GetTime();
		void SetTime(float time);

		EaseType easeType;

	private:
		float time;
	};

	class PartKeyFrames
	{
	public:
		PartKeyFrames();
		void AddKeyFrame(const KeyFrame &keyFrame);
		void SetPart(Part *part);
		Part *GetPart();
		void GetKeyframeForTime(float time, KeyFrame **prev, KeyFrame **next);
		
	private:
		Part *part;
	
		std::list<KeyFrame> keyFrames;
	};

	class Animation
	{
	public:
		Animation();
		void Update();
		std::string GetName();
		bool IsName(const std::string &name);
		void AddPartKeyFrames(const PartKeyFrames &partKeyFrames);
		void SetLength(float length);

	private:
		friend class Puppet;
		float time;
		float length;
		std::string name;
		std::list<PartKeyFrames> partKeyFrames;
	};

	///TODO: Add layers, so multiple animations can be blended at once
	class AnimationLayer
	{
	public:
	};

	class Puppet
	{
	public:
		Puppet();
		void Load(const std::string &filename, Entity *entity);
		void Play(const std::string &animationName, bool isLooping=true);
		void Stop();
		void Pause();
		void Resume();

		void Update();

		bool IsPlaying();
		bool IsPaused();

		Part *GetPartByName(const std::string &name);

		void DebugSetup();
		
	private:
		Animation *GetAnimationByName(const std::string &animName);
		bool isPlaying;
		bool isPaused;
		bool isLooping;
	
		Animation *currentAnimation;
	
		std::list<Animation> animations;
		std::list<Part*> parts;

		void LoadParts(TiXmlElement *element, Entity *intoEntity);

	private:
		
	};

}