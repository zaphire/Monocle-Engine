//
//  FrameBuffer.cpp
//  Monocle
//
//  Created by Josh Whelchel on 5/31/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#ifdef MONOCLE_OPENGLES

#include "OpenGLESFrameBuffer.h"
#include "../MonocleToolkit.h"
#include "../Platform.h"
#include "../Graphics.h"
#include "../Debug.h"

namespace Monocle
{
    OpenGLFrameBuffer::OpenGLFrameBuffer( int w, int h, bool highFilter )
    {
        GLenum mode;
        height = h;
        width = w;
        
        mode = highFilter ? GL_LINEAR : GL_NEAREST;
        
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        
        glGenBuffers(1, &fboID);
        glBindFramebuffer(GL_FRAMEBUFFER ,fboID);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
        
        glGenRenderbuffers(1, &depID);
        glBindRenderbuffer(GL_RENDERBUFFER, depID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depID);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
        {
            Debug::Log("FrameBuffer failed setup");
            // bad?
        }
        else
        {
            Debug::Log("new FrameBuffer("+StringOf(width)+"x"+StringOf(height)+") (texID:"+StringOf(texID)+") = ID: "+StringOf(fboID));
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        originalSize = Vector2(Graphics::GetVirtualWidth(), Graphics::GetVirtualHeight());
    }
    
    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &fboID);
        glDeleteTextures(1, &texID);
    }
    
    
    void OpenGLFrameBuffer::Use( bool set2D )
    {
        originalSize = Vector2(Platform::GetWidth(), Platform::GetHeight());
        originalVirtualSize = Vector2(Graphics::GetVirtualWidth(), Graphics::GetVirtualHeight());
        
        glBindFramebuffer(GL_FRAMEBUFFER, fboID);
//        glPushAttrib(GL_VIEWPORT_BIT);
        
        Graphics::Resize(width,height);
        
//        glViewport(0,0, width, height);
        
        if (set2D)
            Graphics::Set2D(width,height);
        
        // Render as normal here
        // output goes to the FBO and it’s attached buffers
    }
    
    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    }
    
    void OpenGLFrameBuffer::Stop()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        Graphics::Resize(originalSize.x,originalSize.y);
//        Graphics::Set2D(originalVirtualSize.x,originalVirtualSize.y);
    }
    
    void OpenGLFrameBuffer::BindTexture()
    {
        glBindTexture(GL_TEXTURE_2D, texID);
    }
    
    OpenGLFrameBuffer *OpenGLFrameBuffer::NewFullBuffer( bool highFilter )
    {
        return new OpenGLFrameBuffer(Platform::GetWidth(),Platform::GetHeight(),highFilter);
    }
}

#endif
