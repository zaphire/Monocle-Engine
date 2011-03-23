#include "FringeTileEditor.h"
#include "../Graphics/FringeTile.h"
#include "../Level.h"
#include "../Scene.h"
#include "../Input.h"
#include "../Monocle.h"
#include <cstdio>

namespace Monocle
{
	FringeTileEditor::FringeTileEditor()
		: scene(NULL), selectedEntity(NULL), selectedFringeTile(NULL), isOn(false), state(FTES_NONE)
	{
	}

	void FringeTileEditor::Init(Scene *scene)
	{
		this->scene = scene;
	}

	void FringeTileEditor::Enable()
	{
		isOn = true;

		Sprite::showBounds = true;
	}

	void FringeTileEditor::Disable()
	{
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
		}
	}

	void FringeTileEditor::UpdateSelect()
	{
		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || Input::IsKeyPressed(KEY_Z))
		{
			//Debug::Log("select");

			Vector2 worldMousePosition = Input::GetWorldMousePosition();
			//printf("mouse position: %d, %d\n", (int)worldMousePosition.x, (int)worldMousePosition.y);

			Entity *entity = scene->GetNearestEntityByControlPoint(worldMousePosition, selectedEntity);
			if (entity)
			{
				Select(entity);
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
	}

	void FringeTileEditor::UpdateOpportunity()
	{
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
				selectedFringeTile = NULL;
				selectedEntity = NULL;
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
			if (Input::IsKeyPressed(KEY_SPACE))
			{
				Clone();
			}
			if (Input::IsKeyPressed(KEY_TAB))
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

			if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && Input::IsKeyHeld(KEY_LSHIFT))
			{
				moveStartPosition = Input::GetWorldMousePosition();
				startRotation = selectedEntity->rotation;
				SetState(FTES_ROTATE);
				return;
				/*
				Vector2 mp = Input::GetWorldMousePosition();
				Vector2 sp = selectedEntity->GetWorldPosition(Vector2(selectedFringeTile->texture->width*0.25f, -1 * (selectedFringeTile->texture->height*0.25f)));
				Vector2 diff = mp - sp;
				printf("mp: %d, %d sp: %d, %d diff: %d, %d\n", (int)mp.x, (int)mp.y, (int)sp.x, (int)sp.y, (int)diff.x, (int)diff.y);
				if (diff.IsInRange(25))
				{
					Debug::Log("rotate!");
					moveStartPosition = Input::GetWorldMousePosition();
					startRotation = selectedEntity->rotation;
					SetState(FTES_ROTATE);
					return;
				}
				*/
			}
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

		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			moveStartPosition = selectedEntity->position;
			moveOffset = selectedEntity->position - Input::GetWorldMousePosition();
			SetState(FTES_MOVE);
			return;
		}
	}

	void FringeTileEditor::UpdateMove()
	{
		selectedEntity->position = Input::GetWorldMousePosition() + moveOffset;
		
		// cancel out of move by hitting escape
		if (Input::IsKeyPressed(KEY_ESCAPE))
		{
			selectedEntity->position = moveStartPosition;
			SetState(FTES_NONE);
			return;
		}

		// if let go, return to none state
		if (Input::IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			SetState(FTES_NONE);
			return;
		}
	}

	void FringeTileEditor::UpdateRotate()
	{
		// let go, return to none state
		if (Input::IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
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
			else if (Input::IsKeyPressed(KEY_ESCAPE))
			{
				selectedEntity->rotation = startRotation;
				SetState(FTES_NONE);
				return;
			}
			else
			{
				float add = (Input::GetWorldMousePosition().x - moveStartPosition.x)/2.4f;
				selectedEntity->rotation = startRotation + add;
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
