#include "PuppetEditor.h"
#include "Game.h"

#include "../MonocleToolkit.h"
#include "../Assets.h"

namespace Monocle
{
	const float TIME_STEP = 0.05f;

	/// TIMELINE
	Timeline::Timeline()
		: Entity(), currentAnimation(NULL)
	{

	}

	void Timeline::Render()
	{
		//Entity::Render();

		if (currentAnimation)
		{
			Graphics::PushMatrix();
			float lineHalfWidth = Graphics::GetVirtualWidth() * 0.9f * 0.5f;
			float y = Graphics::GetVirtualHeight() * 0.5f - 50;
			float p = currentAnimation->GetCurrentTime() / currentAnimation->GetDuration();
			Vector2 start = Graphics::GetScreenCenter() + Vector2(-lineHalfWidth, y);
			Vector2 end = Graphics::GetScreenCenter() + Vector2(lineHalfWidth, y);
			Graphics::BindTexture(NULL);
			Graphics::SetColor(Color::blue);
			Graphics::RenderLine(start, end);
			Vector2 playHead;
			float playHeadHalfHeight = 16;

			if (Debug::selectedEntity)
			{
				Graphics::SetColor(Color::orange);
				Part *part = dynamic_cast<Part*>(Debug::selectedEntity);
				if (part)
				{
					PartKeyFrames *partKeyFrames = currentAnimation->GetPartKeyFrames(part);
					if (partKeyFrames)
					{

						const std::list<KeyFrame> *keyFrames = partKeyFrames->GetKeyFrames();
						for (std::list<KeyFrame>::const_iterator i = keyFrames->begin(); i != keyFrames->end(); ++i)
						{
							playHead = (end-start) * ((*i).GetTime()/currentAnimation->GetDuration()) + start;
							Graphics::RenderLine(playHead + Vector2::up * playHeadHalfHeight, playHead + Vector2::down * playHeadHalfHeight);
						}
					}
				}
			}

			playHead = (end - start) * p + start;

			Graphics::SetColor(Color::blue);
			Graphics::RenderLine(playHead + Vector2::up * playHeadHalfHeight * 0.5f, playHead + Vector2::down * playHeadHalfHeight * 0.5f);

			Graphics::PopMatrix();
		}
	}

	PuppetEntity::PuppetEntity()
		: Entity()
	{
	}

	void PuppetEntity::Load(const std::string &filename)
	{
		puppet.Load(filename, this);

		puppet.Play("idle");
	}

	void PuppetEntity::Update()
	{
		Entity::Update();
		puppet.Update();
	}

	/// PUPPET EDITOR
	PuppetEditor::PuppetEditor() : Editor()
	{
	}

	void PuppetEditor::Load(const std::string &filename)
	{
		puppetEntity->Load(filename);
	}

	void PuppetEditor::Added()
	{
		keyTogglePause = KEY_TAB;

		keyMoveLeft = KEY_A;
		keyMoveRight = KEY_D;
		keyMoveUp = KEY_W;
		keyMoveDown = KEY_S;

		keyRotateLeft = KEY_J;
		keyRotateRight = KEY_L;

		keyBackwards = KEY_LEFT;
		keyForwards = KEY_RIGHT;

		keySetKeyFrame = KEY_SPACE;

		keyOffset = KEY_LALT;

		keyZero = KEY_0;

		Graphics::Set2D(800, 600);
		//Graphics::SetBackgroundColor(Color::white);
        Game::GetScene()->GetCamera()->position = Graphics::GetScreenCenter();

		puppetEntity = new PuppetEntity();
		puppetEntity->position = Graphics::GetScreenCenter() + Vector2::down * 100;
		puppetEntity->scale = Vector2::one * 0.75f;
		scene->Add(puppetEntity);

		Entity *entity = new Entity();
		entity->SetLayer(5);
		entity->SetGraphic(new Sprite("graphics/logo.png"));
		entity->position = Graphics::GetScreenCenter() + Vector2::up * 138;
		scene->Add(entity);

		timeline = new Timeline();
		scene->Add(timeline);
	}
	
	void PuppetEditor::Enable()
	{
		Editor::Enable();
	}
	
	void PuppetEditor::Disable()
	{
		Editor::Disable();
	}

