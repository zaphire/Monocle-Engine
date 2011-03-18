#include "Flash.h"
#include "../../XML/tinyxml.h"
#include <math.h>

namespace Flash
{
	float ReadFloatAttribute(TiXmlElement* elem, const std::string &att)
	{
		const std::string *read = elem->Attribute(att);
		if (read != NULL)
			return atof((*read).c_str());
		return 0.0f;
	}

	float ReadIntAttribute(TiXmlElement* elem, const std::string &att)
	{
		const std::string *read = elem->Attribute(att);
		if (read != NULL)
			return atoi((*read).c_str());
		return 0.0f;
	}

	std::string ReadStringAttribute(TiXmlElement *elem, const std::string &att)
	{
		const std::string *read = elem->Attribute(att);
		if (read != NULL)
			return *read;
		return "";
	}

	Texture* TextureSheet::GetTextureByName(const std::string &name)
	{
		for (std::vector<Texture>::iterator i = textures.begin(); i != textures.end(); ++i)
		{
			Texture *texture = (Texture*)(&(*i)); // <- haHA! take that, Bjarne
			if (texture->name == name)
			{
				return texture;
			}
		}
		return NULL;
	}

	int Animation::GetMaxFrames()
	{
		int max = -1;
		for (int i = 0; i < parts.size(); i++)
		{
			if (max == -1 || parts[i].frames.size() > max)
			{
				max = parts[i].frames.size();
			}
		}
		return max;
	}

	Entity* Part::CreateEntity(TextureSheet &textureSheet)
	{
		Texture *texture = textureSheet.GetTextureByName(name);
		if (texture != NULL)
		{
			entity = new Entity();
			sprite = new Sprite("../../Content/Flash/" + textureSheet.name + "/" + name + ".png");
			sprite->position = (texture->registrationPoint * -1) + Vector2(sprite->width, sprite->height)*0.5f;
			//sprite->position = texture->registrationPoint * -1;
			entity->SetGraphic(sprite);

			printf("size (%d, %d)", (int)sprite->width, (int)sprite->height);

			return entity;
		}
		else
		{
			Debug::Log("Error: Part could not CreateEntity, Part named: " + name + " not found in TextureSheet. Did you remember to load the TextureSheet?");
		}
		return NULL;
	}

	void Part::Update(float f)
	{
		int frame = floor(f);
		if (frame >= 0 && frame < frames.size())
		{
			// lerp!
			if (frame < frames.size()-1)
			{
				float p = f - (float)frame;

				//printf("frame: %d\nf: %f\n p: %f\n", frame, f, p);

				entity->position = frames[frame].pos + (p * (frames[frame+1].pos - frames[frame].pos));

				if (frames[frame].hasScaleData && frames[frame+1].hasScaleData)
				{
					Debug::Log("using scale data");
					Vector2 scale = frames[frame].scale + (p * (frames[frame+1].scale - frames[frame].scale));
					sprite->width = scale.x;
					sprite->height = scale.y;
				}

				sprite->color.a = frames[frame].alpha + (p * (frames[frame+1].alpha - frames[frame].alpha));

				printf("part: %s frame: %d pos(%d, %d) size(%d, %d) a: %f\n", name.c_str(), frame, (int)entity->position.x, (int)entity->position.y, (int)sprite->width, (int)sprite->height, sprite->color.a);
			}
			else
			{
				entity->position = frames[frame].pos;

				if (frames[frame].hasScaleData)
				{
					Debug::Log("using scale data");
					sprite->width = frames[frame].scale.x;
					sprite->height = frames[frame].scale.y;
				}

				sprite->color.a = frames[frame].alpha;
			}
		}
	}

