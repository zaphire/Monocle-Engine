#include "FringeTileEditor.h"
#include "../Graphics/FringeTile.h"
#include "../Level.h"
#include "../Scene.h"
#include "../Input.h"
#include "../Monocle.h"
#include <cstdio>

namespace Monocle
{
	/*
	Cursor::Cursor()
		: Entity()
	{
		SetLayer(-50);
		color = Color::orange;
		color.a = 0.5f;
	}

	void Cursor::Update()
	{
		position = Input::GetWorldMousePosition();
	}

	void Cursor::Render()
	{
		Graphics::Translate(position);
		Graphics::BindTexture(NULL);
		//Debug::Log("cursor, render");
		//Graphics::RenderQuad(50, 50);
		Graphics::SetColor(color);
		Graphics::Rotate(Monocle::timeSinceStart * 180, 0, 0, 1);
		Graphics::RenderLineRect(0, 0, 10,10);
		Graphics::RenderLineRect(0, 0, 2,2);
	}
	*/

	FringeTileEditor::FringeTileEditor()
		: scene(NULL), selectedEntity(NULL), selectedFringeTile(NULL), isOn(false), state(FTES_NONE), waitForLMBRelease(false), moveAxisLock(0)
		//, cursor(NULL)
	{
		keyMove = KEY_Q;
		keyRotate = KEY_W;
		keyScale = KEY_E;
		keySelect = KEY_SPACE;
		keyFocus = KEY_F;
		keyClone = KEY_T;
		keyFlipV = KEY_C;
		keyFlipH = KEY_V;
	}

	void FringeTileEditor::Init(Scene *scene)
	{
		this->scene = scene;

		//cursor = new Cursor();
		//scene->Add(cursor);
	}

	void FringeTileEditor::Enable()
	{
		isOn = true;

		Sprite::showBounds = true;
	}

	void FringeTileEditor::Disable()
	{
		Select(NULL);
		isOn = false;

		Sprite::showBounds = false;
	}

	void FringeTileEditor::Update()
	{
		if (isOn)
		{
			UpdateCamera();

			if (state == FTES_NONE)
			{
				UpdateSelect();
			}

			if (selectedEntity)
			{
				switch (state)
				{
				case FTES_NONE:
					UpdateOpportunity();
					break;

				case FTES_COMMAND:
					UpdateCommand();
					break;

				case FTES_MOVE:
					UpdateMove();
					break;

				case FTES_ROTATE:
					UpdateRotate();
					break;

				case FTES_SCALE:
					UpdateScale();
					break;
				}
			}
		}
	}

	void FringeTileEditor::UpdateCommand()
	{

	}

	void FringeTileEditor::UpdateCamera()
	{
		if (!Input::IsKeyHeld(KEY_LCTRL))
		{
			float moveSpeed = 200.0f;
			float zoomSpeed = 0.5f;

			if (Input::IsKeyHeld(KEY_LSHIFT))
			{
				moveSpeed *= 5.0f;
				zoomSpeed *= 2.0f;
			}
			/*
			if (Input::IsKeyHeld(KEY_D))
			{
				Graphics::AdjustCameraPosition(Vector2::right * moveSpeed * Monocle::deltaTime);
			}
			if (Input::IsKeyHeld(KEY_A))
			{
				Graphics::AdjustCameraPosition(Vector2::left * moveSpeed * Monocle::deltaTime);
			}
			if (Input::IsKeyHeld(KEY_W))
			{
				Graphics::AdjustCameraPosition(Vector2::up * moveSpeed * Monocle::deltaTime);
			}
			if (Input::IsKeyHeld(KEY_S))
			{
				Graphics::AdjustCameraPosition(Vector2::down * moveSpeed * Monocle::deltaTime);
			}
			if (Input::IsKeyHeld(KEY_Q))
			{
				Graphics::AdjustCameraZoom(-1*Vector2::one * zoomSpeed * Monocle::deltaTime);
			}
			if (Input::IsKeyHeld(KEY_E))
			{
				Graphics::AdjustCameraZoom(Vector2::one * zoomSpeed * Monocle::deltaTime);
			}
			*/
			if (Platform::mouseWheel != 0)
			{
				Graphics::AdjustCameraZoom(Platform::mouseWheel * Vector2::one * 0.0005f);
			}
			if (Input::IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
			{
				lastWorldMousePosition = Input::GetWorldMousePosition();
			}
			if (Input::IsMouseButtonHeld(MOUSE_BUTTON_MIDDLE))
			{
				Vector2 diff = Input::GetWorldMousePosition() - lastWorldMousePosition;
				Graphics::AdjustCameraPosition(-1*diff);
				lastWorldMousePosition = Input::GetWorldMousePosition();
				//const float camPanSpeed = 8.0f;
				//Graphics::AdjustCameraPosition((Input::GetMousePosition() - startCameraMovePosition)*camPanSpeed*Monocle::deltaTime);//(Input::GetWorldMousePosition() - Graphics::GetCameraPosition()));
			}
		}
	}

	void FringeTileEditor::UpdateSelect()
	{
		if (Input::IsKeyPressed(KEY_ESCAPE))
		{
			if (selectedEntity == NULL)
			{
				Sprite::showBounds = !Sprite::showBounds;
			}
		}
		else
		{
			if (Sprite::showBounds == true)
			{
				if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || Input::IsKeyPressed(keySelect))
				{
					Vector2 worldMousePosition = Input::GetWorldMousePosition();
					//"FringeTile"
					Entity *entity = scene->GetNearestEntityByControlPoint(worldMousePosition, "", selectedEntity);
					if (entity)
					{
						Select(entity);
						//waitForLMBRelease = true;
					}
				}
			}
		}
	}

