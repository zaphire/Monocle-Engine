#include "Editor.h"
#include "Input.h"
#include "Scene.h"
#include "Entity.h"
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

			if (Platform::mouseScroll != 0)
			{
				const float minZoom = 0.01f;
				Scene::GetCamera()->scale += (Platform::mouseScroll * Vector2::one * 0.00025f);
				if (Scene::GetCamera()->scale.x < minZoom)
				{
					Scene::GetCamera()->scale = Vector2::one * minZoom;
				}
				Vector2 camZoom = Scene::GetCamera()->scale;//Graphics::GetCameraZoom();
				printf("camZoom (%f, %f)\n", camZoom.x, camZoom.y);
			}
			if (Input::IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
			{
				lastWorldMousePosition = Input::GetWorldMousePosition();
			}
			if (Input::IsMouseButtonHeld(MOUSE_BUTTON_MIDDLE))
			{
				Vector2 diff = Input::GetWorldMousePosition() - lastWorldMousePosition;
				Scene::GetCamera()->position += (-1*diff);
				lastWorldMousePosition = Input::GetWorldMousePosition();

				Vector2 camPos = Scene::GetCamera()->position;
				printf("camPos (%d, %d)\n", (int)camPos.x, (int)camPos.y);
				//const float camPanSpeed = 8.0f;
				//Graphics::AdjustCameraPosition((Input::GetMousePosition() - startCameraMovePosition)*camPanSpeed*Monocle::deltaTime);//(Input::GetWorldMousePosition() - Graphics::GetCameraPosition()));
			}
		}
	}
}
