#include "Puppet.h"

#include <TinyXML/tinyxml.h>
#include <Assets.h>
#include <Graphics/Sprite.h>
#include <Monocle.h>
#include <TextureAtlas.h>

#include <sstream>

namespace Monocle
{
	
	/// PART
	Part::Part(int id, const std::string &name, const std::string &imageFilename)
		: Entity(), id(id), name(name), sprite(NULL), puppet(NULL)
	{
		sprite = new Sprite(imageFilename);
		SetGraphic(sprite);
	}

	Part::Part()
		: Entity(), id(-1), sprite(NULL), puppet(NULL)
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

	std::string Part::GetName()
	{
		return this->name;
	}

	bool Part::IsID(int id)
	{
		return this->id == id;
	}

	Sprite *Part::GetSprite()
	{
		return sprite;
	}

	void Part::SetPuppet(Puppet *puppet)
	{
		this->puppet = puppet;
	}

	void Part::Save(FileNode *fileNode)
	{
		//Entity::Save(fileNode);
		fileNode->Write("id", id);
		fileNode->Write("name", name);
		
		fileNode->Write("atlasEntry", atlasEntry);

		if (sprite)
		{
			if (atlasEntry == "")
			{
				fileNode->Write("image", sprite->texture->GetName());
				fileNode->Write("width", sprite->width);
				fileNode->Write("height", sprite->height);
			}
			
			if (sprite->position != Vector2::zero)
				fileNode->Write("offset", sprite->position);
		}

		int layer = GetLayer();
		if (layer != 0)
			fileNode->Write("layer", layer);

		//std::ostringstream os;
		//os << sprite->textureOffset.x << " " << sprite->textureOffset.y << " " << sprite->textureScale.x << " " << sprite->textureScale.y;
		//os.str();
	}

