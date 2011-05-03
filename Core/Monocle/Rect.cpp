#include "Rect.h"

namespace Monocle
{
    Rect::Rect()
    {
    }
    
    Rect::Rect(const Vector2& topLeft, const Vector2& bottomRight):
        topLeft(topLeft), bottomRight(bottomRight)
    {
        
    }
    
    Rect::Rect(const Rect& other):
        topLeft(other.topLeft), bottomRight(other.bottomRight)
    {
        
    }
}