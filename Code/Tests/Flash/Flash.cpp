#include "Flash.h"
#include "../../TinyXML/tinyxml.h"
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

	Entity* Part::CreateEntity(const std::string &textureSheetName)
	{
		entity = new Entity();
		sprite = new Sprite("../../Content/Flash/" + textureSheetName + "/" + name + ".png");
		entity->SetGraphic(sprite);

		printf("size (%d, %d)", (int)sprite->width, (int)sprite->height);

		return entity;
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

					/*
					TiXmlElement* eTexture = eTextureSheet->FirstChildElement("Texture");
					while (eTexture)
					{
						Texture texture;
						texture.name = eTexture->Attribute("name");
						texture.width = IntAttribute(eTexture, "width");
						texture.height = IntAttribute(eTexture, "height");
					}
					*/

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

		playingAnimation = NULL;
		
#ifdef ANIM_MONOCLE_LOGO
		LoadAnimation("../../Content/Flash/animations.xml");
		LoadTextureSheet("../../Content/Flash/sheets.xml");
#else
		LoadAnimation("../../Content/Flash/test.xml");
#endif

		InitAnimation(&animations[0]);
		OffsetFramesBy(Vector2(400, 300));
		PlayAnimation(&animations[0], 15.0f);
	}

	void TestScene::Update()
	{
		Scene::Update();

		if (playingAnimation)
		{
			UpdateAnimation(playingAnimation);
		}
	}

	void TestScene::InitAnimation(Animation *animation)
	{
		for (int i = 0; i < animation->parts.size(); i++)
		{
			Add(animation->parts[i].CreateEntity(textureSheet.name));
		}
	}

	void TestScene::PlayAnimation(Animation *animation, float fps)
	{
		this->fps = fps;
		playingAnimation = animation;
		animationFrame = 0.0f;
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
		animationFrame += Monocle::deltaTime * fps;

		for (int i = 0; i < animation->parts.size(); i++)
		{
			animation->parts[i].Update(animationFrame);
		}
	}

}