	void Part::Load(FileNode *fileNode)
	{
		//Entity::Load(fileNode);

		fileNode->Read("id", id);
		fileNode->Read("name", name);

		sprite = NULL;
		SetGraphic(NULL);

		TextureAtlas *textureAtlas = puppet->GetTextureAtlas();
		if (textureAtlas)
		{
			TextureAtlasEntry *textureAtlasEntry = NULL;
			fileNode->Read("atlasEntry", atlasEntry);
			if (atlasEntry != "")
			{
				textureAtlasEntry = textureAtlas->GetEntryByName(atlasEntry);
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
			if (atlasEntry == "")
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
			}

			fileNode->Read("offset", sprite->position);
		}

		int layer = 0;
		fileNode->Read("layer", layer);
		SetLayer(layer);

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
		: part(NULL), puppet(NULL)
	{
	}
    
    PartKeyFrames::PartKeyFrames(Part *part)
        : part(part), puppet(NULL)
    {
    }

	std::list<KeyFrame> *PartKeyFrames::GetKeyFrames()
	{
		return &keyFrames;
	}

	void PartKeyFrames::SetPuppet(Puppet *puppet)
	{
		this->puppet = puppet;
	}

	void PartKeyFrames::GetKeyFrameForTime(float time, KeyFrame **prev, KeyFrame **next)
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

	void PartKeyFrames::Save(FileNode *fileNode)
	{
		fileNode->Write("id", part->GetID());
	}

	void PartKeyFrames::Load(FileNode *fileNode)
	{
		int id = -1;
		fileNode->Read("id", id);
		SetPart(puppet->GetPartByID(id));
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
    
    void Animation::ApplyTimeChange(bool loop)
    {
		if (currentTime >= duration)
		{
			if (!loop)
				currentTime = duration;
			else
				currentTime -= int(currentTime/duration) * duration;
		} 
		else if (currentTime < 0)
		{
			if (!loop)
				currentTime = 0;
			else
				currentTime += (int(-1*currentTime/duration)+1) * duration;
		}

		for (std::list<PartKeyFrames>::iterator i = partKeyFrames.begin(); i != partKeyFrames.end(); ++i)
		{
			PartKeyFrames *currentPartKeyFrames = &(*i);
			if (currentPartKeyFrames)
			{
				KeyFrame *prev=NULL, *next=NULL;
				currentPartKeyFrames->GetKeyFrameForTime(currentTime, &prev, &next);
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
    
    void Animation::AdjustCurrentTime(float timeOffset, bool loop)
    {
        currentTime += timeOffset;
        ApplyTimeChange(loop);
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
		: isPlaying(false), isPaused(false), textureAtlas(NULL), currentAnimation(NULL)
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

	void Puppet::Save()
	{
		if (entity)
		{
			// save to filename
			TiXmlDocument xmlDoc;

			/// TextureAtlas
			if (textureAtlas)
			{
				textureAtlas->Save(&xmlDoc);
			}

			/// Parts
			//TiXmlElement *xmlParts = xmlDoc.FirstChildElement("Parts");
			TiXmlElement xmlParts("Parts");
			for (std::list<Part*>::iterator i = parts.begin(); i != parts.end(); ++i)
			{
				if ((*i)->GetParent() == entity)
				{
					printf("calling SaveParts... top level...\n");
					SavePart(&xmlParts, (*i));
				}
			}
			xmlDoc.InsertEndChild(xmlParts);


			/// Animations
			TiXmlElement xmlAnimations("Animations");
			{
				/// Animation
				for (std::list<Animation>::iterator i = animations.begin(); i != animations.end(); ++i)
				{
					TiXmlElement xmlAnimation("Animation");

					Animation *animation = &(*i);
                
					XMLFileNode xmlFileNodeKeyFrameAnim(&xmlAnimation);
					animation->Save(&xmlFileNodeKeyFrameAnim);

					/// PartKeyFrames
					for (std::list<Part*>::iterator j = parts.begin(); j != parts.end(); ++j)
					{
						PartKeyFrames *partKeyFrames = animation->GetPartKeyFrames(*j);
						if (partKeyFrames)
						{
							TiXmlElement xmlPartKeyFrames("PartKeyFrames");
							XMLFileNode xmlFileNodePartKeyFrames(&xmlPartKeyFrames);

							partKeyFrames->Save(&xmlFileNodePartKeyFrames);

							/// KeyFrame
					
							std::list<KeyFrame> *keyFrames = partKeyFrames->GetKeyFrames();
							for (std::list<KeyFrame>::iterator k = keyFrames->begin(); k != keyFrames->end(); ++k)
							{
								KeyFrame *keyFrame = &(*k);

								TiXmlElement xmlKeyFrame("KeyFrame");
								XMLFileNode xmlFileNodeKeyFrame(&xmlKeyFrame);

								keyFrame->Save(&xmlFileNodeKeyFrame);

								xmlPartKeyFrames.InsertEndChild(xmlKeyFrame);
							}

							xmlAnimation.InsertEndChild(xmlPartKeyFrames);
						}
					}

					xmlAnimations.InsertEndChild(xmlAnimation);
				}
			}
			xmlDoc.InsertEndChild(xmlAnimations);

			xmlDoc.SaveFile(Assets::GetContentPath() + filename);
		}
	}

	void Puppet::Load(const std::string &filename, Entity *entity)
	{
		this->entity = entity;
		this->filename = filename;
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
				LoadParts(xmlParts, NULL);
			}

			/// Animations
			TiXmlElement *xmlAnimations = xmlDoc.FirstChildElement("Animations");
			if (xmlAnimations)
			{
				/// Animation
				TiXmlElement *xmlAnimation = xmlAnimations->FirstChildElement("Animation");
				while (xmlAnimation)
				{
					Animation animation;
                    XMLFileNode xmlFileNodeKeyFrameAnim(xmlAnimation);
					animation.Load(&xmlFileNodeKeyFrameAnim);

					/// PartKeyFrames
					TiXmlElement *xmlPartKeyFrames = xmlAnimation->FirstChildElement("PartKeyFrames");
					while (xmlPartKeyFrames)
					{
						PartKeyFrames partKeyFrames;
						partKeyFrames.SetPuppet(this);
                        XMLFileNode xmlFileNodeKeyFramePart(xmlPartKeyFrames);
						partKeyFrames.Load(&xmlFileNodeKeyFramePart);

						/// KeyFrame
						TiXmlElement *xmlKeyFrame = xmlPartKeyFrames->FirstChildElement("KeyFrame");
						while (xmlKeyFrame)
						{
							KeyFrame keyFrame;
                            XMLFileNode xmlFileNodeKeyFrame(xmlKeyFrame);
							keyFrame.Load(&xmlFileNodeKeyFrame);
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

	void Puppet::SavePart(TiXmlElement *parentElement, Part *part)
	{
		printf("saving part [%s]\n", part->GetName().c_str());

		TiXmlElement xmlPart("Part");
		XMLFileNode xmlFileNode(&xmlPart);

		if (part)
		{
			part->Save(&xmlFileNode);
		}

		for (std::list<Part*>::iterator i = part->parts.begin(); i != part->parts.end(); ++i)
		{
			Part *iterPart = (*i);
			if (iterPart)
			{
				printf ("   calling save on subpart [%s]\n", iterPart->GetName().c_str());

				SavePart(&xmlPart, iterPart);
			}
		}
		parentElement->InsertEndChild(xmlPart);
	}

	void Puppet::LoadParts(TiXmlElement *element, Part *intoPart)
	{
		TiXmlElement *xmlPart = element->FirstChildElement("Part");
		while (xmlPart)
		{
			XMLFileNode xmlFileNode(xmlPart);

			Part *part = new Part();
			part->SetPuppet(this);
			part->Load(&xmlFileNode);
			
			LoadParts(xmlPart, part);

			printf("loaded part [%s]\n", part->GetName().c_str());

			if (intoPart)
			{
				intoPart->parts.push_back(part);
				part->SetParent(intoPart);

				printf("   into part [%s]\n", intoPart->GetName().c_str());
			}
			else
			{
				part->SetParent(entity);
			}

			parts.push_back(part);

			Game::GetScene()->Add(part);
			
			//intoEntity->Add(part);

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
