#pragma once

#include <Scene.h>
#include <Input.h>

class InputHandlerTestScene : public Monocle::Scene, public Monocle::Input::EventHandler
{
public:
    void Begin()
    {
        Monocle::Input::AddHandler(this);
    }
    
    void OnKeyPress(Monocle::KeyCode key)
    {
        Monocle::Debug::Log("Key Pressed!");
    }
    
    void OnKeyRelease(Monocle::KeyCode key)
    {
        Monocle::Debug::Log("Key Released!");
    }
    
    void OnMousePress(Monocle::Vector2 mousePos, Monocle::MouseButton button)
    {
        if(button == Monocle::MOUSE_BUTTON_LEFT)
            Monocle::Debug::Log("Left Mouse Button pressed!");
        if(button == Monocle::MOUSE_BUTTON_RIGHT)
            Monocle::Debug::Log("Right Mouse Button pressed!");
        if(button == Monocle::MOUSE_BUTTON_MIDDLE)
            Monocle::Debug::Log("Middle Mouse Button pressed!");
    }
    
    void OnMouseRelease(Monocle::Vector2 mousePos, Monocle::MouseButton button)
    {
        if(button == Monocle::MOUSE_BUTTON_LEFT)
            Monocle::Debug::Log("Left Mouse Button released!");
        if(button == Monocle::MOUSE_BUTTON_RIGHT)
            Monocle::Debug::Log("Right Mouse Button released!");
        if(button == Monocle::MOUSE_BUTTON_MIDDLE)
            Monocle::Debug::Log("Middle Mouse Button released!");
    }
    
    void OnMouseScroll(int scrollDelta)
    {
        Monocle::Debug::Log("Mouse Scrolled!");
        Monocle::Debug::Log(scrollDelta);
    }
    
    void OnMouseMove(Monocle::Vector2 mousePos)
    {
        Monocle::Debug::Log("Mouse Moved!");
        Monocle::Debug::Log(mousePos);
    }
};
