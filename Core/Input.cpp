#include "Input.h"
#include <stdio.h> // for null :P
#include "Debug.h"
#include "Graphics.h"
#include "Camera.h"
#include "Scene.h"

namespace Monocle
{
	Input *Input::instance = NULL;

    Input::EventHandler::~EventHandler()
    {
        Input::RemoveHandler(this);
    }

	Input::Input()
	{
		instance = this;
	}

	void Input::Init()
	{
		for (int i = 0; i < (int)KEY_MAX; i++)
			previousKeys[i] = currentKeys[i] = false;

		for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
			previousMouseButtons[i] = currentMouseButtons[i] = false;
	}

	void Input::Update()
	{
		for (int i = 0; i < (int)KEY_MAX; i++)
		{
			previousKeys[i] = currentKeys[i];
			currentKeys[i] = Platform::keys[i];
			
			if(currentKeys[i] != previousKeys[i] && handlers.size() > 0)
			{
			    if( currentKeys[i] )
                {
                    for(std::list<EventHandler*>::iterator it = handlers.begin(); it != handlers.end(); it++)
		                (*it)->OnKeyPress((KeyCode)i);
                }else
                {
                    for(std::list<EventHandler*>::iterator it = handlers.begin(); it != handlers.end(); it++)
		                (*it)->OnKeyRelease((KeyCode)i);
                }
			}
		}

		for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
		{
		    previousMouseButtons[i] = currentMouseButtons[i];
			currentMouseButtons[i] = Platform::mouseButtons[i];
			
			if( previousMouseButtons[i] != currentMouseButtons[i] && handlers.size() > 0 )
		    {
		        Vector2 mousePos = GetMousePosition();
		        if(currentMouseButtons[i])
		        {
		            for(std::list<EventHandler*>::iterator it = handlers.begin(); it != handlers.end(); it++)
		                (*it)->OnMousePress(mousePos, (MouseButton)i);
		        }
		        else
		        {
		            for(std::list<EventHandler*>::iterator it = handlers.begin(); it != handlers.end(); it++)
		                (*it)->OnMouseRelease(mousePos, (MouseButton)i);
		        }
		    }
		}
		
		if(Platform::mouseScroll != 0 && handlers.size() > 0)
		{
		    for(std::list<EventHandler*>::iterator it = handlers.begin(); it != handlers.end(); it++)
                (*it)->OnMouseScroll(Platform::mouseScroll);
		}
		lastMouseScroll = Platform::mouseScroll;
		
		if(lastMousePos != Platform::mousePosition && handlers.size() > 0)
		{
		    for(std::list<EventHandler*>::iterator it = handlers.begin(); it != handlers.end(); it++)
                (*it)->OnMouseMove(Platform::mousePosition);
		}
		lastMousePos = Platform::mousePosition;
	}

	//Keys API
	bool Input::IsKeyHeld(KeyCode keyCode)
	{
		return instance->currentKeys[(int)keyCode];
	}

	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		return instance->currentKeys[(int)keyCode] && !instance->previousKeys[(int)keyCode];
	}

	bool Input::IsKeyReleased(KeyCode keyCode)
	{
		return !instance->currentKeys[(int)keyCode] && instance->previousKeys[(int)keyCode];
	}

	//Mouse API
	Vector2 Input::GetMousePosition()
	{
		///HACK: optimize this later, probably don't need so many function calls
		return Vector2((Platform::mousePosition.x / Platform::GetWidth()) * Graphics::GetVirtualWidth(), (Platform::mousePosition.y / Platform::GetHeight()) * Graphics::GetVirtualHeight());
	}

	Vector2 Input::GetWorldMousePosition()
	{
		Camera *mainCamera = Scene::GetMainCamera();
		Vector2 resScale = Graphics::GetResolutionScale();
		Vector2 invResScale = Vector2(1.0f/resScale.x, 1.0f/resScale.y);
		Vector2 diff = (Platform::mousePosition*invResScale) - Graphics::GetScreenCenter();
		Vector2 cameraZoom = mainCamera->scale;
		return mainCamera->position + (diff * Vector2(1/cameraZoom.x, 1/cameraZoom.y));
	}

	int Input::GetMouseScroll()
	{
		return Platform::mouseScroll;
	}

	bool Input::IsMouseButtonHeld(MouseButton mouseButton)
	{
		return Platform::mouseButtons[(int)mouseButton];
	}

	bool Input::IsMouseButtonReleased(MouseButton mouseButton)
	{
		return !instance->currentMouseButtons[(int)mouseButton] && instance->previousMouseButtons[(int)mouseButton];
	}

	bool Input::IsMouseButtonPressed(MouseButton mouseButton)
	{
		return instance->currentMouseButtons[(int)mouseButton] && !instance->previousMouseButtons[(int)mouseButton];
	}

	//KeyMask API
	void Input::DefineMaskKey(const std::string& mask, KeyCode keyCode)
	{
#ifdef DEBUG
		//ERROR: If the mask already contains that keys
		if (MaskHasKey(mask, keyCode))
			Debug::Log("ERROR: Defining a key to a keymask that it is already in.");
#endif
		instance->keyMasks[mask].push_back(keyCode);
	}

	void Input::UndefineMaskKey(const std::string& mask, KeyCode keyCode)
	{
#ifdef DEBUG
		//ERROR: If the mask doesn't contain that key
		if (!MaskHasKey(mask, keyCode))
			Debug::Log("ERROR: Undefining a key from a keymask that it isn't in.");
#endif
		instance->keyMasks[mask].remove(keyCode);
	}

	void Input::UndefineMaskAll(const std::string& mask)
	{
		instance->keyMasks[mask].clear();
	}

	std::list<KeyCode>* Input::GetMaskKeys(const std::string& mask)
	{
		return new std::list<KeyCode>(instance->keyMasks[mask]);
	}

	bool Input::MaskHasKey(const std::string& mask, KeyCode keyCode)
	{
		for (std::list<KeyCode>::iterator i = instance->keyMasks[mask].begin(); i != instance->keyMasks[mask].end(); ++i)
		{
			if ((*i) == keyCode)
				return true;
		}
		return false;
	}

	bool Input::IsKeyMaskHeld(const std::string& mask)
	{
		for (std::list<KeyCode>::iterator i = instance->keyMasks[mask].begin(); i != instance->keyMasks[mask].end(); ++i)
		{
			if (IsKeyHeld(*i))
				return true;
		}
		return false;
	}

	bool Input::IsKeyMaskReleased(const std::string& mask)
	{
		for (std::list<KeyCode>::iterator i = instance->keyMasks[mask].begin(); i != instance->keyMasks[mask].end(); ++i)
		{
			if (IsKeyReleased(*i))
				return true;
		}
		return false;
	}

	bool Input::IsKeyMaskPressed(const std::string& mask)
	{
		for (std::list<KeyCode>::iterator i = instance->keyMasks[mask].begin(); i != instance->keyMasks[mask].end(); ++i)
		{
			if (IsKeyPressed(*i))
				return true;
		}
		return false;
	}
	
	void Input::AddHandler(EventHandler *handler)
	{
	    instance->handlers.push_back(handler);
	}
	
	void Input::RemoveHandler(EventHandler *handler)
	{
	    instance->handlers.remove(handler);
	}
}
