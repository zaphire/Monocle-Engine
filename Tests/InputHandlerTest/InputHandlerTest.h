#pragma once

#include <Scene.h>
#include <Entity.h>
#include <Input.h>

class EntityInputHandler : public Monocle::Entity, public Monocle::Input::EventHandler
{
public:
    ~EntityInputHandler() { }
    void OnKeyPress() { }
};

class InputHandlerTestScene : public Monocle::Scene, public Monocle::Input::EventHandler
{
public:
    void Begin()
    {
        Monocle::Input::AddHandler(this);
        Monocle::Input::AddHandler(entity = new EntityInputHandler());
    }
    
    void OnKeyPress(Monocle::KeyCode key)
    {
        Monocle::Debug::Log("Key Pressed!");
        
        if(key == Monocle::KEY_R)
        {
            if(entity)
                delete entity;
            entity = NULL;
        }
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
    
private:
    EntityInputHandler *entity;
};