	void TestScene::LoadAnimation(const std::string &filename)
	{
		// load the xml file
		TiXmlDocument xml(filename);
		bool isLoaded = xml.LoadFile();

		if (isLoaded)
		{
			TiXmlElement* eAnimations = xml.FirstChildElement("Animations");
			if (eAnimations)
			{
				TiXmlElement* eAnimation = eAnimations->FirstChildElement("Animation");
				while (eAnimation)
				{
					Animation animation(eAnimation->Attribute("name"), ReadIntAttribute(eAnimation, "frameCount"));
					
					TiXmlElement* ePart = eAnimation->FirstChildElement("Part");
					while (ePart)
					{
						Part part(ePart->Attribute("name"));
						
						TiXmlElement* eFrame = ePart->FirstChildElement("Frame");
						while (eFrame)
						{
							Frame frame;
							frame.pos = Vector2(ReadFloatAttribute(eFrame, "x"), ReadFloatAttribute(eFrame, "y"));
							
							if (eFrame->Attribute("scaleX"))
							{
								frame.hasScaleData = true;
								frame.scale.x = ReadFloatAttribute(eFrame, "scaleX");
							}

							if (eFrame->Attribute("scaleY"))
							{
								frame.scale.y = ReadFloatAttribute(eFrame, "scaleY");
								frame.hasScaleData = true;
							}

							if (eFrame->Attribute("alpha") != NULL)
							{
								frame.alpha = atof(eFrame->Attribute("alpha"));
							}

							part.frames.push_back(frame);

							eFrame = eFrame->NextSiblingElement("Frame");
						}
						
						animation.parts.push_back(part);

						ePart = ePart->NextSiblingElement("Part");
					}

					animations.push_back(animation);

					eAnimation = eAnimation->NextSiblingElement("Animation");
				}
			}
		}
		else
		{
			Debug::Log("Error: could not load animations from: " + filename + " - " + xml.ErrorDesc());
		}
	}

	void TestScene::LoadTextureSheet(const std::string &filename)
	{
		// load the xml file
		TiXmlDocument xml(filename);
		bool isLoaded = xml.LoadFile();

		if (isLoaded)
		{
			TiXmlElement* eTextures = xml.FirstChildElement("Textures");
			if (eTextures)
			{
				TiXmlElement* eTextureSheet = eTextures->FirstChildElement("TextureSheet");
				while (eTextureSheet)
				{
					textureSheet.name = ReadStringAttribute(eTextureSheet, "name");

					TiXmlElement* eTexture = eTextureSheet->FirstChildElement("Texture");
					while (eTexture)
					{
						Texture texture;
						texture.name = eTexture->Attribute("name");
						texture.width = ReadIntAttribute(eTexture, "width");
						texture.height = ReadIntAttribute(eTexture, "height");
						texture.registrationPoint.x = ReadFloatAttribute(eTexture, "registrationPointX");
						texture.registrationPoint.y = ReadFloatAttribute(eTexture, "registrationPointY");

						textureSheet.textures.push_back(texture);

						eTexture = eTexture->NextSiblingElement("Texture");
					}

					eTextureSheet = eTextureSheet->NextSiblingElement("TextureSheet");
				}
			}
		}
		else
		{
			Debug::Log("Error: could not load textureSheet from: " + filename + " - " + xml.ErrorDesc());
		}
	}

#define ANIM_MONOCLE_LOGO

	void TestScene::Begin()
	{
		Scene::Begin();

		Graphics::SetBackgroundColor(Color::black);

		selectedPartIndex = 0;
		isRecording = false;
		isPlaying = false;
		playingAnimation = NULL;
		isEditing = false;
		
#ifdef ANIM_MONOCLE_LOGO
		LoadAnimation("../../Content/Flash/animations.xml");
		LoadTextureSheet("../../Content/Flash/sheets.xml");
#else
		LoadAnimation("../../Content/Flash/test.xml");
#endif

		Entity *eAnimation = new Entity();
		eAnimation->position = Vector2(400,300);
		Add(eAnimation);

		InitAnimation(&animations[0], eAnimation);
		Play(&animations[0], 15.0f);

		// fudging:
		eAnimation->position.x += 40;
		eAnimation->scale = Vector2(800.0f/640.0f, 600.0f/480.0f);

		//editor
		//Pause();
		isEditing = true;
		isRecording = true;
		Sprite::showBounds = true;
	}

	void TestScene::SelectPrevPart()
	{
		selectedPartIndex --;
		if (selectedPartIndex < 0)
		{
			selectedPartIndex = 0;
		}
	}

	void TestScene::SelectNextPart()
	{
		selectedPartIndex ++;
		if (selectedPartIndex > playingAnimation->parts.size()-1)
		{
			selectedPartIndex = playingAnimation->parts.size()-1;
		}
	}

