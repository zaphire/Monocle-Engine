#pragma once

#include "Platform.h"
#include "Vector2.h"
#include <string>
#include <map>
#include <list>

namespace Monocle
{    
	class Camera;

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
		//! Base for classes that handle input events.
		//! Classes that inherit from Input::EventHandler may
		//! be added to the list of handlers by calling Input::AddHandler()
        class EventHandler
        {
        public:
            virtual ~EventHandler();
            virtual void OnKeyPress(Monocle::KeyCode key) {}
            virtual void OnKeyRelease(Monocle::KeyCode key) {}
            
            virtual void OnMouseMove(Vector2 mousePosition) {}
            virtual void OnMousePress(Vector2 mousePosition, MouseButton button) {}
            virtual void OnMouseRelease(Vector2 mousePosition, MouseButton button) {}
            virtual void OnMouseScroll(int scrollDelta) {}
        };
	
		

		//Mouse API
		//! get mouse position relative to virtual screen
		static Vector2 GetMousePosition();
		//! take camera into account
		static Vector2 GetWorldMousePosition(Camera *camera = NULL);
		//! get the scroll wheel
		static int GetMouseScroll();

		//! Determines whether the given mouse button is
		//! currently depressed and has been for at least
		//! one previous Input::Update() call.
		//! \param mouseButton The mouse button to check
		static bool IsMouseButtonHeld(MouseButton mouseButton);
		//! Determines whether the given mouse button was
		//! released this frame.
		//! \param mouseButton The mouse button to check
		static bool IsMouseButtonReleased(MouseButton mouseButton);
		//! Determines whether the given mouse button was
		//! pressed this frame.
		//! \param mouseButton The mouse button to check
		static bool IsMouseButtonPressed(MouseButton mouseButton);

		//Keys API
		//! Determines whether the given key is currently
		//! depressed and has been for at least one previous
		//! Input::Update() call
		//! \param keyCode The key to check
		static bool IsKeyHeld(KeyCode keyCode);
		//! Determines whether the given key was released
		//! this frame.
		//! \param keyCode The key to check
		static bool IsKeyReleased(KeyCode keyCode);
		//! Determines whether the given key was pressed
		//! this frame.
		//! \param keyCode The key to check
		static bool IsKeyPressed(KeyCode keyCode);

		//Key Mask API
		//! Adds a key to the given mask.
		static void DefineMaskKey(const std::string& mask, KeyCode keyCode);
		//! Removes a key from the given mask.
		static void UndefineMaskKey(const std::string& mask, KeyCode keyCode);
		//! Removes all keys from the given mask.
		static void UndefineMaskAll(const std::string& mask);
		//! Returns a list of the keys currently in the given mask.
		static std::list<KeyCode>* GetMaskKeys(const std::string& mask);
		//! Determines whether the given mask contains the given key.
		static bool MaskHasKey(const std::string& mask, KeyCode keyCode);
		//! Determines whether any of the keys in the given mask
		//! are held.
		static bool IsKeyMaskHeld(const std::string& mask);
		//! Determines whether any of the keys in the given mask
		//! has been released this frame.
		static bool IsKeyMaskReleased(const std::string& mask);
		//! Determines whether any of the keys in the given mask
		//! has been pressed this frame.
		static bool IsKeyMaskPressed(const std::string& mask);
        
        //Touch API
        static Touch *GetTouchWithStatus( TouchPhase phase, int index = 0 );
        static Touch *IsTouchBeginning( int index = 0 );
        static Touch *IsTouchEnding( int index = 0 );
        static Touch *IsTouchMoving( int index = 0 );
        static int TouchCount();
        static int TouchCountWithPhase( TouchPhase phase );
        static Touch *IsTouchInRect( Vector2 topLeft, Vector2 bottomRight, TouchPhase phase = TOUCH_PHASE_ANY );
        
        //! Determines if there is a touch with a specific index in the rectangle provided. This is for when you expect multi-touch.
        static Touch *IsTouchWithIndexInRect( Vector2 topLeft, Vector2 bottomRight, TouchPhase phase = TOUCH_PHASE_ANY, int index = 0 );

		//! Adds an event handler to the callback list.
		static void AddHandler(EventHandler *handler);
		//! Removes an event handler from the callback
		//! list.
		static void RemoveHandler(EventHandler *handler);
		

		static void SetWorldMouseCamera(Camera *camera);

		void Update();
	
	protected:
		friend class Game;
		Input();
		void Init();
	
	private:
		static Input *instance;

		bool previousKeys[KEY_MAX];
		bool currentKeys[KEY_MAX];
	
		bool previousMouseButtons[MOUSE_BUTTON_MAX];
		bool currentMouseButtons[MOUSE_BUTTON_MAX];

		Camera *worldMouseCamera;

		std::map<std::string, std::list<KeyCode> > keyMasks;
		int lastMouseScroll;
		Vector2 lastMousePos;
		std::list < EventHandler* > handlers;
	};
}
