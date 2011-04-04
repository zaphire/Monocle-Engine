#include "FringeTileEditor.h"
#include "FringeTile.h"
#include "../Level.h"
#include "../Scene.h"
#include "../Input.h"
#include "../Monocle.h"
#include <cstdio>
#include <cmath>

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
		keyFlipH = KEY_C;
		keyFlipV = KEY_V;
		keyPrevTile = KEY_LEFTBRACKET;
		keyNextTile = KEY_RIGHTBRACKET;
		keyDelete = KEY_BACKSPACE;
		keyScaleDown = KEY_MINUS;
		keyScaleUp = KEY_EQUALS;
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

		Debug::showBounds = true;
		Debug::render = true;
	}

	void FringeTileEditor::Disable()
	{
		Select(NULL);
		isOn = false;

		Debug::showBounds = false;
		Debug::render = false;
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
				scene->GetCamera()->scale += (Platform::mouseWheel * Vector2::one * 0.00025f);
				Vector2 camZoom = scene->GetCamera()->scale;//Graphics::GetCameraZoom();
				printf("camZoom (%f, %f)\n", camZoom.x, camZoom.y);
			}
			if (Input::IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
			{
				lastWorldMousePosition = Input::GetWorldMousePosition();
			}
			if (Input::IsMouseButtonHeld(MOUSE_BUTTON_MIDDLE))
			{
				Vector2 diff = Input::GetWorldMousePosition() - lastWorldMousePosition;
				scene->GetCamera()->position += (-1*diff);
				lastWorldMousePosition = Input::GetWorldMousePosition();

				Vector2 camPos = scene->GetCamera()->position;
				printf("camPos (%d, %d)\n", (int)camPos.x, (int)camPos.y);
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
				Debug::showBounds = !Debug::showBounds;
			}
		}
		else
		{
			if (Debug::showBounds == true)
			{
				if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || Input::IsKeyPressed(keySelect))
				{
					Vector2 worldMousePosition = Input::GetWorldMousePosition();
					//"FringeTile"
					Entity *entity = scene->GetNearestEntityByControlPoint(worldMousePosition, "", selectedEntity);
					if (entity)
					{
						Select(entity);
						if(Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
							waitForLMBRelease = true;
					}
				}
			}
		}
	}

	template <class T>
	void FringeTileEditor::CloneEntity(T *t)
	{
		T *newT = new T(*t);
		Entity *entity = (Entity*)newT;
		entity->position = Input::GetWorldMousePosition();
		scene->Add(entity);
		Select(entity);
	}

	void FringeTileEditor::CloneNode()
	{
		if (selectedEntity && selectedNode)
		{
			Node *newNode = new Node(Input::GetWorldMousePosition());
			newNode->Copy(selectedNode);
			if (selectedNode->GetParent())
			{
				///HACK: replace with "GetLocalPosition" function
				newNode->position = Input::GetWorldMousePosition() - selectedNode->GetParent()->position;
				selectedNode->GetParent()->Add(newNode);
			}
			else
				scene->Add(newNode);
			selectedNode->InsertNext(newNode);
			Select(newNode);
		}
	}

	void FringeTileEditor::Select(Entity *entity)
	{
		if (entity == NULL)
			Debug::Log("select none");

		selectedEntity = entity;

		Debug::selectedEntity = selectedEntity;

		if (entity)
		{
			selectedFringeTile = dynamic_cast<FringeTile*>(entity);
			selectedNode = dynamic_cast<Node*>(entity);
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
			//Graphics::MoveCameraPosition(selectedEntity->position, 0.125f, EASE_OUTSIN);
			scene->GetCamera()->position = selectedEntity->position;
		}

		if (selectedNode)
		{
			if (Input::IsKeyPressed(keyClone))
			{
				CloneNode();
			}
			if (Input::IsKeyPressed(keyDelete))
			{
				Node *lastSelectedNode = selectedNode;
				Select(selectedNode->GetPrev());
				lastSelectedNode->TakeOut();
				lastSelectedNode->RemoveSelf();
				return;
			}

			if (Input::IsKeyPressed(KEY_LEFTBRACKET))
			{
				selectedNode->variant--;
			}
			if (Input::IsKeyPressed(KEY_RIGHTBRACKET))
			{
				selectedNode->variant++;
			}

			/*
			const float sizeSpeed = 32.0f;
			if (Input::IsKeyHeld(keyScaleDown))
			{
				selectedNode->scale -= sizeSpeed * Monocle::deltaTime;
			}
			if (Input::IsKeyHeld(keyScaleUp))
			{
				selectedNode->size += sizeSpeed * Monocle::deltaTime;
			}
			*/

			//printf("selectedNode->size: %f\n", selectedNode->scale.y);
		}
		if (selectedFringeTile)
		{
			if (Input::IsKeyPressed(KEY_B))
			{
				selectedFringeTile->PrevBlend();
			}
			if (Input::IsKeyPressed(KEY_N))
			{
				selectedFringeTile->NextBlend();
			}

			if (Input::IsKeyPressed(KEY_LEFTBRACKET))
			{
				selectedFringeTile->PrevTile();
			}
			if (Input::IsKeyPressed(KEY_RIGHTBRACKET))
			{
				selectedFringeTile->NextTile();
			}
			if (Input::IsKeyPressed(keyClone))
			{
				CloneEntity(selectedFringeTile);
			}
			if (Input::IsKeyPressed(KEY_M))
			{
				std::string path = Monocle::GetWorkingDirectory() + selectedFringeTile->sprite->texture->filename;
				Debug::Log("Opening: " + path + " ...");
				Monocle::OpenURL(path);
			}
			if (Input::IsKeyPressed(KEY_R))
			{
				selectedFringeTile->sprite->texture->Reload();
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

		if (Input::IsKeyPressed(keyDelete))
		{
			scene->Remove(selectedEntity);
			Select(NULL);
			return;
		}

		if (Input::IsKeyPressed(keyScaleUp))
		{
			Vector2 add = Vector2(SIGNOF(selectedEntity->scale.x), SIGNOF(selectedEntity->scale.y)) * 0.125f;
			selectedEntity->scale += add;
			printf("add(%f, %f)\n", add.x, add.y);
		}
		if (Input::IsKeyPressed(keyScaleDown))
		{
			selectedEntity->scale -= Vector2(SIGNOF(selectedEntity->scale.x), SIGNOF(selectedEntity->scale.y)) * 0.125f;
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

		if (!waitForLMBRelease && Input::IsKeyHeld(keyScale))
		{
			Debug::Log("scale start");
			moveStartPosition = Input::GetWorldMousePosition();
			startScale = selectedEntity->scale;
			SetState(FTES_SCALE);
			return;
		}

		if (!waitForLMBRelease && Input::IsKeyPressed(keyMove))//Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) //
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
		{
			if (moveAxisLock == 1)
				moveAxisLock = 0;
			else
				moveAxisLock = 1;
		}
		else if (Input::IsKeyPressed(KEY_Y))
		{
			if (moveAxisLock == 2)
				moveAxisLock = 0;
			else
				moveAxisLock = 2;
		}

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
			printf("position: (%d, %d)\n", (int)selectedEntity->position.x, (int)selectedEntity->position.y);
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
		if (Input::IsKeyPressed(KEY_X))
		{
			if (moveAxisLock == 1)
				moveAxisLock = 0;
			else
				moveAxisLock = 1;
		}
		else if (Input::IsKeyPressed(KEY_Y))
		{
			if (moveAxisLock == 2)
				moveAxisLock = 0;
			else
				moveAxisLock = 2;
		}

		Vector2 dir = (Input::GetWorldMousePosition() - moveStartPosition);
		float mag = fabs(dir.y);
		if (fabs(dir.x) > fabs(dir.y))
			mag = fabs(dir.x);

		Vector2 add = Vector2(SIGNOF(selectedEntity->scale.x), SIGNOF(selectedEntity->scale.y)) * mag;

		selectedEntity->scale = startScale + add * 0.01f;

		switch(moveAxisLock)
		{
		case 1:
			selectedEntity->scale.y = startScale.y;
			break;
		case 2:
			selectedEntity->scale.x = startScale.x;
			break;
		}
		
		if (Input::IsKeyPressed(KEY_0))
		{
			selectedEntity->scale = Vector2::one;
			SetState(FTES_NONE);
			return;
		}

		// cancel out of move by hitting escape or rmb
		if (Input::IsKeyPressed(KEY_ESCAPE) || Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			selectedEntity->scale = startScale;
			SetState(FTES_NONE);
			return;
		}

		// if let go, return to none state
		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || Input::IsKeyPressed(keyScale) || Input::IsKeyPressed(keySelect))
		{
			SetState(FTES_NONE);
			return;
		}
	}

	void FringeTileEditor::SetState(FringeTileEditorState state)
	{
		this->state = state;
	}
}