	void TestScene::Update()
	{
		Scene::Update();

		if (playingAnimation)
		{
			if (isPlaying)
			{
				UpdateAnimation(playingAnimation);
			}
		}

		if (true)
		{
			if (isPlaying)
			{
				if (Input::IsKeyPressed(KEY_SPACE))
				{
					Pause();
				}
			}
			else if (!isPlaying)
			{
				if (Input::IsKeyPressed(KEY_LEFT))
				{
					GoPrevFrame();
				}
				if (Input::IsKeyPressed(KEY_RIGHT))
				{
					GoNextFrame();
				}
				if (Input::IsKeyPressed(KEY_Q))
				{
					SelectPrevPart();
				}
				if (Input::IsKeyPressed(KEY_E))
				{
					SelectNextPart();
				}
			
				const float moveSpeed = 40.0f;
				float moveAmount = moveSpeed * Monocle::deltaTime;
				
				const float rotateSpeed = 25.0f;
				float rotateAmount = rotateSpeed * Monocle::deltaTime;

				if (Input::IsKeyHeld(KEY_LSHIFT))
				{
					moveAmount *= 4.0f;
					rotateAmount *= 5.0f;
				}

				Part *editPart = &playingAnimation->parts[selectedPartIndex];
				Entity *editEntity = editPart->entity;
				Sprite *editSprite = editPart->sprite;

				Sprite::selectedSprite = editSprite;
			
				if (Input::IsKeyHeld(KEY_A))
				{
					editEntity->position.x -= moveAmount;
				}
				if (Input::IsKeyHeld(KEY_D))
				{
					editEntity->position.x += moveAmount;
				}
				if (Input::IsKeyHeld(KEY_W))
				{
					editEntity->position.y -= moveAmount;
				}
				if (Input::IsKeyHeld(KEY_S))
				{
					editEntity->position.y += moveAmount;
				}

				if (Input::IsKeyHeld(KEY_J))
				{
					editEntity->rotation -= rotateAmount;
				}

				if (Input::IsKeyHeld(KEY_L))
				{
					editEntity->rotation += rotateAmount;
				}

				if (isRecording)
				{
					editPart->frames[floor(animationFrame)].pos = editEntity->position;
				}

				if (Input::IsKeyPressed(KEY_SPACE))
				{
					Resume();
				}
			}
		}
	}

	// create objects required to make the animation in the entity eParent
	// if eParent is NULL, add to the scene instead
	void TestScene::InitAnimation(Animation *animation, Entity *eParent)
	{
		for (int i = 0; i < animation->parts.size(); i++)
		{
			Entity *entity = animation->parts[i].CreateEntity(textureSheet);
			if (entity)
			{
				if (eParent)
					eParent->Add(entity);
				else
					Add(entity);
			}
		}
	}

	void TestScene::Play(Animation *animation, float fps)
	{
		this->fps = fps;
		playingAnimation = animation;
		animationFrame = 0.0f;
		isPlaying = true;
		ApplyFrame();
	}

	void TestScene::Pause()
	{
		isPlaying = false;
	}

	void TestScene::Resume()
	{
		isPlaying = true;
	}

	void TestScene::GoNextFrame(int num)
	{
		animationFrame = floor(animationFrame) + num;
		if (animationFrame > playingAnimation->GetMaxFrames()-1)
		{
			animationFrame = playingAnimation->GetMaxFrames()-1;
		}
		ApplyFrame();
	}

	void TestScene::GoPrevFrame(int num)
	{
		animationFrame = floor(animationFrame) - num;
		if (animationFrame < 0)
		{
			animationFrame = 0;
		}
		ApplyFrame();
	}

	void TestScene::OffsetFramesBy(const Vector2 &offset)
	{
		for (int a = 0; a < animations.size(); a++)
		{
			for (int i = 0; i < animations[a].parts.size(); i++)
			{
				for (int j = 0; j < animations[a].parts[i].frames.size(); j++)
				{
					animations[a].parts[i].frames[j].pos += offset;
				}
			}
		}
	}

	void TestScene::UpdateAnimation(Animation *animation)
	{
		if (isPlaying)
		{
			animationFrame += Monocle::deltaTime * fps;

			if (animationFrame > animation->GetMaxFrames()-1)
			{
				animationFrame = animation->GetMaxFrames()-1;
				Pause();
			}

			ApplyFrame();
		}
	}

	void TestScene::ApplyFrame()
	{
		if (playingAnimation != NULL)
		{
			for (int i = 0; i < playingAnimation->parts.size(); i++)
			{
				playingAnimation->parts[i].Update(animationFrame);
			}
		}
	}

}