#pragma once

#include "Platform.h"
#include "Vector2.h"
#include <string>
#include <map>
#include <list>

namespace Monocle
{    
	//!
	//! \brief Manages input for Monocle
	//! 
	//! Is*Held = is key/mouse button currently being held
	//! Is*Pressed = was the key/mouse button go down in this frame
	//! Is*Released = did the key/mouse button go up in this frame
	//! 
	class Input
	{
	public:
        class EventHandler
        {
        public:
            virtual void OnKeyPress(Monocle::KeyCode key) {}
            virtual void OnKeyRelease(Monocle::KeyCode key) {}
            
            virtual void OnMouseMove(Vector2 mousePosition) {}
            virtual void OnMousePress(Vector2 mousePosition, MouseButton button) {}
            virtual void OnMouseRelease(Vector2 mousePosition, MouseButton button) {}
            virtual void OnMouseScroll(int scrollDelta) {}
        };
	
		Input();
		void Init();

		//Mouse API
		//! get mouse position relative to virtual screen
		static Vector2 GetMousePosition();
		//! take camera into account
		static Vector2 GetWorldMousePosition();
		//! get the scroll wheel
		static int GetMouseScroll();

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

		static void AddHandler(EventHandler *handler);
		
		void Update();
		
	private:
		static Input *instance;

		bool previousKeys[KEY_MAX];
		bool currentKeys[KEY_MAX];
	
		bool previousMouseButtons[MOUSE_BUTTON_MAX];
		bool currentMouseButtons[MOUSE_BUTTON_MAX];

		std::map<std::string, std::list<KeyCode> > keyMasks;
		int lastMouseScroll;
		Vector2 lastMousePos;
		std::list < EventHandler* > handlers;
	};
}