	void FringeTileEditor::Clone()
	{
		if (selectedEntity && selectedFringeTile)
		{
			FringeTile *fringeTile = Level::AddFringeTile(selectedFringeTile->GetFringeTileset(), selectedFringeTile->GetTileID(), selectedEntity->GetLayer(), Input::GetWorldMousePosition(), selectedEntity->scale, selectedEntity->rotation);
			Tween::FromTo(&fringeTile->entity->color.a, 0.0f, 1.0f, 0.25f, EASE_LINEAR);
			Select(fringeTile->entity);
		}
	}

	void FringeTileEditor::Select(Entity *entity)
	{
		if (entity == NULL)
			Debug::Log("select none");
		selectedEntity = entity;
		Sprite::selectedSpriteEntity = selectedEntity;

		if (entity)
		{
			// unhappy with this so far:
			selectedFringeTile = Level::GetFringeTileForEntity(selectedEntity);
		}
		else
		{
			selectedFringeTile = NULL;
		}

		if (entity != NULL)
		{
			printf("entity (%d, %d)\n", (int)entity->position.x, (int)entity->position.y);

			printf("(Q) move (W) rotate (E) scale\n");
		}
	}

	void FringeTileEditor::ApplyGrid(Entity *entity, int gridSize)
	{
		int x = (entity->position.x / gridSize);
		entity->position.x = (x * gridSize) + gridSize*0.5f;
		int y = (entity->position.y / gridSize);
		entity->position.y = (y * gridSize) + gridSize*0.5f;

		printf("applied grid %d now (%d, %d)\n", gridSize, (int)entity->position.x, (int)entity->position.y);
	}

