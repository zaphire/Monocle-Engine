#include "Editor.h"
#include "Input.h"
#include "Scene.h"
#include <cstdio>

namespace Monocle
{
	Editor::Editor()
	{

	}

	void Editor::Update()
	{

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

			if (Platform::mouseWheel != 0)
			{
				Scene::GetCamera()->scale += (Platform::mouseWheel * Vector2::one * 0.00025f);
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
