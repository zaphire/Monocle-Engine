#pragma once

#include <list>
#include "../../Entity.h"
#include "../../Tween.h"
#include "../../Graphics/Sprite.h"

class TiXmlElement;

namespace Monocle
{
    /*
    class AtlasEntry
    {
    public:
        void Save(FileNode *fileNode);
        void Load(FileNode *fileNode);
        
        Vector2 offset;
        Vector2 scale;
    };
    */
	
	class Puppet;
    
	class Part : public Entity
	{
	public:
		Part(int id, const std::string &name, const std::string &imageFilename);
		Part();
		int GetID();
		bool IsName(const std::string &name);
		bool IsID(int id);

		void Save(FileNode *fileNode, Puppet *puppet);
		void Load(FileNode *fileNode, Puppet *puppet);

	private:
		friend class Puppet;
		int id;
		std::string name;
		Sprite *sprite;
	};

	class KeyFrame : public Transform
	{
	public:
        KeyFrame(float time, const Entity &entity);
		KeyFrame();
        float GetTime() const;
		void SetTime(float time);

		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);

		EaseType easeType;

	private:
		float time;
	};

	class PartKeyFrames
	{
	public:
        PartKeyFrames(Part *part);
		PartKeyFrames();
		void AddKeyFrame(const KeyFrame &keyFrame);
		void SetPart(Part *part);
		Part *GetPart();
		void GetKeyframeForTime(float time, KeyFrame **prev, KeyFrame **next);
		KeyFrame *GetLastKeyFrame();
        void InsertKeyFrame(const KeyFrame &keyFrame);
        
        std::list<KeyFrame> keyFrames;
		
	private:
		Part *part;
	
	};

	class Animation
	{
	public:
		Animation();
		void Update();
        void ApplyTimeChange();
		void SetCurrentTime(float time);
        void AdjustCurrentTime(float timeOffset);
        
		std::string GetName();
		bool IsName(const std::string &name);
		void AddPartKeyFrames(const PartKeyFrames &partKeyFrames);
        void SetPartKeyFrame(Part *part, const KeyFrame &keyFrame);
		void SetDuration(float length);
        float GetDuration();
        float GetCurrentTime();
        PartKeyFrames *GetPartKeyFrames(Part *part);
        
		void RefreshDuration();

		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);
        
	private:
		friend class Puppet;
		float currentTime;
		float duration;
		std::string name;
		std::list<PartKeyFrames> partKeyFrames;
	};

	///TODO: Add layers, so multiple animations can be blended at once
	class AnimationLayer
	{
	public:
	};

	class TextureAtlas;
	
	class Puppet
	{
	public:
		Puppet();
		~Puppet();
		void Load(const std::string &filename, Entity *entity);
		void Play(const std::string &animationName, bool isLooping=true);
		void Stop();
		void Pause();
		void Resume();

		void TogglePause();

		void Update();

		bool IsPlaying();
		bool IsPaused();

		Part *GetPartByName(const std::string &name);
		Part *GetPartByID(int id);

		Animation *GetCurrentAnimation();
		TextureAtlas *GetTextureAtlas();

		void AdjustTime(float time);
		
	private:		
		Animation *GetAnimationByName(const std::string &animName);
		bool isPlaying;
		bool isPaused;
		bool isLooping;
	
		Animation *currentAnimation;
	
		std::list<Animation> animations;
		std::list<Part*> parts;
		
		TextureAtlas *textureAtlas;

		void LoadParts(TiXmlElement *element, Entity *intoEntity);

	private:
		
	};

}