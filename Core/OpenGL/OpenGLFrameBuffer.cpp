//
//  OpenGLFrameBuffer.cpp
//  Monocle
//
//  Created by Josh Whelchel on 5/31/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#ifdef MONOCLE_OPENGL

#include "OpenGLFrameBuffer.h"
#include "../Graphics.h"
#include "../MonocleToolkit.h"
#include "../Platform.h"
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
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        
        glGenBuffers(1, &fboID);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT ,fboID);
        
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texID, 0);
        
        glGenRenderbuffersEXT(1, &depID);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depID);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depID);
        
        if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)!=GL_FRAMEBUFFER_COMPLETE_EXT)
        {
            Debug::Log("FrameBuffer failed setup");
            // bad?
        }
        else
        {
            Debug::Log("new FrameBuffer("+StringOf(width)+"x"+StringOf(height)+") (texID:"+StringOf(texID)+") = ID: "+StringOf(fboID));
        }
        
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        
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
        
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
        glPushAttrib(GL_VIEWPORT_BIT);
        
        Graphics::Resize(width,height);
        
//        glViewport(0,0, width, height);
        
        if (set2D)
            Graphics::Set2D(width,height);
        
        // Render as normal here
        // output goes to the FBO and it’s attached buffers
    }
    
    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
    }
    
    void OpenGLFrameBuffer::Stop()
    {
        glPopAttrib();
        
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        
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
