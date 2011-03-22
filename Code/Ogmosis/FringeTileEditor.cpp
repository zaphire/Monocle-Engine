#include "FringeTileEditor.h"
#include "../Level.h"
#include "../Scene.h"
#include "../Input.h"
#include "../FringeTile.h"

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

	void FringeTileEditor::UpdateSelect()
	{
		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			Debug::Log("select");

			printf("mouse position: %d, %d", (int)Input::GetMousePosition().x, (int)Input::GetMousePosition().y);

			Entity *entity = scene->GetNearestEntityContaining(Input::GetMousePosition(), selectedEntity);//scene->GetEntityAtPosition(Input::GetMousePosition());
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
			FringeTile *fringeTile = Level::AddFringeTile(selectedFringeTile->GetFringeTileset(), selectedFringeTile->GetTileID(), selectedEntity->GetLayer(), Input::GetMousePosition(), selectedEntity->scale, selectedEntity->rotation);
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
		}

		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			moveStartPosition = selectedEntity->position;
			moveOffset = selectedEntity->position - Input::GetMousePosition();
			SetState(FTES_MOVE);
			return;
		}
	}

	void FringeTileEditor::UpdateMove()
	{
		selectedEntity->position = Input::GetMousePosition() + moveOffset;
		
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