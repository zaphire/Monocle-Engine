#include "Editor.h"
#include "Input.h"
#include "Scene.h"
#include "Entity.h"
#include "MonocleToolkit.h"
#include "Game.h"
#include <cstdio>

namespace Monocle
{

	Editor::Editor() : Entity()
	{
		// set layer on top of foreground selectable layer (i.e. avoid having this entity be considered for selection)
		SetLayer(Debug::layerMin - 1);
	}

	// update the camera controls
	void CameraEditor::UpdateCamera()
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

			Camera *mainCamera = Game::GetScene()->GetMainCamera();

			if (Platform::mouseScroll != 0)
			{
				const float minZoom = 0.01f;
				mainCamera->scale += (Platform::mouseScroll * Vector2::one * 0.00025f);
				if (mainCamera->scale.x < minZoom)
				{
					mainCamera->scale = Vector2::one * minZoom;
				}
				Vector2 camZoom = mainCamera->scale;
				printf("camZoom (%f, %f)\n", camZoom.x, camZoom.y);
			}

			if (Input::IsKeyPressed(KEY_LALT))
			{
				lastWorldMousePosition = Input::GetWorldMousePosition();
			}
			else if (Input::IsKeyHeld(KEY_LALT))
			{
				Vector2 diff = Input::GetWorldMousePosition() - lastWorldMousePosition;
				mainCamera->scale += Vector2::one * diff.y * Monocle::deltaTime * 0.05f;
				lastWorldMousePosition = Input::GetWorldMousePosition();
			}

			if (Input::IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) || Input::IsKeyPressed(KEY_LSHIFT))
			{
				lastWorldMousePosition = Input::GetWorldMousePosition();
			}
			else if (Input::IsMouseButtonHeld(MOUSE_BUTTON_MIDDLE) || Input::IsKeyHeld(KEY_LSHIFT))
			{
				Vector2 diff = Input::GetWorldMousePosition() - lastWorldMousePosition;
				mainCamera->position += (-1*diff);
				lastWorldMousePosition = Input::GetWorldMousePosition();

				Vector2 camPos = mainCamera->position;
				printf("camPos (%d, %d)\n", (int)camPos.x, (int)camPos.y);
			}

			Vector2 moveDiff = (Input::IsKeyHeld(KEY_KP4) ? Vector2::left : Vector2::zero) + (Input::IsKeyHeld(KEY_KP6) ? Vector2::right : Vector2::zero);
			moveDiff += (Input::IsKeyHeld(KEY_KP8) ? Vector2::up : Vector2::zero) + (Input::IsKeyHeld(KEY_KP5) ? Vector2::down : Vector2::zero);

			const float cameraMoveSpeed = 800.0f; // replace with virtualWidth
			moveDiff *= Monocle::deltaTime * cameraMoveSpeed;
			mainCamera->position += moveDiff;

			float zoomDiff = (Input::IsKeyHeld(KEY_KP7) ? -1.0f : 0.0f) + (Input::IsKeyHeld(KEY_KP9) ? 1.0f : 0.0f);
			const float cameraZoomSpeed = 1.0f;
			mainCamera->scale += zoomDiff * cameraZoomSpeed * Vector2::one * Monocle::deltaTime;
		}
	}
}
