#pragma once

#include <OpenGL/CGLTypes.h>

extern NSOpenGLContext* Cocoa_GL_CreateContext();
extern int Cocoa_GL_MakeCurrent(NSWindow* window, NSOpenGLContext* context);
extern void Cocoa_GL_SwapWindow();
extern void Cocoa_GL_DeleteContext(NSOpenGLContext* context);
extern int Cocoa_GL_SetSwapInterval(int interval);
extern int Cocoa_GL_GetSwapInterval();

/* vi: set ts=4 sw=4 noexpandtab: */

