//
//  OpenGLFrameBuffer.h
//  Monocle
//
//  Created by Josh Whelchel on 5/31/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#ifdef MONOCLE_OPENGL

#include "../Vector2.h"

#define GLEW_STATIC
#include <GL/glew.h>

namespace Monocle {
    
    class OpenGLFrameBuffer
    {
    public:
        
        OpenGLFrameBuffer( int w, int h, bool highFilter = true );
        ~OpenGLFrameBuffer();
        
        static OpenGLFrameBuffer *NewFullBuffer( bool highFilter = true );
        
        Vector2 originalSize;
        Vector2 originalVirtualSize;
        
        void Use( bool set2D = false );
        void Stop();
        
        void Bind();
        
        void BindTexture();
        
        GLuint fboID;
        int width;
        int height;
        
        GLuint texID;
        
        GLuint depID;
    };
    
}

#endif
