#pragma once

#include "Platform.h"
#include "Vector2.h"
#include <string>
#include <vector>
#include <map>
#include <list>

namespace Monocle
{
	class Input
	{
	public:
		Input();
		void Init();

		//Mouse API
		static Vector2 GetMousePosition();
		static bool IsMouseButtonHeld(MouseButton mouseButton);
		static bool IsMouseButtonReleased(MouseButton mouseButton);
		static bool IsMouseButtonPressed(MouseButton mouseButton);

		//Keys API
		static bool IsKeyHeld(KeyCode keyCode);
		static bool IsKeyReleased(KeyCode keyCode);
		static bool IsKeyPressed(KeyCode keyCode);

		//Key Mask API
		static void DefineMaskKey(const std::string& mask, KeyCode keyCode);
		static void UndefineMaskKey(const std::string& mask, KeyCode keyCode);
		static void UndefineMaskAll(const std::string& mask);
		static std::list<KeyCode>* GetMaskKeys(const std::string& mask);
		static bool MaskHasKey(const std::string& mask, KeyCode keyCode);
		static bool IsKeyMaskHeld(const std::string& mask);
		static bool IsKeyMaskReleased(const std::string& mask);
		static bool IsKeyMaskPressed(const std::string& mask);

		static Vector2 GetWorldMousePosition();

		void Update();
		
	private:
		static Input *instance;

		bool previousKeys[KEY_MAX];
		bool currentKeys[KEY_MAX];
	
		bool previousMouseButtons[MOUSE_BUTTON_MAX];
		bool currentMouseButtons[MOUSE_BUTTON_MAX];

		std::map<std::string, std::list<KeyCode> > keyMasks;
	};
}