#include "Quad.h"

namespace Monocle
{
    Quad::Quad():
        tl(), br()
    {
    }
    
    Quad::Quad(const Vector2& tl, const Vector2& br):
        tl(tl), br(br)
    {
        
    }
    
    Quad::Quad(const Quad& other):
        tl(other.tl), br(other.br)
    {
        
    }
}