	void PuppetEditor::Update()
	{
		Editor::Update();

		if (isEnabled)
		{
			UpdateCamera();

			timeline->currentAnimation = puppetEntity->puppet.GetCurrentAnimation();

			Animation *anim = puppetEntity->puppet.GetCurrentAnimation();

			if (Input::IsKeyPressed(keyTogglePause))
			{
				puppetEntity->puppet.TogglePause();
				Debug::render = Debug::showBounds = puppetEntity->puppet.IsPaused();
				if (!puppetEntity->puppet.IsPaused())
				{
					Debug::selectedEntity = NULL;
				}
				else
				{
					if (anim)
					{
						anim->SetCurrentTime(int(anim->GetCurrentTime()/TIME_STEP)*TIME_STEP);
					}
				}
			}

			if (puppetEntity->puppet.IsPaused())
			{
				if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
					Debug::selectedEntity = scene->GetNearestEntityByControlPoint(Input::GetWorldMousePosition(), "", Debug::selectedEntity);

				if (Input::IsKeyHeld(KEY_LCTRL))
				{
					if (Input::IsKeyPressed(KEY_S))
					{
						puppetEntity->puppet.Save();
					}
				}

				if (Debug::selectedEntity)
				{
					Part *part = dynamic_cast<Part*>(Debug::selectedEntity);

					float moveSpeed = 10.0f;
					float rotateSpeed = 15.0f;

					if (Input::IsKeyHeld(KEY_LSHIFT))
					{
						float multiplier = 5.0f;
						moveSpeed *= multiplier;
						rotateSpeed *= multiplier;
					} 

					if (Input::IsKeyPressed(keyZero))
					{
						part->position = Vector2::zero;
						part->rotation = 0.0f;
						part->GetSprite()->position = Vector2::zero;
					}

					float moveAmount = Monocle::deltaTime * moveSpeed * 10.0f;
					float rotateAmount = Monocle::deltaTime * rotateSpeed;

					if (Input::IsKeyHeld(KEY_LCTRL))
					{
					}
					else
					{
						if (Input::IsKeyHeld(keyOffset))
						{
							if (part)
							{
								Sprite *sprite = part->GetSprite();

								if (Input::IsKeyHeld(keyMoveLeft))
									sprite->position += Vector2::left * moveAmount;
								if (Input::IsKeyHeld(keyMoveRight))
									sprite->position += Vector2::right * moveAmount;
								if (Input::IsKeyHeld(keyMoveUp))
									sprite->position += Vector2::up * moveAmount;
								if (Input::IsKeyHeld(keyMoveDown))
									sprite->position += Vector2::down * moveAmount;
							}
						}
						else
						{
							if (Input::IsKeyHeld(keyMoveLeft))
								Debug::selectedEntity->position += Vector2::left * moveAmount;
							if (Input::IsKeyHeld(keyMoveRight))
								Debug::selectedEntity->position += Vector2::right * moveAmount;
							if (Input::IsKeyHeld(keyMoveUp))
								Debug::selectedEntity->position += Vector2::up * moveAmount;
							if (Input::IsKeyHeld(keyMoveDown))
								Debug::selectedEntity->position += Vector2::down * moveAmount;
						}

						if (Input::IsKeyHeld(keyRotateLeft))
							Debug::selectedEntity->rotation -= rotateAmount;
						if (Input::IsKeyHeld(keyRotateRight))
							Debug::selectedEntity->rotation += rotateAmount;
					}


				}
				else 
				{

				}

				if (anim)
				{
					Part *part = dynamic_cast<Part*>(Debug::selectedEntity);

					if (Input::IsKeyPressed(keyBackwards))
						anim->AdjustCurrentTime(-TIME_STEP, false);
					if (Input::IsKeyPressed(keyForwards))
						anim->AdjustCurrentTime(TIME_STEP, false);

					if (Input::IsKeyPressed(keySetKeyFrame))
					{
						anim->SetPartKeyFrame(part, KeyFrame(anim->GetCurrentTime(), *part));
						//puppetEntity.puppet.GetCurrentAnimation()->AddNewPartKeyFrame();
					}
				}
			}
			else
			{
				//if (Input::IsMouseButtonHeld(MOUSE_BUTTON_LEFT))
				//puppetEntity->position = WEIGHTED_AVERAGE(puppetEntity->position, Input::GetWorldMousePosition(), 15);
			}
		}
	}
}

