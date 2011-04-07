#include "Puppet.h"

#include "../../XML/tinyxml.h"
#include "../../Assets.h"
#include "../../Graphics/Sprite.h"
#include "../../Monocle.h"

#include <sstream>

namespace Monocle
{
    /*
    void AtlasEntry::Save(FileNode *fileNode)
    {
        std::ostringstream os;
        os << offset.x << " " << offset.y << " " << scale.x << " " << scale.y << " ";
        fileNode->Write("atlas", os.str());
    }
    
    void AtlasEntry::Load(FileNode *fileNode)
    {
        std::string str;
        
        fileNode->Read("atlasPixels", str);
        if (str != "")
        {
            std::istringstream is(str);
            float x, y, w, h, mw, mh;
            is >> x >> y >> w >> h >> mw >> mh;
            offset = Vector2(x/mw, y/mh);
            scale = Vector2(w/mw, h/mh);
        }
        else
        {
            fileNode->Read("atlas", str);
            if (str != "")
            {
                std::istringstream is(str);
                is >> offset.x >> offset.y >> scale.x >> scale.y;
            }
        }
    }
    */
	
	class TextureAtlas;
	
	class TextureAtlasEntry
	{
	public:
		TextureAtlasEntry();
		TextureAtlasEntry(TextureAtlas *textureAtlas);
		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);

		Vector2 GetTextureOffset();
		Vector2 GetTextureScale();
		int GetWidth();
		int GetHeight();
		