	void FringeTileEditor::UpdateOpportunity()
	{
		if (waitForLMBRelease && Input::IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			waitForLMBRelease = false;
		}

		if (Input::IsKeyPressed(KEY_ESCAPE))
		{
			Select(NULL);
			return;
		}

		if (Input::IsKeyPressed(KEY_1))
		{
			ApplyGrid(selectedEntity, 32);
		}
		if (Input::IsKeyPressed(KEY_2))
		{
			ApplyGrid(selectedEntity, 64);
		}
		if (Input::IsKeyPressed(KEY_3))
		{
			ApplyGrid(selectedEntity, 128);
		}
		if (Input::IsKeyPressed(KEY_4))
		{
			ApplyGrid(selectedEntity, 256);
		}
		if (Input::IsKeyPressed(KEY_5))
		{
			ApplyGrid(selectedEntity, 512);
		}

		if (Input::IsKeyPressed(keyFocus))
		{
			Graphics::MoveCameraPosition(selectedEntity->position, 0.125f, EASE_OUTSIN);
			//Graphics::SetCameraPosition(selectedEntity->position);
		}

		if (selectedFringeTile)
		{
			if (Input::IsKeyPressed(KEY_BACKSPACE))
			{
				///HACK: need to delete graphic and entity at some point
				/// doesn't happen automatically (yet)
				Level::RemoveFringeTile(selectedFringeTile);
				//selectedEntity->SetGraphic(NULL);
				//delete selectedFringeTile;
				scene->Remove(selectedEntity);
				// enqueue deletion of entity?
				//delete selectedEntity;
				Select(NULL);
				return;
			}

			if (Input::IsKeyPressed(KEY_MINUS))
			{
				selectedFringeTile->PrevTile();
			}
			if (Input::IsKeyPressed(KEY_EQUALS))
			{
				selectedFringeTile->NextTile();
			}
			if (Input::IsKeyPressed(keyClone))
			{
				Clone();
			}
			if (Input::IsKeyPressed(KEY_M))
			{
				std::string path = Monocle::GetWorkingDirectory() + selectedFringeTile->texture->filename;
				Debug::Log("Opening: " + path + " ...");
				Monocle::OpenURL(path);
			}
			if (Input::IsKeyPressed(KEY_R))
			{
				selectedFringeTile->texture->Reload();
			}

			if (Input::IsKeyHeld(KEY_LSHIFT) && Input::IsKeyPressed(KEY_0))
			{
				selectedEntity->rotation = 0;
			}

			if (Input::IsKeyPressed(keyFlipH))
			{
				selectedEntity->scale.x *= -1;
			}
			if (Input::IsKeyPressed(keyFlipV))
			{
				selectedEntity->scale.y *= -1;
			}


		}

		const float moveSpeed = 10.0f;
		if (Input::IsKeyHeld(KEY_LEFT))
		{
			selectedEntity->position += Vector2::left * moveSpeed * Monocle::deltaTime;
		}
		if (Input::IsKeyHeld(KEY_RIGHT))
		{
			selectedEntity->position += Vector2::right * moveSpeed * Monocle::deltaTime;
		}
		if (Input::IsKeyHeld(KEY_UP))
		{
			selectedEntity->position += Vector2::up * moveSpeed * Monocle::deltaTime;
		}
		if (Input::IsKeyHeld(KEY_DOWN))
		{
			selectedEntity->position += Vector2::down * moveSpeed * Monocle::deltaTime;
		}

		if (Input::IsKeyPressed(KEY_O))
		{
			Vector2 add = Vector2(SIGNOF(selectedEntity->scale.x), SIGNOF(selectedEntity->scale.y)) * 0.125f;
			selectedEntity->scale += add;
			printf("add(%f, %f)\n", add.x, add.y);
		}
		if (Input::IsKeyPressed(KEY_U))
		{
			selectedEntity->scale -= Vector2(SIGNOF(selectedEntity->scale.x), SIGNOF(selectedEntity->scale.y)) * 0.125f;
		}

		if (Input::IsKeyPressed(KEY_I))
		{
			selectedEntity->AdjustLayer(1);
			printf("layer is now: %d\n", selectedEntity->GetLayer());
		}
		if (Input::IsKeyPressed(KEY_K))
		{
			selectedEntity->AdjustLayer(-1);
			printf("layer is now: %d\n", selectedEntity->GetLayer());
		}

		if (!waitForLMBRelease && Input::IsKeyHeld(keyRotate))
		{
			moveStartPosition = Input::GetWorldMousePosition();
			startRotation = selectedEntity->rotation;
			SetState(FTES_ROTATE);
			return;
		}

		if (!waitForLMBRelease && Input::IsKeyPressed(keyMove))//Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			Debug::Log("move start");
			moveStartPosition = selectedEntity->position;
			moveOffset = selectedEntity->position - Input::GetWorldMousePosition();
			moveAxisLock = 0;
			SetState(FTES_MOVE);
			return;
		}
	}

	void FringeTileEditor::UpdateMove()
	{
		if (Input::IsKeyPressed(KEY_X))
			moveAxisLock = 1;
		else if (Input::IsKeyPressed(KEY_Y))
			moveAxisLock = 2;

		selectedEntity->position = Input::GetWorldMousePosition() + moveOffset;

		switch(moveAxisLock)
		{
		case 1:
			selectedEntity->position.y = moveStartPosition.y;
			break;
		case 2:
			selectedEntity->position.x = moveStartPosition.x;
			break;
		}
		
		if (Input::IsKeyPressed(KEY_0))
		{
			selectedEntity->position = Vector2::zero;
			SetState(FTES_NONE);
			return;
		}

		// cancel out of move by hitting escape or rmb
		if (Input::IsKeyPressed(KEY_ESCAPE) || Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			selectedEntity->position = moveStartPosition;
			SetState(FTES_NONE);
			return;
		}

		// if let go, return to none state
		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || Input::IsKeyPressed(keyMove) || Input::IsKeyPressed(keySelect))
		{
			SetState(FTES_NONE);
			return;
		}
	}

	void FringeTileEditor::UpdateRotate()
	{
		// let go, return to none state
		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || Input::IsKeyPressed(keyRotate) || Input::IsKeyPressed(keySelect))
		{
			SetState(FTES_NONE);
		}
		else
		{
			if (Input::IsKeyPressed(KEY_0))
			{
				selectedEntity->rotation = 0;
				SetState(FTES_NONE);
				return;
			}
			else if (Input::IsKeyPressed(KEY_ESCAPE) || Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
			{
				selectedEntity->rotation = startRotation;
				SetState(FTES_NONE);
				return;
			}
			else
			{
				float add = (Input::GetWorldMousePosition().x - moveStartPosition.x)/2.4f;
				if (Input::IsKeyHeld(KEY_LCTRL))
				{
					int grads = (startRotation + add) / (45.0f/2.0f);
					selectedEntity->rotation = grads * (45.0f/2.0f);
				}
				else
				{
					selectedEntity->rotation = startRotation + add;
				}
			}
		}
	}

	void FringeTileEditor::UpdateScale()
	{
		if (Input::IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			SetState(FTES_NONE);
		}
	}

	void FringeTileEditor::SetState(FringeTileEditorState state)
	{
		this->state = state;
	}
}
