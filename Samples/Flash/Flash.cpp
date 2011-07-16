#include "Flash.h"
#include <TinyXML/tinyxml.h>
#include <math.h>
#include <algorithm>

// this puppy will be cleaned up and refactored to the max later

namespace Flash
{
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

	void Part::CloneNewEndFrame()
	{
		std::vector<Frame> newFrames;
		newFrames.resize(frames.size()+1);
		for (int i = 0; i < frames.size(); i++)
		{
			newFrames[i] = frames[i];
		}
		newFrames[newFrames.size()-1] = frames[frames.size()-1];
		frames = newFrames;
	}

	Entity* Part::CreateEntity(TextureSheet &textureSheet)
	{
		Texture *texture = textureSheet.GetTextureByName(name);
		if (texture != NULL)
		{
			Entity *entity = new Entity();
			Sprite *sprite = new Sprite(textureSheet.name + "/" + name + ".png");
			sprite->position = (texture->registrationPoint * -1) + Vector2(sprite->width, sprite->height)*0.5f;
			//sprite->position = texture->registrationPoint * -1;
			entity->SetGraphic(sprite);
			entity->SetLayer(-texture->zIndex); // layers are reversed compared to zIndex

			//printf("size (%d, %d)", (int)sprite->width, (int)sprite->height);

			//hackish
			if (this->sprite == NULL)
			{
				this->sprite = sprite;
			}

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
		int frame = int(f);
		if (frame >= 0 && frame < frames.size())
		{
			// lerp!
			if (frame < frames.size()-1)
			{
				float p = f - (float)frame;

				//printf("frame: %d\nf: %f\n p: %f\n", frame, f, p);

				entity->position = frames[frame].pos + (p * (frames[frame+1].pos - frames[frame].pos));
				entity->scale = frames[frame].scale + (p * (frames[frame+1].scale - frames[frame].scale));
				entity->rotation = frames[frame].rotation + (p * (frames[frame+1].rotation - frames[frame].rotation));
				entity->color.a = frames[frame].alpha + (p * (frames[frame+1].alpha - frames[frame].alpha));

				//printf("part: %s frame: %d pos(%d, %d) size(%d, %d) a: %f\n", name.c_str(), frame, (int)entity->position.x, (int)entity->position.y, (int)sprite->width, (int)sprite->height, entity->color.a);
			}
			else
			{
				ApplyFrameToEntity(frame, entity);
			}
		}
	}

	void Part::ApplyFrameToEntity(int frame, Entity *passed)
	{
		// if entity passed in isn't set
		if (passed == NULL)
		{
			// set it to our entity pointer
			passed = this->entity;
		}

		if (passed != NULL)
		{
			passed->position = frames[frame].pos;
			passed->scale = frames[frame].scale;
			passed->color.a = frames[frame].alpha;
			passed->rotation = frames[frame].rotation;
		}
		else
		{
			Debug::Log("ApplyFrameToEntity... entity is NULL");
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
					Animation animation(XMLReadString(eAnimation, "name"), XMLReadInt(eAnimation, "frameCount"));
					
					TiXmlElement* ePart = eAnimation->FirstChildElement("Part");
					while (ePart)
					{
						Part part(ePart->Attribute("name"));
						
						TiXmlElement* eFrame = ePart->FirstChildElement("Frame");
						while (eFrame)
						{
							Frame frame;
							frame.pos = Vector2(XMLReadFloat(eFrame, "x"), XMLReadFloat(eFrame, "y"));
							
							if (eFrame->Attribute("scaleX"))
							{
								frame.scale.x = XMLReadFloat(eFrame, "scaleX");
							}

							if (eFrame->Attribute("scaleY"))
							{
								frame.scale.y = XMLReadFloat(eFrame, "scaleY");
							}

							if (eFrame->Attribute("alpha") != NULL)
							{
								frame.alpha = atof(eFrame->Attribute("alpha"));
							}

							if (eFrame->Attribute("rotation") != NULL)
							{
								frame.rotation = XMLReadFloat(eFrame, "rotation");
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
					textureSheet.name = XMLReadString(eTextureSheet, "name");

					TiXmlElement* eTexture = eTextureSheet->FirstChildElement("Texture");
					while (eTexture)
					{
						Texture texture;
						texture.name = eTexture->Attribute("name");
						texture.width = XMLReadInt(eTexture, "width");
						texture.height = XMLReadInt(eTexture, "height");
						texture.registrationPoint.x = XMLReadFloat(eTexture, "registrationPointX");
						texture.registrationPoint.y = XMLReadFloat(eTexture, "registrationPointY");
						
						if (eTexture->Attribute("zIndex") != NULL)
						{
							texture.zIndex = XMLReadInt(eTexture, "zIndex");
						} 
						else 
						{
							texture.zIndex = 0;
						}

						textureSheet.textures.push_back(texture);

						eTexture = eTexture->NextSiblingElement("Texture");
					}

					std::sort(textureSheet.textures.begin(), textureSheet.textures.end());

					eTextureSheet = eTextureSheet->NextSiblingElement("TextureSheet");
				}
			}
		}
		else
		{
			Debug::Log("Error: could not load textureSheet from: " + filename + " - " + xml.ErrorDesc());
		}
	}

//#define ANIM_MONOCLE_LOGO
#define ANIM_MONOCLE_MAN
//#define ANIM_TEST

	void TestScene::Begin()
	{
		Scene::Begin();

		Graphics::SetBackgroundColor(Color::black);
       
        const std::string cp = Assets::GetContentPath();

		editPart = NULL;
		selectedPartIndex = 0;
		isRecording = false;
		isPlaying = false;
		currentAnimation = NULL;
		isEditing = false;
		
#ifdef ANIM_MONOCLE_LOGO
		LoadAnimation(cp+"animations-monoclelogo.xml");
		LoadTextureSheet(cp+"sheets-monoclelogo.xml");
#endif

#ifdef ANIM_MONOCLE_MAN
		LoadAnimation(cp+"animations-monocleman.xml");
		LoadTextureSheet(cp+"sheets-monocleman.xml");
#endif

#ifdef ANIM_TEST
		LoadAnimation(cp+"animations-test.xml");
#endif


		eAnimation = new Entity();
		eAnimation->position = Vector2(400,300);
		Add(eAnimation);

		InitAnimation(&animations[0], eAnimation);
		Play(&animations[0], 30.0f);

		// fudging:
		//eAnimation->position.x += 40;
		eAnimation->scale = Vector2(800.0f/640.0f, 600.0f/480.0f);

		//editor
		//Pause();
		//isEditing = true;
		//isRecording = true;
		//Debug::showBounds = true;
	}

	void TestScene::SelectPrevPart()
	{
		int oldSelectedPartIndex = selectedPartIndex;

		selectedPartIndex --;
		if (selectedPartIndex < 0)
		{
			selectedPartIndex = 0;
		}
		
		if (selectedPartIndex != oldSelectedPartIndex)
		{
			DeleteOnionSkins();
			CreateOnionSkins();
			StoreBackupPartFrame();
		}

		editPart = &currentAnimation->parts[selectedPartIndex];
	}

	void TestScene::SelectNextPart()
	{	
		int oldSelectedPartIndex = selectedPartIndex;

		selectedPartIndex ++;
		if (selectedPartIndex > currentAnimation->parts.size()-1)
		{
			selectedPartIndex = currentAnimation->parts.size()-1;
		}

		if (selectedPartIndex != oldSelectedPartIndex)
		{
			DeleteOnionSkins();
			CreateOnionSkins();
			StoreBackupPartFrame();
		}

		editPart = &currentAnimation->parts[selectedPartIndex];
	}

	void TestScene::UpdateFrameNumberDisplay()
	{
		if (currentAnimation)
			printf("moved to frame: %d/%d\n", (int)(animationFrame), (int)currentAnimation->GetMaxFrames()-1);
		else
			printf("no animation\n");
	}

	Part *TestScene::GetPartForEntity(Entity *entity)
	{
		if (currentAnimation)
		{
			for (int i = 0; i < currentAnimation->parts.size(); i++)
			{
				if (currentAnimation->parts[i].entity == entity)
				{
					return &currentAnimation->parts[i];
				}
			}
		}
		return NULL;
	}

	void TestScene::Update()
	{
		Scene::Update();

		if (currentAnimation)
		{
			if (isPlaying)
			{
				UpdateAnimation(currentAnimation);
			}
		}

		if (isEditing)
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
				if (Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
				{
					Entity *clickedEntity = GetEntityAtPosition(Input::GetMousePosition());
					if (clickedEntity)
					{
						Debug::Log("clicked entity!");
						editPart = GetPartForEntity(clickedEntity);
					}
				}

				if (Input::IsKeyPressed(KEY_LEFTBRACKET))
				{
					SelectPrevPart();
				}
				if (Input::IsKeyPressed(KEY_RIGHTBRACKET))
				{
					SelectNextPart();
				}

				Entity *editEntity = NULL;
				Sprite *editSprite = NULL;

				if (editPart)
				{
					editEntity = editPart->entity;
					editSprite = editPart->sprite;
				}

				if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					if (editEntity)
					{
						state = "moving";
						offset = editEntity->position - Input::GetMousePosition();
					}
				}
				if (Input::IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
				{
					state = "";
				}

				if (state == "moving")
				{
					editEntity->position = Input::GetMousePosition() + offset;
				}

				Debug::selectedEntity = editEntity;
				
				if (Input::IsKeyPressed(KEY_LEFT))
				{
					state = "";
					GoPrevFrame();
					UpdateFrameNumberDisplay();
				}
				if (Input::IsKeyPressed(KEY_RIGHT))
				{
					state = "";
					GoNextFrame();
					UpdateFrameNumberDisplay();
				}
				if (Input::IsKeyPressed(KEY_UP))
				{
					state = "";
					CloneNewEndFrame();
					UpdateFrameNumberDisplay();
				}
			
				const float moveSpeed = 40.0f;
				float moveAmount = moveSpeed * Monocle::deltaTime;
				
				const float rotateSpeed = 25.0f;
				float rotateAmount = rotateSpeed * Monocle::deltaTime;

				const float scaleSpeed = 0.5f;
				float scaleAmount = scaleSpeed * Monocle::deltaTime;

				if (Input::IsKeyHeld(KEY_LSHIFT))
				{
					moveAmount *= 4.0f;
					rotateAmount *= 5.0f;
					scaleAmount *= 4.0f;
				}
				
				if (editEntity)
				{
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

					if (Input::IsKeyHeld(KEY_Q))
					{
						editEntity->rotation -= rotateAmount;
					}
					if (Input::IsKeyHeld(KEY_E))
					{
						editEntity->rotation += rotateAmount;
					}

					// flip when rotation is in certain quadrants
					if (Input::IsKeyHeld(KEY_J))
					{
						editEntity->scale.x -= scaleAmount;
					}
					if (Input::IsKeyHeld(KEY_L))
					{
						editEntity->scale.x += scaleAmount;
					}
					if (Input::IsKeyHeld(KEY_I))
					{
						editEntity->scale.y += scaleAmount;
					}
					if (Input::IsKeyHeld(KEY_K))
					{
						editEntity->scale.y -= scaleAmount;
					}
				}



				// store part's frame when we switch frames/parts
				// restore to that if hit a key
				if (Input::IsKeyPressed(KEY_BACKSPACE))
				{
					RevertToBackupPartFrame();
				}

				// apply the previous frame data to this one
				if (Input::IsKeyPressed(KEY_MINUS))
				{
					ApplyPrevPartFrame();
				}

				// apply the next frame data to this one
				if (Input::IsKeyPressed(KEY_EQUALS))
				{
					ApplyNextPartFrame();
				}

				if (isRecording)
				{
					if (editPart)
					{
						int currentFrame = int(animationFrame);
						editPart->frames[currentFrame].pos = editEntity->position;
						editPart->frames[currentFrame].scale = editEntity->scale;
						editPart->frames[currentFrame].rotation = editEntity->rotation;
					}
				}

				if (Input::IsKeyPressed(KEY_SPACE))
				{
					Resume();
				}
			}
		}
	}

	void TestScene::ApplyPrevPartFrame()
	{
		if (currentAnimation)
		{
			int currentFrame = int(animationFrame);
			int prevFrame = SafeFrameRange(currentFrame-1);
			///TODO: build functions to make this prettier
			if (editPart)
			{
				editPart->frames[currentFrame] = editPart->frames[prevFrame];
				editPart->ApplyFrameToEntity(currentFrame, editPart->entity);
			}
		}
	}

	void TestScene::ApplyNextPartFrame()
	{
		if (currentAnimation)
		{
			int currentFrame = int(animationFrame);
			int nextFrame = SafeFrameRange(currentFrame+1);
			///TODO: build functions to make this prettier
			if (editPart)
			{
				editPart->frames[currentFrame] = editPart->frames[nextFrame];
				editPart->ApplyFrameToEntity(currentFrame, editPart->entity);
			}
		}
	}

	void TestScene::RevertToBackupPartFrame()
	{
		Debug::Log("RevertToBackupPartFrame");

		if (editPart)
		{
			int currentFrame = int(animationFrame);
			editPart->frames[currentFrame] = backupPartFrame;
			editPart->ApplyFrameToEntity(currentFrame);
		}
	}

	void TestScene::StoreBackupPartFrame()
	{
		Debug::Log("StoreBackupPartFrame");
		if (editPart)
		{
			backupPartFrame = editPart->frames[int(animationFrame)];
		}
	}

	void TestScene::CloneNewEndFrame()
	{
		for (int i = 0; i < currentAnimation->parts.size(); i++)
		{
			currentAnimation->parts[i].CloneNewEndFrame();
		}
		GoNextFrame(currentAnimation->GetMaxFrames());
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
				animation->parts[i].entity = entity;
				Add(entity);
				entity->SetParent(eParent);
			}
		}
	}

	void TestScene::Play(Animation *animation, float fps)
	{
		this->fps = fps;
		currentAnimation = animation;
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
		DeleteOnionSkins();

		animationFrame = int(animationFrame) + num;
		if (animationFrame > currentAnimation->GetMaxFrames()-1)
		{
			animationFrame = currentAnimation->GetMaxFrames()-1;
		}

		ApplyFrame();

		CreateOnionSkins();
		StoreBackupPartFrame();
	}

	void TestScene::GoPrevFrame(int num)
	{
		DeleteOnionSkins();

		animationFrame = int(animationFrame) - num;
		if (animationFrame < 0)
		{
			animationFrame = 0;
		}

		ApplyFrame();

		CreateOnionSkins();
		StoreBackupPartFrame();
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
		if (currentAnimation != NULL)
		{
			for (int i = 0; i < currentAnimation->parts.size(); i++)
			{
				currentAnimation->parts[i].Update(animationFrame);
			}
		}
	}
	
	void TestScene::CreateOnionSkins()
	{
		// current part
		Part *editPart = &currentAnimation->parts[selectedPartIndex];
		if (editPart)
		{
			Entity *editEntity = editPart->entity;
			Sprite *editSprite = editPart->sprite;

			int currentFrame = int(animationFrame);
			
			const int numFramesToPreview = 5;

			for (int i = 0; i < numFramesToPreview; i++)
			{
				int prevFrame = SafeFrameRange(currentFrame - i);
				int nextFrame = SafeFrameRange(currentFrame + i);

				const float dyeP = 0.25f;

				if (prevFrame != currentFrame)
				{
					Entity *entity = editPart->CreateEntity(textureSheet);
					Add(entity);
					entity->SetParent(eAnimation);
					editPart->ApplyFrameToEntity(prevFrame, entity);
					onionSkins.push_back(entity);
					float oldA = entity->color.a;
					entity->color = entity->color * (1.0f-dyeP) + Color::blue * dyeP;
					entity->color.a = oldA * (0.25f - 0.05f*i);
				}

				if (nextFrame != currentFrame)
				{
					// spawn part at next frame
					Entity *entity = editPart->CreateEntity(textureSheet);
					Add(entity);
					entity->SetParent(eAnimation);
					editPart->ApplyFrameToEntity(nextFrame, entity);
					onionSkins.push_back(entity);
					float oldA = entity->color.a;
					entity->color = entity->color * (1.0f-dyeP) + Color::red * dyeP;
					entity->color.a = oldA * (0.25f - 0.05f*i);
				}
			}
		}
	}

	void TestScene::DeleteOnionSkins()
	{
		for (std::vector<Entity*>::iterator i = onionSkins.begin(); i != onionSkins.end(); ++i)
		{
			Remove(*i);
			//eAnimation->Remove((*i));
		}
		onionSkins.clear();
	}

	int TestScene::SafeFrameRange(int frame)
	{
		if (currentAnimation)
		{
			if (frame > currentAnimation->GetMaxFrames()-1)
				frame = currentAnimation->GetMaxFrames()-1;

			if (frame < 0)
				return 0;
		}
		return frame;
	}

}