	private:
		friend class TextureAtlas;
		std::string name;
		int x, y, width, height;
		TextureAtlas *textureAtlas;
	};
	
	class TextureAtlas
	{
	public:
		TextureAtlas();
		~TextureAtlas();
		TextureAtlasEntry* GetEntryByName(const std::string &name);
		void Load(TiXmlElement *element);
		std::string GetImageName();
		
	private:
		friend class TextureAtlasEntry;
		std::string image;
		int width, height;
		
	private:
		std::map<std::string, TextureAtlasEntry*> entries;
	};
	
	TextureAtlasEntry::TextureAtlasEntry()
	: textureAtlas(NULL), x(0), y(0), width(0), height(0)
	{
	}
	
	TextureAtlasEntry::TextureAtlasEntry(TextureAtlas *textureAtlas)
		: textureAtlas(textureAtlas), x(0), y(0), width(0), height(0)
	{
	}
	
	int TextureAtlasEntry::GetWidth()
	{
		return width;
	}
	
	int TextureAtlasEntry::GetHeight()
	{
		return height;
	}
	
	Vector2 TextureAtlasEntry::GetTextureOffset()
	{
		return Vector2(x/float(textureAtlas->width), y/float(textureAtlas->height));
	}
	
	Vector2 TextureAtlasEntry::GetTextureScale()
	{
		return Vector2(width/float(textureAtlas->width), height/float(textureAtlas->height));
	}
	
	void TextureAtlasEntry::Save(FileNode *fileNode)
	{
		fileNode->Write("name", name);
		std::ostringstream os;
		os << x << " " << y << " " << width << " " << height;
		fileNode->Write("coords", os.str());
	}
	
	void TextureAtlasEntry::Load(FileNode *fileNode)
	{
		fileNode->Read("name", name);
		std::string read;
		fileNode->Read("coords", read);
		std::istringstream is(read);
		is >> x >> y >> width >> height;
	}
	
	TextureAtlas::TextureAtlas()
		: width(0), height(0)
	{
	}
	
	TextureAtlas::~TextureAtlas()
	{
		for (std::map<std::string, TextureAtlasEntry*>::iterator i = entries.begin(); i != entries.end(); ++i)
		{
			delete (*i).second;
		}
		entries.clear();
	}
	
	TextureAtlasEntry* TextureAtlas::GetEntryByName(const std::string &name)
	{
		return entries[name];
	}
	
	void TextureAtlas::Load(TiXmlElement *element)
	{
		XMLFileNode xmlFileNode(element);
		xmlFileNode.Read("image", image);
		xmlFileNode.Read("width", width);
		xmlFileNode.Read("height", height);
		
		TiXmlElement *xmlAtlasEntry = element->FirstChildElement("TextureAtlasEntry");
		while (xmlAtlasEntry)
		{
			XMLFileNode xmlFileNode(xmlAtlasEntry);
			
			TextureAtlasEntry *textureAtlasEntry = new TextureAtlasEntry(this);
			textureAtlasEntry->Load(&xmlFileNode);
			if (textureAtlasEntry->name != "")
				entries[textureAtlasEntry->name] = textureAtlasEntry;
			
			xmlAtlasEntry = xmlAtlasEntry->NextSiblingElement("TextureAtlasEntry");
		}
	}
	
	std::string TextureAtlas::GetImageName()
	{
		return image;
	}

	Part::Part(int id, const std::string &name, const std::string &imageFilename)
		: Entity(), id(id), name(name), sprite(NULL)
	{
		sprite = new Sprite(imageFilename);
		SetGraphic(sprite);
	}

	Part::Part()
		: Entity(), id(-1), sprite(NULL)
	{

	}

	int Part::GetID()
	{
		return id;
	}

	bool Part::IsName(const std::string &name)
	{
		return this->name == name;
	}

	bool Part::IsID(int id)
	{
		return this->id == id;
	}

	void Part::Save(FileNode *fileNode, Puppet *puppet)
	{
		fileNode->Write("id", id);
		fileNode->Write("name", name);
		fileNode->Write("image", sprite->texture->GetName());

		//std::ostringstream os;
		//os << sprite->textureOffset.x << " " << sprite->textureOffset.y << " " << sprite->textureScale.x << " " << sprite->textureScale.y;
		//os.str();
	}

	void Part::Load(FileNode *fileNode, Puppet *puppet)
	{
		fileNode->Read("id", id);
		fileNode->Read("name", name);

		sprite = NULL;
		SetGraphic(NULL);

		TextureAtlas *textureAtlas = puppet->GetTextureAtlas();
		if (textureAtlas)
		{
			TextureAtlasEntry *textureAtlasEntry = NULL;
			std::string atlas;
			fileNode->Read("atlas", atlas);
			if (atlas != "")
			{
				textureAtlasEntry = textureAtlas->GetEntryByName(atlas);
				if (textureAtlasEntry)
				{
					sprite = new Sprite(textureAtlas->GetImageName());
					SetGraphic(sprite);
					sprite->textureOffset = textureAtlasEntry->GetTextureOffset();
					sprite->textureScale = textureAtlasEntry->GetTextureScale();
					printf("textureOffset: (%f, %f) textureScale: (%f, %f)\n", sprite->textureOffset.x, sprite->textureOffset.y, sprite->textureScale.x, sprite->textureScale.y);
					sprite->width = textureAtlasEntry->GetWidth();
					sprite->height = textureAtlasEntry->GetHeight();
				}
			}
		}

		if (sprite == NULL)
		{
			std::string image;
			fileNode->Read("image", image);

			sprite = new Sprite(image);
			SetGraphic(sprite);
		}
		
		if (sprite != NULL)
		{
			int width=-1, height=-1;
			fileNode->Read("width", width);
			fileNode->Read("height", height);

			if (width != -1)
			{
				sprite->width = width;
			}

			if (height != -1)
			{
				sprite->height = height;
			}

			Vector2 offset;
			std::string offsetString;
			fileNode->Read("offset", offsetString);

			if (offsetString != "")
			{
				std::istringstream is(offsetString);
				is >> offset.x >> offset.y;
				printf("offset (%f, %f)\n", offset.x, offset.y);
			}

			sprite->position = offset;
		}

//		if (sprite)
//		{
//			std::string atlas;
//			fileNode->Read("atlas", atlas);
//			if (atlas != "")
//			{
//				int x, y, width, height, maxWidth=-1, maxHeight=-1;
//				std::istringstream is(atlas);
//				is >> x >> y >> width >> height >> maxWidth >> maxHeight;
//
//				if (maxWidth == -1)
//				maxWidth = width;
//				if (maxHeight == -1)
//				maxHeight = height;
//
//				sprite->textureOffset = Vector2(x/float(maxWidth), y/float(maxHeight));
//				sprite->textureScale = Vector2(width/float(maxWidth), height/float(maxHeight));
//
//				sprite->width = width;
//				sprite->height = height;
//			}
//		}
	}

    KeyFrame::KeyFrame(float time, const Entity &entity)
        : Transform(entity), time(time)
    {
    }
    
	KeyFrame::KeyFrame()
		: Transform(), easeType(EASE_LINEAR)
	{
	}

   float KeyFrame::GetTime() const
	{
		return time;
	}

	void KeyFrame::SetTime(float time)
	{
		this->time = time;
	}

	void KeyFrame::Save(FileNode *fileNode)
	{
		Transform::Save(fileNode);

		fileNode->Write("time", time);
	}

	void KeyFrame::Load(FileNode *fileNode)
	{
		Transform::Load(fileNode);

		fileNode->Read("time", time);
	}

    
	PartKeyFrames::PartKeyFrames()
		: part(NULL)
	{
	}
    
    PartKeyFrames::PartKeyFrames(Part *part)
        : part(part)
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

	KeyFrame *PartKeyFrames::GetLastKeyFrame()
	{
		if (keyFrames.size() == 0)
			return NULL;
		return &keyFrames.back();
	}
    
    void PartKeyFrames::InsertKeyFrame(const KeyFrame &keyFrame)
    {
		for (std::list<KeyFrame>::iterator i = keyFrames.begin(); i != keyFrames.end(); ++i)
		{
			if (keyFrame.GetTime() == (*i).GetTime())
			{
				(*i) = keyFrame;
				return;
			}
			else if ((*i).GetTime() > keyFrame.GetTime())
			{
				//inserts BEFORE iterator
				keyFrames.insert(i, keyFrame);
				return;
			}
		}
		keyFrames.push_back(keyFrame);
    }


	Animation::Animation()
		: currentTime(0.0f), duration(0.0f)
	{
	}

	void Animation::Update()
	{
		currentTime += Monocle::deltaTime;

        ApplyTimeChange();
	}
    
    void Animation::ApplyTimeChange()
    {
        if (currentTime > duration)
		{
			currentTime -= int(currentTime/duration) * duration;
		} 
        if (currentTime < 0)
        {
            currentTime += (int(-1*currentTime/duration)+1) * duration;
        }
        
		for (std::list<PartKeyFrames>::iterator i = partKeyFrames.begin(); i != partKeyFrames.end(); ++i)
		{
			PartKeyFrames *currentPartKeyFrames = &(*i);
			if (currentPartKeyFrames)
			{
				KeyFrame *prev=NULL, *next=NULL;
				currentPartKeyFrames->GetKeyframeForTime(currentTime, &prev, &next);
				if (prev && !next)
				{
					currentPartKeyFrames->GetPart()->LerpTransform(prev, prev, 1.0f);
				}
				else if (prev && next)
				{
					float diff = next->GetTime() - prev->GetTime();
					float p = (currentTime - prev->GetTime()) / diff;
                    
					//printf("LerpTransform %f\n", p);
					// adjust p by ease
					currentPartKeyFrames->GetPart()->LerpTransform(prev, next, Tween::Ease(p, EASE_INOUTSIN));//prev->easeType));
				}
			}
		}
    }

	std::string Animation::GetName()
	{
		return name;
	}
    
    float Animation::GetDuration()
    {
        return duration;
    }

    float Animation::GetCurrentTime()
    {
        return currentTime;
    }
    
    PartKeyFrames *Animation::GetPartKeyFrames(Part *part)
    {
        for(std::list<PartKeyFrames>::iterator i = partKeyFrames.begin(); i != partKeyFrames.end(); ++i)
        {
            if ((*i).GetPart() == part)
            {
                return &(*i);
            }
        }
        return NULL;
    }
    
    void Animation::SetPartKeyFrame(Part *part, const KeyFrame &keyFrame)
    {
        PartKeyFrames *partKeyFrames = GetPartKeyFrames(part);
        if (!partKeyFrames)
        {
            AddPartKeyFrames(PartKeyFrames(part));
            partKeyFrames = GetPartKeyFrames(part);
        }
        if (partKeyFrames)
        {
            partKeyFrames->InsertKeyFrame(keyFrame);
        }
    }

	bool Animation::IsName(const std::string &name)
	{
		return (this->name == name);
	}

	void Animation::AddPartKeyFrames(const PartKeyFrames &partKeyFrames)
	{
		this->partKeyFrames.push_back(partKeyFrames);
	}
	
	void Animation::SetDuration(float duration)
	{
		this->duration = duration;
	}

	void Animation::SetCurrentTime(float currentTime)
	{
		this->currentTime = currentTime;
		ApplyTimeChange();
	}
    
    void Animation::AdjustCurrentTime(float timeOffset)
    {
        currentTime += timeOffset;
        ApplyTimeChange();
    }

	void Animation::RefreshDuration()
	{
		duration = -1.0f;
		for (std::list<PartKeyFrames>::iterator i = partKeyFrames.begin(); i != partKeyFrames.end(); ++i)
		{
			KeyFrame *keyFrame = (*i).GetLastKeyFrame();
			if (keyFrame)
			{
				float t = keyFrame->GetTime();
				if (t > duration)
					duration = t;
			}
		}
	}

	void Animation::Save(FileNode *fileNode)
	{
		fileNode->Write("name", name);
	}

	void Animation::Load(FileNode *fileNode)
	{
		fileNode->Read("name", name);
	}

	Puppet::Puppet()
		: isPlaying(false), isPaused(false), textureAtlas(NULL)
	{
	}
	
	Puppet::~Puppet()
	{
		if (textureAtlas)
		{
			delete textureAtlas;
			textureAtlas = NULL;
		}
	}

	void Puppet::Load(const std::string &filename, Entity *entity)
	{
		animations.clear();
		// delete parts?
		parts.clear();
		
		TiXmlDocument xmlDoc(Assets::GetContentPath() + filename);
		
		if (xmlDoc.LoadFile())
		{
			/// TextureAtlas
			TiXmlElement *xmlTextureAtlas = xmlDoc.FirstChildElement("TextureAtlas");
			if (xmlTextureAtlas)
			{
				textureAtlas = new TextureAtlas();
				textureAtlas->Load(xmlTextureAtlas);
			}
			
			/// Parts
			TiXmlElement *xmlParts = xmlDoc.FirstChildElement("Parts");
			if (xmlParts)
			{
				LoadParts(xmlParts, entity);
			}

			/// Animations
			TiXmlElement *xmlAnimations = xmlDoc.FirstChildElement("Animations");
			if (xmlAnimations)
			{
				/// Animation
				TiXmlElement *xmlAnimation = xmlAnimations->FirstChildElement("Animation");
				while (xmlAnimation)
				{
					XMLFileNode xmlFileNode(xmlAnimation);

					Animation animation;
					animation.Load(&xmlFileNode);

					/// PartKeyFrames
					TiXmlElement *xmlPartKeyFrames = xmlAnimation->FirstChildElement("PartKeyFrames");
					while (xmlPartKeyFrames)
					{
						PartKeyFrames partKeyFrames;
						int id = -1;
						if (xmlPartKeyFrames->Attribute("id"))
							id = atoi(xmlPartKeyFrames->Attribute("id"));
						partKeyFrames.SetPart(GetPartByID(id));

						/// KeyFrame
						TiXmlElement *xmlKeyFrame = xmlPartKeyFrames->FirstChildElement("KeyFrame");
						while (xmlKeyFrame)
						{
							XMLFileNode xmlFileNode2(xmlKeyFrame);

							KeyFrame keyFrame;
							keyFrame.Load(&xmlFileNode2);
							partKeyFrames.AddKeyFrame(keyFrame);

							xmlKeyFrame = xmlKeyFrame->NextSiblingElement("KeyFrame");
						}

						animation.AddPartKeyFrames(partKeyFrames);

						xmlPartKeyFrames = xmlPartKeyFrames->NextSiblingElement("PartKeyFrames");
					}

					animation.RefreshDuration();
					animations.push_back(animation);

					xmlAnimation = xmlAnimation->NextSiblingElement("Animation");
				}
			}
		}
		else
		{
			Debug::Log("Warning: Could not open puppet file: " + Assets::GetContentPath() + filename);
			Debug::Log("         " + std::string(xmlDoc.ErrorDesc()));
			printf("         Row: %d\n", xmlDoc.ErrorRow());
		}
	}

	void Puppet::LoadParts(TiXmlElement *element, Entity *intoEntity)
	{
		TiXmlElement *xmlPart = element->FirstChildElement("Part");
		while (xmlPart)
		{
			XMLFileNode xmlFileNode(xmlPart);

			Part *part = new Part();
			part->Load(&xmlFileNode, this);
			
			LoadParts(xmlPart, part);

			parts.push_back(part);

			/// TODO...?
			intoEntity->Add(part);

			xmlPart = xmlPart->NextSiblingElement("Part");
		}
	}

	void Puppet::Play(const std::string &animationName, bool isLooping)
	{
		currentAnimation = GetAnimationByName(animationName);

		if (currentAnimation)
		{
			this->isPlaying = true;
			this->isLooping = isLooping;
		}

	}

	void Puppet::Update()
	{
		if (currentAnimation)
		{
			if (isPlaying && !isPaused)
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

	void Puppet::TogglePause()
	{
		isPaused = !isPaused;
	}

	bool Puppet::IsPlaying()
	{
		return isPlaying;
	}

	bool Puppet::IsPaused()
	{
		return isPaused;
	}
    
    Animation *Puppet::GetCurrentAnimation()
    {
        return currentAnimation;
    }
	
	TextureAtlas *Puppet::GetTextureAtlas()
	{
		return textureAtlas;
	}

	Animation *Puppet::GetAnimationByName(const std::string &name)
	{
		for (std::list<Animation>::iterator i = animations.begin(); i != animations.end(); ++i)
		{
			return &(*i);
		}
		return NULL;
	}

	Part *Puppet::GetPartByName(const std::string &name)
	{
		for (std::list<Part*>::iterator i = parts.begin(); i != parts.end(); ++i)
		{
			if ((*i)->IsName(name))
			{
				return *i;
			}
		}
		return NULL;
	}

	Part *Puppet::GetPartByID(int id)
	{
		for (std::list<Part*>::iterator i = parts.begin(); i != parts.end(); ++i)
		{
			if ((*i)->IsID(id))
			{
				return *i;
			}
		}
		return NULL;
	}
}
