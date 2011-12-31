#include "Input.h"
#include <stdio.h> // for null :P
#include "Debug.h"
#include "Graphics.h"
#include "Camera.h"
#include "Scene.h"
#include "Game.h"

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
		worldMouseCamera = NULL;

		for (int i = 0; i < (int)KEY_MAX; i++)
			previousKeys[i] = currentKeys[i] = false;

		for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
			previousMouseButtons[i] = currentMouseButtons[i] = false;
        
        for (int i = 0; i < TOUCHES_MAX; i++)
            Platform::touches[i].touched = 0;
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
        
        /** Push out the ending touches and move existing beginning touches to stationary (so they don't retrigger) **/
        for (int i=0; i<TOUCHES_MAX; i++)
        {
            Touch *t = &Platform::touches[i];
            if (t->phase == TOUCH_PHASE_ENDED && t->touched > 0){
                t->phase = TOUCH_PHASE_NONE;
                t->touched = 0;
            }
            
            if (t->phase == TOUCH_PHASE_BEGIN && t->touched > 0){
                t->phase = TOUCH_PHASE_STATIONARY;
                t->touched = 0;
            }
            
            t->touched++;
        }
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

	Vector2 Input::GetWorldMousePosition(Camera *camera)
	{
		if (!camera)
		{
			if (instance->worldMouseCamera)
			{
				camera = instance->worldMouseCamera;
			}
			else
			{
				camera = Game::GetScene()->GetMainCamera();
			}
		}
		Vector2 resScale = Graphics::GetResolutionScale();
		Vector2 invResScale = Vector2(1.0f/resScale.x, 1.0f/resScale.y);
		Vector2 adjustedToCameraMousePosition = Platform::mousePosition;

		adjustedToCameraMousePosition = adjustedToCameraMousePosition / Vector2(camera->viewport.width, camera->viewport.height);

		Vector2 adjust = Vector2(-camera->viewport.x * Platform::GetWidth() * 2.0f, -(1.0 - (camera->viewport.height + camera->viewport.y)) * Platform::GetHeight() * 2.0f);
		//printf("adjust (%f, %f)\n", adjust.x, adjust.y);
		adjustedToCameraMousePosition += adjust;

		Vector2 diff = (adjustedToCameraMousePosition * invResScale) - Graphics::GetScreenCenter();
		Vector2 cameraZoom = camera->scale;
		return camera->position + (diff * Vector2(1/cameraZoom.x, 1/cameraZoom.y));
	}

	void Input::SetWorldMouseCamera(Camera *camera)
	{
		instance->worldMouseCamera = camera;
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
    
    Touch *Input::GetTouchWithStatus( TouchPhase phase, int index )
    {
        int numTouches = Platform::numTouches;
        int sk=0;
        
        for (int i=0;i<numTouches&&i<TOUCHES_MAX;i++)
        {
            Touch *t = &Platform::touches[i];
            if ((phase == TOUCH_PHASE_ANY && t->phase != TOUCH_PHASE_NONE) || t->phase == phase){
                if (sk==index) return t;
                sk++;
            }
        }
        
        return NULL;
    }
    
    int Input::TouchCountWithPhase( TouchPhase phase )
    {
        int numTouches = Platform::numTouches;
        int num=0;
        
        for (int i=0;i<numTouches&&i<TOUCHES_MAX;i++)
        {
            Touch *t = &Platform::touches[i];
            if ((phase == TOUCH_PHASE_ANY && t->phase != TOUCH_PHASE_NONE) || t->phase == phase){
                num++;
            }
        }
        
        return num;
    }
    
    Touch *Input::IsTouchBeginning( int index )
    {
        return GetTouchWithStatus( TOUCH_PHASE_BEGIN, index );
    }
    
    Touch *Input::IsTouchEnding( int index )
    {
        return GetTouchWithStatus( TOUCH_PHASE_ENDED, index );
    }
    
    Touch *Input::IsTouchMoving( int index )
    {
        return GetTouchWithStatus( TOUCH_PHASE_MOVED, index );
    }
    
    Touch *Input::IsTouchWithIndexInRect( Vector2 topLeft, Vector2 bottomRight, TouchPhase phase, int index )
    {
        Touch *t = GetTouchWithStatus( phase, index );
        if (!t) return NULL;
        
        if (t->position.x >= topLeft.x &&
            t->position.x <= bottomRight.x &&
            t->position.y >= topLeft.y &&
            t->position.y <= bottomRight.y)
            return t;
        
        return NULL;
    }
    
    Touch *Input::IsTouchInRect( Vector2 topLeft, Vector2 bottomRight, TouchPhase phase )
    {
        int numTouches = Platform::numTouches;
        
        for (int i=0;i<numTouches&&i<TOUCHES_MAX;i++)
        {
            Touch *t = &Platform::touches[i];
            if ((phase == TOUCH_PHASE_ANY && t->phase != TOUCH_PHASE_NONE) || t->phase == phase){
                if (t->position.x >= topLeft.x &&
                    t->position.x <= bottomRight.x &&
                    t->position.y >= topLeft.y &&
                    t->position.y <= bottomRight.y){
//                    printf("T: (%f,%f)\n",t->position.x,t->position.y);
                    return t;
                }
            }
        }
        
        return NULL;
    }

    int Input::TouchCount()
    {
        return Platform::numTouches;
    }
}
