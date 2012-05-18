/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#ifndef GLUT_STUFF_H
#define GLUT_STUFF_H

#ifdef _WIN32//for glut.h
#include <windows.h>
#endif

//think different
#if defined(__APPLE__) && !defined (VMDMESA)
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else


#ifdef _WINDOWS
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#ifdef __QNX__
#include <GLES/gl.h>
#include <GLES/glext.h>
#include "gleswrappers.h"
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif //__QNX__
#endif //_WINDOWS
#endif //APPLE

#ifdef _WINDOWS
#define BT_ACTIVE_ALT   VK_LMENU

#else

// Define GLUT constants for QNX.
#ifdef __QNX__
#define  GLUT_KEY_F1                        0x0001
#define  GLUT_KEY_F2                        0x0002
#define  GLUT_KEY_F3                        0x0003
#define  GLUT_KEY_F4                        0x0004
#define  GLUT_KEY_F5                        0x0005
#define  GLUT_KEY_F6                        0x0006
#define  GLUT_KEY_F7                        0x0007
#define  GLUT_KEY_F8                        0x0008
#define  GLUT_KEY_F9                        0x0009
#define  GLUT_KEY_F10                       0x000A
#define  GLUT_KEY_F11                       0x000B
#define  GLUT_KEY_F12                       0x000C

#define  GLUT_KEY_LEFT                      0x0064
#define  GLUT_KEY_UP                        0x0065
#define  GLUT_KEY_RIGHT                     0x0066
#define  GLUT_KEY_DOWN                      0x0067
#define  GLUT_KEY_PAGE_UP                   0x0068
#define  GLUT_KEY_PAGE_DOWN                 0x0069
#define  GLUT_KEY_HOME                      0x006A
#define  GLUT_KEY_END                       0x006B
#define  GLUT_KEY_INSERT                    0x006C

#define  GLUT_ACTIVE_SHIFT                  0x0001
#define  GLUT_ACTIVE_CTRL                   0x0002
#define  GLUT_ACTIVE_ALT                    0x0004
#endif // __QNX__

#define BT_KEY_K 'k'
#define BT_KEY_LEFT			GLUT_KEY_LEFT
#define BT_KEY_RIGHT		GLUT_KEY_RIGHT
#define BT_KEY_UP			GLUT_KEY_UP
#define BT_KEY_DOWN			GLUT_KEY_DOWN
#define	BT_KEY_F1			GLUT_KEY_F1
#define	BT_KEY_F2			GLUT_KEY_F2
#define	BT_KEY_F3			GLUT_KEY_F3
#define	BT_KEY_F4			GLUT_KEY_F4
#define	BT_KEY_F5			GLUT_KEY_F5
#define BT_KEY_PAGEUP		GLUT_KEY_PAGE_UP
#define BT_KEY_PAGEDOWN		GLUT_KEY_PAGE_DOWN
#define BT_KEY_END			GLUT_KEY_END
#define BT_KEY_HOME			GLUT_KEY_HOME
#define BT_ACTIVE_ALT		GLUT_ACTIVE_ALT
#define	BT_ACTIVE_CTRL		GLUT_ACTIVE_ALT
#define BT_ACTIVE_SHIFT		GLUT_ACTIVE_SHIFT
#endif

#if BT_USE_FREEGLUT
#include "GL/freeglut_ext.h" //to be able to return from glutMainLoop()
#endif



class DemoApplication;

int glutmain(int argc, char **argv,int width,int height,const char* title,DemoApplication* demoApp);

#if defined(BT_USE_DOUBLE_PRECISION)
#define btglLoadMatrix glLoadMatrixd
#define btglMultMatrix glMultMatrixd
#define btglColor3 glColor3d
#define btglVertex3 glVertex3d
#else
#define btglLoadMatrix glLoadMatrixf
#define btglMultMatrix glMultMatrixf
#define btglColor3 glColor3f
#define btglVertex3 glVertex3d
#endif

#endif //GLUT_STUFF_H
